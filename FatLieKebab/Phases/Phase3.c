/** 10. Let's fill in the steps and substeps of this phase and give the final 
leaf respond functions names. Here they are repeated:
7a: (i), (ii) and (iii),
7b: (i), (ii) and (iii),
7c: Just one
So 3,3, and 1 substeps in 3 steps.
*/
#include "../StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include "Phase3.h"
#include "../Track/Track.h"
#include "../Screen/Screen.h"
#include "Phases.h"


unsigned int g_uNumSteps = 3;
unsigned int g_uPhase3Step=0;
unsigned int g_uPhase3SubStep = 0;
int (*** g_pppfnRespondP3)(
	               UINT message,
	               WPARAM wParam,
	               LPARAM lParam,
	               long lTime     
				          );
int (*** g_pppfnShowP3)(
	               HDC hDC     
				          );


extern HWND g_hWnd;

void phase3_load( 
        int (** ppfnRespond)(
	               UINT message,
	               WPARAM wParam,
	               LPARAM lParam,
				   long lTime
	                      ),
        int (** ppfnShow)(
	               HDC hDC
	                      )

		        )
{
   g_pppfnRespondP3 = (int (***)(UINT, WPARAM, LPARAM, long)) 
	          malloc( 
			     g_uNumSteps * sizeof( int (**)(UINT, WPARAM, LPARAM, long) )
				    );
   g_pppfnShowP3 = (int (***)(HDC)) 
	          malloc( 
			     g_uNumSteps * sizeof( int (**)(HDC) )
				    );
   p3load_steps();
   *ppfnRespond = phase3_respond;
   *ppfnShow = phase3_show;
   return;
}

void phase3_unload()
{

	return;
}

void p3load_steps()
{
   g_pppfnRespondP3[0] = (int (**)(UINT, WPARAM, LPARAM, long)) 
	          malloc( 
			     g_uNumSteps * sizeof( int (*)(UINT, WPARAM, LPARAM, long) )
				    );
   g_pppfnShowP3[0] = (int (**)(HDC)) 
	          malloc( 
			     g_uNumSteps * sizeof( int (*)(UINT, WPARAM, LPARAM, long) )
				    );
   g_pppfnRespondP3[0][0] = respond7ai;   
   g_pppfnShowP3[0][0] = show7ai;   
   g_pppfnRespondP3[0][1] = respond7aii;   
   g_pppfnShowP3[0][1] = show7aii;   
   g_pppfnRespondP3[0][2] = respond7aiii;   
   g_pppfnShowP3[0][2] = show17aiii;   

   g_pppfnRespondP3[1] = (int (**)(UINT, WPARAM, LPARAM, long)) 
	          malloc( 
			     g_uStep1Substeps * sizeof( int (*)(UINT, WPARAM, LPARAM, long) )
				    );
   g_pppfnShowP3[1] = (int (**)(HDC)) 
	          malloc( 
			     g_uStep1Substeps * sizeof( int (*)(HDC) )
				    );
   g_pppfnRespondP3[1][0] = respond7bi;  
   g_pppfShowP3[1][0] = show7bi;  
   g_pppfnRespondP3[1][1] = respond7bii;  
   g_pppfnShowP3[1][1] = show7bii;  
   g_pppfnRespondP3[1][2] = respond7biii;  
   g_pppfnShowP3[1][2] = show7biii;  

   g_pppfnRespondP3[2] = (int (**)(UINT, WPARAM, LPARAM, long)) 
	          malloc( 
			     1 * sizeof( int (*)(UINT, WPARAM, LPARAM, long) )
				    );
   g_pppfnShowP3[2] = (int (**)(HDC)) 
	          malloc( 
			     1 * sizeof( int (*)(HDC) )
				    );
   g_pppfnRespondP3[2][0] = respond7c;   
   g_pppfnShowP3[2][0] = show7c;   


   return;
}

void phase3_init()
{ 
   return;
}

void phase3_close()
{
	return;
}

int phase3_respond( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return( g_pppfnRespondP3[g_uPhase3Step][g_uPhase3SubStep]( message, wParam, lParam, lTime ) );
}   

int phase3_show( HDC hDC )
{
   return( g_pppfnShowP3[g_uPhase3Step][g_uPhase3SubStep]( hDC ) );
}   

/** 11. 7a: Now that we're actually going to do it, we are a bit clearer on how. The
process will be as follows:
App loads the tanpura recording file and shows a graph of the signal centered
in a sup-port inside the port, with a border left for messages.
*/
int show7ai( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   
/*
On space bar, the recording is played out
*/
int respond7ai( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
} 
/*  
 and the graph starts scrolling to the left in sync with the signal. 
 (This is done by the drawing code in the buffer switch callback)
By convention, the currently played sample is displayed
at the center of the track display. Later, we'll calibrate the display with the 
audio output.
The next thing to be done is to calibrate the space bar press with the audio stream 
being played back. 
*/
/*
(Nothing is shown in this substep) 
*/
int show7aii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   
/*
To do this, on space bar press, the app notes the current time 
and subtracts from it the time of the last buffer switch to determine the offset
into the buffer. From this, and the current buffer index and the known buffer size,
the offset in the file of the space bar press is recorded. 

The first time around, the user would not press space bar since they wouldn't
know when to press having not previewed the signal. They preview the signal
and then ask for a replay by pressing the 'H' key.
*/
int respond7ai( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   // on space bar
   // on 'H'
   return 0;
} 
/*
That sample location is
now displayed in the track sub-port. 
*/
int show7aiii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   
/*
The user will press the space bar at the exact perceived time of 
the base Saa (the start of the track). By looking at the signal and the entered location of this event, 
the user now describes any obvious latency in the system. 
Since pressing the space bar in sync with an audio event in a consistent way 
requires some practice, the app will 
provide a way (pressing the 'H' key) for the user to re-do the space bar tapping 
several times, till they're satisfied with the perceived synchronicity of their tap.
At this point, they will need a way to adjust the track location they have input 
till it's visually matched up (center strip). 
Once this is done, the app will display the end of the track to enable
the user to specify the location of the start (the base Saa) of the last aavartan.
Then of the one before that. The app will then play back in a loop the last two 
aavartans and the first two followed by some silence. This will enable the user to
test the quality of the fade. The user may repeat the process from some point to 
tune the parameters till they're satisfied. At this point, the tanpura synth is 
armed with all the info it needs to produce its output. The app now enables the 
user to test the synth. 
11. */

/** 12. In the interests of brevity, we've decided to skip the looping altogether
and go straight to a multi-tracking recorder. Since our track is short, we 
should be able to produce everything live. Later, as we develop better analysis 
tools, we should be able to automatically extract loops.
*/

/** 13. Let's describe the protocol with an awareness of the implementation
arch underneath so that the mapping from the protocol description (which currently
we are using English sentences for) to the state machine is easier. In other 
words, the language used to describe the protocol should easily map to the code. In 
11, we first wrote a protocol as an English paragraph, then we spliced into it 
show and respond functions based on the activity described. Now let's take this
a step further and analyze the content of the protocol description to come up with
a much simpler parametric descriptor.

At this point, the protocol will only contain elements to describe what the app
needs to do to run the studio. However, to design it's descriptor, we will factor
in what future versions of the protocol, as far as we can predict, will do. The 
next area of development will be for the app to start helping in the coding of 
itself. This work will be done as part of the Retro-Coding project. After that, 
we will add the ability for it to help design an FPGA to realize its function.
In parallel, we will teach it to help us build custom ASICs to realize it and to 
design the hardware system that will realize it in a gadget. Finally, the industrial 
design parts. At some point this app will enable us to design a gadget that has well
developed media interfaces specially well tuned for use by musicians. 

1. Studio
Playing a tuning tone
Recording the tanpura track
Recording the tabla track
Recording the guitar track
Syncing the guitar track with the tabla track
Recording the live track
Sync
Playing back the final mix
Exporting to .wav
Creating a fly-through track (Next Milestone, M2)
Exporting to some video format for YouTube (M2)
Characterizing the tanpura (M3)
Playing the tanpura synth (M3)
Characterizing the tabla (M4)
Playing the tabla synth (M4)
Characterizing the Guild (M5)
Characterizing voice (M6)
Encoding into a transmission format (M7)
 
2. Programming
Organizing all the functions in a C program in a better manner than the flat
file based organization that is currently used. Basically providing a better 
interface to editing C code that outputs to a single C file that has all the code 
in it, and using the current IDE (Visual C++ 6.0) as a build and debug
environment. That interface would have some navigational elements designed for
the keyboard-screen and the familiar text editing environment of Notepad for 
editing code. The goal is a to map this software based organization 
into a spatial and funtional based organization for a gate network layout. 
Basically something that we can easily translate into formats for popular FPGA tools
and chip design tools.

3. FPGA
Some interface to FPGA design formats?

4. ASIC
Some interface to VHDL?

5. Board + peripherals
??
6. Industrial Design
??

While currently, the interface takes input from the keyboard, in the future, it will
also include speech input. Our protocol descriptior should comprehend the speech 
case as well.

Note: This approach to design where we factor in future use in current design is 
along the line of thinking of the software product line approach to software product
design popularized by CMU.

If we analyze the data we've generated, we conclude that on the keyboard input side
of the descriptor, we will have to come up with some organization of the different 
ways the keys of a keyboard can be mapped for different purposes in addition to the 
usual typewriter mapping. On the output side, we need to map the zones of the 
keyboard into different areas of the screen for easy association between the 
keyboard and the screen which is needed sometimes. A common protocol element is a 
user message that should go to a known area of the screen for consistency in the UI.

Let's now make our first design of the descriptor for the task at hand.
These are the tasks the app will perform:
1Playing a tuning tone
2Recording the tanpura track
3Recording the tabla track
4Recording the live track
5Syncing the live track with the tabla track
6Playing back the final mix
Let's also describe the user's expected activity during the session to make it
a 2 way protocol giving some knowledge to the app about the user for future
elements in the protocol based on an understanding of what the user is doing
and not just through the phases in the protocol that the user has explicitly 
called out.

*/

int show7ai( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int respond7aii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int show7aii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int respond7aiii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int show7aiii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int respond7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int respond7bii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int respond7biii( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int respond7c( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int show7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int show7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int show7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   

int show7c( UINT message, WPARAM wParam, LPARAM lParam, long lTime )
{
   return 0;
}   
