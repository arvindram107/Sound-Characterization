/* 3. This file contains the application level descriptions which is a like a
shell that manages the sound buffers and the keyboard inputs and outputs. The actual
work of handling keyboard input and taking the associated actions gets in in the 
states of the state machine that runs the show. The app has access to that through
a matrix of function pointers and indices into that matrix to indicate which element
to fire currently. That index is altered by the states to change state. This 
architecture comes from the Fifth Version project of the Inck codebase. We need 
to move all the action oriented stuff that is integrated in the Sound Characterization 2
main.c file to the states in a new state machine that we define based on the analysis so 
far in the README.md file. As a first step, let's redefine the state machine to match up 
with our phases, steps and sub-steps, then implement the required actions. We want to 
have a tanpura, tabla and guitar track ready by the 8th, so that we can focus on 
multi-tracking and get the live track recorded in the last 5 days.
*/ 

/* 1. We're starting with the ASIO ready main file and adding in functions 
for all the state machine keyboard UI parts.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"

#include "Phases/Phases.h"
#include "Screen/Screen.h"

extern HWND g_hWnd;
extern BOOL g_bScreenInitialized;

LRESULT CALLBACK WndProc(
                    HWND hwnd, 
					UINT Message, 
					WPARAM wParam, 
					LPARAM lParam
                    ); 

int WINAPI WinMain(
              HINSTANCE hInstance, 
			  HINSTANCE hPrevInstance, 
			  LPSTR lpCmdLine, 
			  int nCmdShow
			  ); 


#define AUDIO_BUFFER_SIZE 1024
#define ASIO_DRIVER_NAME  "Fast Track C600 ASIO"
#define TwoPow31Minus1 2147483647
#define TwoPi 6.28318531

enum 
{
   // number of input and outputs supported by the host application
   // you can change these to higher or lower values
   kMaxInputChannels = 2,
   kMaxOutputChannels = 2
};


// internal data storage
typedef struct DriverInfo
{
   // ASIOInit()
   ASIODriverInfo driverInfo;

   // ASIOGetChannels()
   long           inputChannels;
   long           outputChannels;

   // ASIOGetBufferSize()
   long           minSize;
   long           maxSize;
   long           preferredSize;
   long           granularity;

   // ASIOGetSampleRate()
   ASIOSampleRate sampleRate;

   // ASIOOutputReady()
   bool           postOutput;

	// ASIOGetLatencies ()
	long           inputLatency;
	long           outputLatency;

	// ASIOCreateBuffers ()
	long inputBuffers;	// becomes number of actual created input buffers
	long outputBuffers;	// becomes number of actual created output buffers
	ASIOBufferInfo bufferInfos[kMaxInputChannels + kMaxOutputChannels]; // buffer info's

	// ASIOGetChannelInfo()
	ASIOChannelInfo channelInfos[kMaxInputChannels + kMaxOutputChannels]; // channel info's
	// The above two arrays share the same indexing, 
   // as the data in them are linked together

	// Information from ASIOGetSamplePosition()
	// data is converted to double floats for easier use, 
   // however 64 bit integer can be used, too
	double         nanoSeconds;
	double         samples;
	double         tcSamples;	// time code samples

	// bufferSwitchTimeInfo()
	ASIOTime       tInfo;	// time info state
	unsigned long  sysRefTime;// system reference time, when bufferSwitch() was called

	// Signal the end of processing in this example
	bool           stopped;
} DriverInfo;


DriverInfo asioDriverInfo = {0};
ASIOCallbacks asioCallbacks;

//----------------------------------------------------------------------------------
// some external references
extern AsioDrivers* asioDrivers;
bool loadAsioDriver(char *name);

long init_asio_static_data (DriverInfo *asioDriverInfo);
ASIOError create_asio_buffers (DriverInfo *asioDriverInfo);
unsigned long get_sys_reference_time();

// callback prototypes
void bufferSwitch(long index, ASIOBool processNow);
ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
void sampleRateChanged(ASIOSampleRate sRate);
long asioMessages(long selector, long value, void* message, double* opt);

#define MIXER_STATE_a 0
#define MIXER_STATE_b 1
#define MIXER_STATE_c 2
#define MIXER_STATE_d 3
#define MIXER_STATE_e 4


unsigned int g_uMixerState = MIXER_STATE_a;

void load_stereo_buffer( int *pnBuffer, int nLeftOrRight );
void load_tone( int *pnBuffer, int nLeftOrRight );
void load_tanpura( int *pnBuffer, int nLeftOrRight );
void load_tanpura_tabla( int *pnBuffer, int nLeftOrRight );
void load_tanpura_tabla_guitar( int *pnBuffer, int nLeftOrRight );
void load_all_tracks( int *pnBuffer, int nLeftOrRight );
void store_stereo_buffer( int *pnBuffer, int nLeftOrRight );

void  load_tanpura_buffer( int * pnBuffer, int nLeftOrRight );
void  load_tabla_buffer( int * pnBuffer, int nLeftOrRight );
void  load_guitar_buffer( int * pnBuffer, int nLeftOrRight );
void  load_live_buffer( int * pnBuffer, int nLeftOrRight );

FILE * g_pFileLive;



// This is where all the input to the window goes to
LRESULT CALLBACK WndProc(
                    HWND hwnd, 
					UINT Message, 
					WPARAM wParam, 
					LPARAM lParam
                    ) 
{
   PAINTSTRUCT ps;
   RECT rectScreen;


   switch (message)
   {
	  case WM_CREATE:
         ShowWindow(hWnd, SW_MAXIMIZE);
         g_hWnd = hWnd;
	  break;
      case WM_PAINT:
		 g_hWnd = hWnd;
		 BeginPaint( hWnd, &ps );
		 GetClientRect( hWnd, &rectScreen );
		 // paint the current screen( ps.hdc, rectScreen );
       EndPaint( hWnd, &ps );
	  break;
	  break;
      case WM_DESTROY:
         PostQuitMessage(0);
      break;
      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
      break;
   }
   return 0;
}


void respond_keystroke( UINT Message, WPARAM wParam, LPARAM lParam )
{
   if( Message == WM_KEYDOWN && wParam == VK_SPACE )
   {
	  // if it's stopped start it, else stop it
  	  if( asioDriverInfo.stopped )
  	  {
         if (ASIOStart() == ASE_OK)
         {
			   // Now all is up and running
			fprintf (stdout, "\nASIO Driver started successfully.\n\n");
			asioDriverInfo.stopped = false;
         }
  	  }
  	  else
  	  {
  	  	 // stop ASIO
         if (ASIOStop() == ASE_OK)
         {
			   // Now it's stopped
			fprintf (stdout, "\nASIO Driver stopped succefully.\n\n");
			asioDriverInfo.stopped = true;
         }
  	  }
   }
   else if( Message == WM_KEYDOWN && wParam == 0x48 ) // the "H" key
   {
      PostQuitMessage( 0 );
   }
}

// The 'main' function of Win32 GUI programs: this is where execution starts
int WINAPI WinMain(
              HINSTANCE hInstance, 
			     HINSTANCE hPrevInstance, 
			     LPSTR lpCmdLine, 
			     int nCmdShow
			     )  
{
	WNDCLASSEX wc; // A properties struct of our window 
	HWND hwnd; // A 'HANDLE', hence the H, or a pointer to our window 
	MSG Msg; // A temporary location for all messages 

	// zero out the struct and set the stuff we want to modify 
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; // This is where we will send messages to 
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); // Load a standard icon 
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) 
	{
		MessageBox(
		   NULL, 
		   "Window Registration Failed!",
		   "Error!",
		   MB_ICONEXCLAMATION|MB_OK
		   );
		return 0;
	}

	hwnd = CreateWindowEx(
	          WS_EX_CLIENTEDGE,
			  "WindowClass",
			  "Caption",
	          WS_POPUP|WS_MAXIMIZE|WS_SYSMENU, // these flags are a for full-screen
		      CW_USEDEFAULT, 
		      CW_USEDEFAULT, 
            CW_USEDEFAULT, 
		      CW_USEDEFAULT, 
		      NULL,
			   NULL,
			   hInstance,
			   NULL
			  );

	if(hwnd == NULL) 
	{
		MessageBox(
		   NULL, 
		   "Window Creation Failed!",
		   "Error!",
		   MB_ICONEXCLAMATION|MB_OK
		   );
		return 0;
	}

   g_pFileLive = fopen( "temp.raw", "wb" );

   // load ASIO driver
    // Init ASIO, (allocate the buffers)
	// load the driver, this will setup all the necessary internal data structures
 	if(loadAsioDriver(ASIO_DRIVER_NAME))
	{
		// initialize the driver
		if (ASIOInit (&asioDriverInfo.driverInfo) == ASE_OK)
		{
			printf(
            "asioVersion:   %d\n"
			   "driverVersion: %d\n"
			   "Name:          %s\n"
			   "ErrorMessage:  %s\n",
			   asioDriverInfo.driverInfo.asioVersion, 
            asioDriverInfo.driverInfo.driverVersion,
			   asioDriverInfo.driverInfo.name, 
            asioDriverInfo.driverInfo.errorMessage
            );
			if (init_asio_static_data (&asioDriverInfo) == 0)
			{
				// set up the asioCallback structure and create the ASIO data buffer
				asioCallbacks.bufferSwitch = &bufferSwitch;
				asioCallbacks.sampleRateDidChange = &sampleRateChanged;
				asioCallbacks.asioMessage = &asioMessages;
				asioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;
				if (create_asio_buffers (&asioDriverInfo) == ASE_OK)
				{
               // ASIO is now ready to be used.
               // run the apps message pump giving priority to keyboard messages
               while(GetMessage(&Msg, NULL, 0, 0) > 0) 
				   { 
	               if( Msg.message == WM_KEYDOWN || Msg.message == WM_KEYUP )
                  { // if its a keyboard message, gets its registration time
		              // and directly process it with the function array
                     long lTime = GetMessageTime();
		               g_ppfnRespond[g_uCurrentPhase]( 
		                  Msg.message, 
			               Msg.wParam, 
			               Msg.lParam,
			               lTime
			                                          );
                  }
	               else 
                  {	 // for non-keyboard messages, follow the usual route
                     TranslateMessage(&msg);
                     DispatchMessage(&msg);
                  }
				   }
               if( !asioDriverInfo.stopped )
				   {
    	            ASIOStop();
				   }
				   ASIODisposeBuffers();
				}
			}
			ASIOExit();
		}
		asioDrivers->removeCurrentDriver();
	}
   fclose( g_pFileLive );
	return Msg.wParam;
}

//----------------------------------------------------------------------------------
long init_asio_static_data (DriverInfo *asioDriverInfo)
{	
   asioDriverInfo->stopped = true;
    // collect the informational data of the driver
	// get the number of available channels
   if(ASIOGetChannels(&asioDriverInfo->inputChannels, &asioDriverInfo->outputChannels) == ASE_OK)
   {
      printf(
         "ASIOGetChannels (inputs: %d, outputs: %d);\n", 
         asioDriverInfo->inputChannels, 
         asioDriverInfo->outputChannels
         );

      // get the usable buffer sizes
      if(ASIOGetBufferSize(&asioDriverInfo->minSize, &asioDriverInfo->maxSize, &asioDriverInfo->preferredSize, &asioDriverInfo->granularity) == ASE_OK)
      {
         printf(
            "ASIOGetBufferSize (min: %d, max: %d, preferred: %d, granularity: %d);\n",
            asioDriverInfo->minSize, asioDriverInfo->maxSize,
            asioDriverInfo->preferredSize, 
            asioDriverInfo->granularity
            );

			// get the currently selected sample rate
        if(ASIOGetSampleRate(&asioDriverInfo->sampleRate) == ASE_OK)
        {
		   printf ("ASIOGetSampleRate (sampleRate: %f);\n", asioDriverInfo->sampleRate);
		   if (asioDriverInfo->sampleRate <= 0.0 || asioDriverInfo->sampleRate > 96000.0)
		   {
			      // Driver does not store it's internal sample rate, 
               // so set it to a know one.
			      // Usually you should check beforehand, 
               // that the selected sample rate is valid
			      // with ASIOCanSampleRate().
		      if(ASIOSetSampleRate(44100.0) == ASE_OK)
			  {
                 if(ASIOGetSampleRate(&asioDriverInfo->sampleRate) == ASE_OK)
			        printf("ASIOGetSampleRate (sampleRate: %f);\n", asioDriverInfo->sampleRate);
			     else
			        return -6;
			  }
			  else
			     return -5;
		   }
            // check wether the driver requires the ASIOOutputReady() optimization
		      // (can be used by the driver to reduce output latency by one block)
		   if(ASIOOutputReady() == ASE_OK)
		      asioDriverInfo->postOutput = true;
		   else
			  asioDriverInfo->postOutput = false;
		   printf("ASIOOutputReady(); - %s\n", asioDriverInfo->postOutput ? "Supported" : "Not supported");
              
		   return 0;
        }
        return -3;
     }
     return -2;
  }
  return -1;
}


//----------------------------------------------------------------------------------
// conversion from 64 bit ASIOSample/ASIOTimeStamp to double float
#if NATIVE_INT64
	#define ASIO64toDouble(a)  (a)
#else
	const double twoRaisedTo32 = 4294967296.;
	#define ASIO64toDouble(a)  ((a).lo + (a).hi * twoRaisedTo32)
#endif

ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow)
{	
	// perform the processing
   // assuming 2 input and 2 output channles, 32 bit int format samples and 44.1KHz 
   // sampling rate
	for (int i = 0; i < asioDriverInfo.inputBuffers + asioDriverInfo.outputBuffers; i++)
	{
		if (asioDriverInfo.bufferInfos[i].isInput == false)
		{
         load_stereo_buffer( 
            (int *)asioDriverInfo.bufferInfos[i].buffers[index], 
            (i%2)
            );
		}
      else
      {
         store_stereo_buffer(
            (int *)asioDriverInfo.bufferInfos[i].buffers[index], 
            (i%2)
            );
		}

	}

	// finally if the driver supports the ASIOOutputReady() optimization, do it here, all data are in place
	if (asioDriverInfo.postOutput)
		ASIOOutputReady();

	return 0L;
}

//----------------------------------------------------------------------------------
void bufferSwitch(long index, ASIOBool processNow)
{	// the actual processing callback.
	// Beware that this is normally in a seperate thread, 
   // hence be sure that you take care about thread synchronization. 
   // This is omitted here for simplicity.

	// as this is a "back door" into the bufferSwitchTimeInfo
   // a timeInfo needs to be created though it will only set the 
   // timeInfo.samplePosition and timeInfo.systemTime fields and 
   // the according flags
	ASIOTime  timeInfo;
	memset (&timeInfo, 0, sizeof (timeInfo));

	// get the time stamp of the buffer, not necessary if no
	// synchronization to other media is required
	if(ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

	bufferSwitchTimeInfo (&timeInfo, index, processNow);
}


//----------------------------------------------------------------------------------
void sampleRateChanged(ASIOSampleRate sRate)
{
	// do whatever you need to do if the sample rate changed
	// usually this only happens during external sync.
	// Audio processing is not stopped by the driver, actual sample rate
	// might not have even changed, maybe only the sample rate status of an
	// AES/EBU or S/PDIF digital input at the audio device.
	// You might have to update time/sample related conversion routines, etc.
}

//----------------------------------------------------------------------------------
long asioMessages(long selector, long value, void* message, double* opt)
{
	// currently the parameters "value", "message" and "opt" are not used.
	long ret = 0;
	switch(selector)
	{
		case kAsioSelectorSupported:
			if(value == kAsioResetRequest
			|| value == kAsioEngineVersion
			|| value == kAsioResyncRequest
			|| value == kAsioLatenciesChanged
			// the following three were added for ASIO 2.0, 
         // you don't necessarily have to support them
			|| value == kAsioSupportsTimeInfo
			|| value == kAsioSupportsTimeCode
			|| value == kAsioSupportsInputMonitor)
				ret = 1L;
			break;
		case kAsioResetRequest:
			// defer the task and perform the reset of the driver 
         // during the next "safe" situation
			// You cannot reset the driver right now, 
         // as this code is called from the driver.
			// Reset the driver is done by completely destruct is. 
         // I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
			// Afterwards you initialize the driver again.
			asioDriverInfo.stopped;  // In this sample the processing will just stop
			ret = 1L;
			break;
		case kAsioResyncRequest:
			// This informs the application, that the driver encountered 
         // some non fatal data loss.
			// It is used for synchronization purposes of different media.
			// Added mainly to work around the Win16Mutex problems 
         // in Windows 95/98 with the Windows Multimedia system, 
         // which could loose data because the Mutex was hold too long
			// by another thread.
			// However a driver can issue it in other situations, too.
			ret = 1L;
			break;
		case kAsioLatenciesChanged:
			// This will inform the host application 
         // that the drivers were latencies changed.
			// Beware, it this does not mean that the buffer sizes have changed!
			// You might need to update internal delay data.
			ret = 1L;
			break;
		case kAsioEngineVersion:
			// return the supported ASIO version of the host application
			// If a host applications does not implement this selector, 
         // ASIO 1.0 is assumed by the driver
			ret = 2L;
			break;
		case kAsioSupportsTimeInfo:
			// informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() 
         // callback is supported.
			// For compatibility with ASIO 1.0 drivers 
         // the host application should always support
			// the "old" bufferSwitch method, too.
			ret = 1;
			break;
		case kAsioSupportsTimeCode:
			// informs the driver wether application is interested 
         // in time code info.
			// If an application does not need to know about time code, 
         // the driver has less work to do.
			ret = 0;
			break;
	}
	return ret;
}


//----------------------------------------------------------------------------------
ASIOError create_asio_buffers (DriverInfo *asioDriverInfo)
{	// create buffers for all inputs and outputs of the card with the 
	// preferredSize from ASIOGetBufferSize() as buffer size
	long i;
	ASIOError result;

	// fill the bufferInfos from the start without a gap
	ASIOBufferInfo *info = asioDriverInfo->bufferInfos;

	// prepare inputs (Though this is not necessaily required, 
   // no opened inputs will work, too
	if (asioDriverInfo->inputChannels > kMaxInputChannels)
		asioDriverInfo->inputBuffers = kMaxInputChannels;
	else
		asioDriverInfo->inputBuffers = asioDriverInfo->inputChannels;
	for(i = 0; i < asioDriverInfo->inputBuffers; i++, info++)
	{
		info->isInput = ASIOTrue;
		info->channelNum = i;
		info->buffers[0] = info->buffers[1] = 0;
	}

	// prepare outputs
	if (asioDriverInfo->outputChannels > kMaxOutputChannels)
		asioDriverInfo->outputBuffers = kMaxOutputChannels;
	else
		asioDriverInfo->outputBuffers = asioDriverInfo->outputChannels;
	for(i = 0; i < asioDriverInfo->outputBuffers; i++, info++)
	{
		info->isInput = ASIOFalse;
		info->channelNum = i;
		info->buffers[0] = info->buffers[1] = 0;
	}

	// create and activate buffers
	result = ASIOCreateBuffers(asioDriverInfo->bufferInfos,
		asioDriverInfo->inputBuffers + asioDriverInfo->outputBuffers,
		AUDIO_BUFFER_SIZE, &asioCallbacks);
	if (result == ASE_OK)
	{
		// now get all the buffer details, sample word length, 
      // name, word clock group and activation
		for (i = 0; i < asioDriverInfo->inputBuffers + asioDriverInfo->outputBuffers; i++)
		{
			asioDriverInfo->channelInfos[i].channel = asioDriverInfo->bufferInfos[i].channelNum;
			asioDriverInfo->channelInfos[i].isInput = asioDriverInfo->bufferInfos[i].isInput;
 			result = ASIOGetChannelInfo(&asioDriverInfo->channelInfos[i]);
			if (result != ASE_OK)
				break;
		}

		if (result == ASE_OK)
		{
			// get the input and output latencies
			// Latencies often are only valid after ASIOCreateBuffers()
			// (input latency is the age of the first sample 
         // in the currently returned audio block)
			// (output latency is the time the first sample 
         // in the currently returned audio block requires to get to the output)
			result = ASIOGetLatencies(&asioDriverInfo->inputLatency, &asioDriverInfo->outputLatency);
			if (result == ASE_OK)
				printf(
               "ASIOGetLatencies (input: %d, output: %d);\n", 
               asioDriverInfo->inputLatency, 
               asioDriverInfo->outputLatency
               );
		}
	}
	return result;
}

unsigned long get_sys_reference_time()
{	// get the system reference time
   return timeGetTime();
}
 

/** 4. This code now needs to load one or more of a few different sound sources
in the system such as tanpura, tabla, guitar and live tracks in a mutli-tracking 
setup. We need a descriptor for the current mix to be played back and this function
needs to read that and playback the desired tracks from the desired locations.

*/     
void load_stereo_buffer( int *pnBuffer, int nLeftOrRight )
{
   static int nBufferIndex = 0;
   // load for one of five different states:
   int nOffset = nBufferIndex * AUDIO_BUFFER_SIZE;
   switch( g_uMixerState )
   {
   case MIXER_STATE_a: // load a 275Hz drone into both channels
      load_tone( pnBuffer, nLeftOrRight );
      break;
   case MIXER_STATE_b: // tanpura
      load_tanpura( pnBuffer, nLeftOrRight );
      break;
   case MIXER_STATE_c: // tanpura and tabla
      load_tanpura_tabla( pnBuffer, nLeftOrRight );
      break;
   case MIXER_STATE_d: // tanpura, tabla, guitar
      load_tanpura_tabla_guitar( pnBuffer, nLeftOrRight );
      break;
   case MIXER_STATE_e: // 3 synth tracks + live track
      load_all_tracks( pnBuffer, nLeftOrRight );
      break;
   default:
      // toto handle error
      break;
   }
   return;
}

// load a sinewave at 275Hz into both left and right channels
void load_tone( int *pnBuffer, int nLeftOrRight )
{
   static int nBufferIndex = 0;
   // load for one of five different states:
   int nOffset = nBufferIndex * AUDIO_BUFFER_SIZE;
   for( int i=0; i<AUDIO_BUFFER_SIZE; i++ )
   {
      pnBuffer[i] = (int) (TwoPow31Minus1 * sin( TwoPi * ((i+nOffset)*275.0)/44100.0));
   }
   if( nLeftOrRight == 1 )
   {
   	nBufferIndex++;
   }
}

void load_tanpura( int *pnBuffer, int nLeftOrRight )
{
   // load the next buffer from the tanpura looper to pnBuffer
   load_tanpura_buffer( pnBuffer, nLeftOrRight );
}

void load_tanpura_tabla( int *pnBuffer, int nLeftOrRight )
{
   // load the next buffer from the tanpura looper to g_pnBus[0]
   load_tanpura_buffer( g_pnBus[0], nLeftOrRight );
   // load the next buffer from the tabla looper to g_pnBus[1]
   load_tabla_buffer( g_pnBus[1], nLeftOrRight );
   // mixdown to pnBuffer
   for( int i=0; i<AUDIO_BUFFER_SIZE; i++ )
   {
      pnBuffer[i] = (g_pnBus[0][i] + g_pnBus[1][i])>>1;
   }
}

void load_tanpura_tabla_guitar( int *pnBuffer, int nLeftOrRight )
{
   // load the next buffer from the tanpura looper to g_pnBus[0]
   load_tanpura_buffer( g_pnBus[0], nLeftOrRight  );
   // load the next buffer from the tabla looper to g_pnBus[1]
   load_tabla_buffer( g_pnBus[1], nLeftOrRight );
   // load the next buffer from the guitar looper to g_pnBus[2]
   load_guitar_buffer( g_pnBus[2], nLeftOrRight );
   // mixdown to pnBuffer
   for( int i=0; i<AUDIO_BUFFER_SIZE; i++ )
   {
      pnBuffer[i] = ((g_pnBus[0] + g_pnBus[1])>>1 + g_pnBus[2])>>1;
   }
}

void load_all_tracks( int *pnBuffer, int nLeftOrRight )
{
   // load the next buffer from the tanpura looper to g_pnBus[0]
   load_tanpura_buffer( g_pnBus[0], nLeftOrRight );
   // load the next buffer from the tabla looper to g_pnBus[1]
   load_tabla_buffer( g_pnBus[1], nLeftOrRight );
   // load the next buffer from the guitar looper to g_pnBus[2]
   load_guitar_buffer( g_pnBus[2], nLeftOrRight );
   // load the next buffer from the live mix to g_pnBus[3]
   load_live_buffer( g_pnBus[3], nLeftOrRight );
   // mixdown to pnBuffer
   for( int i=0; i<AUDIO_BUFFER_SIZE; i++ )
   {
      pnBuffer[i] = (((g_pnBus[0] + g_pnBus[1])>>1 + g_pnBus[2])>>1 + g_pbBus[3])>>1;
   }
}

void  load_tanpura_buffer( int * pnBuffer, int nLeftOrRight )
{
   // keep track of which buffer you are processing for the timing sync.
   // static init the current buffer to 0
   // load the next set of samples for the buffer.
   //    if the samples are in fader range, perform the fade and load the result
   //    else just load the samples from the current tanpura file.
   // if it's the right buffer, update the current buffer index
}

void  load_tabla_buffer( int * pnBuffer, int nLeftOrRight )
{

}

void  load_guitar_buffer( int * pnBuffer, int nLeftOrRight )
{

}

void  load_live_buffer( int * pnBuffer, int nLeftOrRight )
{
   // For all the live tracks in the current mix,
   //    based on the current buffer being processed
   //    determine, which track are in the mix.
   //    Mix down those tracks into pnBuffer
}

/** 5. Similarly, this code will need to read some information about how the
current sound inputs need to be processed and vector out to those functions
instead of this current hard-coded storage function.
*/
void store_stereo_buffer( int *pnBuffer, int nLeftOrRight )
{
   fwrite( pnBuffer, sizeof(int), AUDIO_BUFFER_SIZE, g_pFileLive );
   return;
}

/** 6. Let's analyze, based on all our app requirements for the final 
release what kind of sound-stream multiplexing needs to be done at both ends.
On the playback side, the source mix is either
a. A tuning drone
b. A tanpura
c. A tabla and a tanpura
d. A guitar, a tabla and a tanpura
e. A live track with the three looped tracks.
On the input side, the destination is always to a stereo file. Later, as we start 
analysing the signals, the destination will become a sound characterizer. So we 
can design for these two types of live sound destinations in our multiplexer design.

  In the current app, the driver's preferred size for our driver is 256. This has 
  been hardcoded into both load and store. Let's change the buffer size to
  1024 and exit the app if that's not supported, since we don't want more than
  44 display updates a second.
*/




