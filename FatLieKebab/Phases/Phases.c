#include "../StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include "../Inck.h"
#include "../Resource.h"
#include "Phases.h"
#include "Phase1.h"
#include "Phase2.h"
#include "Phase3.h"

/** 7.
Let's take stock of the entire Fat Lie Kebab production app and add the phases 
after the third to complete the entire apps design as a whole. The third phase gets 
us our tanpura track. Then we get the tabla track (4), guitar (5) and 
finally multi-tracking of the live tracks (6). Let's flesh out 4,5,6 and then 
redesign all the state-machine states in this file and the indivual phase files. 
4 and 5 can be more or less based on 3 since they mostly involve selecting a segment
to loop. 6 can be broken into three parts, creating the punch line, creating the 
narrative and putting it all together. Let's make each of these separate phases, 
so now we have 6,7 and 8 for a total of eight phases. That's all the info we need 
to create this file. The steps and substeps in each of the phases we will describe 
in the respective file for the phase. 

Note: Later, as we build tools to automate this process, we should have an easy way
to navigate to any state in the track.
*/


unsigned int g_uNumPhases = 3;
unsigned int g_uCurrentPhase;

int (** g_ppfnRespond)(
	      UINT message,
	      WPARAM wParam,
	      LPARAM lParam,
		  long lTime
	                   );

int (** g_ppfnShow)(
         HDC hDC
                   );

int load_phases()
{
   g_uCurrentPhase = 0; 

   g_ppfnRespond = (int (**)(UINT, WPARAM, LPARAM, long)) 
	          malloc( 
			     g_uNumPhases *
				 sizeof( int (*)(UINT, WPARAM, LPARAM, long) )
				    );
   g_ppfnShow = (int (**)(HDC)) 
	          malloc( 
			     g_uNumPhases *
				 sizeof( int (*)(HDC) )
   phase1_load( &(g_ppfnRespond[0]), &(g_ppfnShow[0]) );
   phase2_load( &(g_ppfnRespond[1]), &(g_ppfnShow[1]) );
   phase3_load( &(g_ppfnRespond[2]), &(g_ppfnShow[2]) );

   return 0;
}

int unload_phases()
{
   phase1_unload();
   phase2_unload();
   phase3_unload();
   free( g_ppfnRespond );
   free( g_ppfnShow );
   return( 0 );
}
