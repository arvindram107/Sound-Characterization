#include "../StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include "Phase2.h"
#include "../Screen/Screen.h"
#include "Phases.h"

/** 9. This is also a simple phase with a single step
*/

void phase2_load( 
        int (** ppfnRespond)(
	               UINT message,
	               WPARAM wParam,
	               LPARAM lParam,
				   long lTime
	                      ),
        int (** ppfnShow)( HDC hDC )
		        )
{
   *ppfnRespond = phase2_respond; 
   *ppfnShow = phase2_show;
   // initialize the file to which recording will be done
   // name of file: T(an)P(ura)_G(ents)_K(ali)1_YYYYMMDD_<Time>
   // where <Time> is time-stamped at the time the recording starts
   // in the format hhhhmmss
   return;
}

void phase2_unload()
{
   return;
}

int phase2_respond(
	    UINT message,
	    WPARAM wParam,
	    LPARAM lParam,
		long lTime
		    )
{
   static bool bRecording = false;
   // implement the keyboard behavior here
   // on space bar, start recording the tanpura track
   
}

int phase2_show(
	    HDC hDC
               )
{
   // display the state here
}

