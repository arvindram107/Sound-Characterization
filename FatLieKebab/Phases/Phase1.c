#include "../StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include "Phase1.h"
#include "../Screen/Screen.h"
#include "Phases.h"

/** 8. This is a simple phase with a single step, so we just implement that step
through the respond and show functions.
*/

void phase1_load( 
        int (** ppfnRespond)(
	               UINT message,
	               WPARAM wParam,
	               LPARAM lParam,
				   long lTime
	                      ),
        int (** ppfnShow)( HDC hDC )
		        )
{
   *ppfnRespond = phase1_respond; 
   return;
}

void phase1_unload()
{
   return;
}

int phase1_respond(
	    UINT message,
	    WPARAM wParam,
	    LPARAM lParam,
		long lTime
		    )
{
   HDC hDC;
   // implement the keyboard behavior here
   // on Space Bar, stop the tone (which we'll do by stopping ASIO)
   // transition to phase 2
   if( message == WM_KEYDOWN && wParam == VK_SPACE )
   {
      ASIOStop();
      g_uCurrentPhase = 2;
      // show the new phase
      hDC = GetDC( g_hWnd );
      g_ppfnShow( hDC );
   }
}

int phase1_show(
	    HDC hDC
               )
{
   // display the state here
   // clear the display port
   HBRUSH hBrush = CreateSolidBrush( RGB( g_uMidGray, g_uMidGray, g_uMidGray ) );
   // first draw a grey rectanlge that covers the entire port
   SelectObject( hDC, hBrush );
   Rectangle(
	   hDC, 
	   g_rectPort.left, 
	   g_rectPort.top,
	   g_rectPort.right,
	   g_rectPort.bottom
	        );

   // display the string "playing 275Hz tone to recording room"
   TCHAR achtung[] = _T("playing 275Hz tone to recording room");
         TextOut(
	        hdc,
           g_rectPort.left + (g_uPortWidth/2) - (g_uFontWidth * _tcslen(achtung))/2, 
		     g_rectPort.top + (g_uPortHeight) - g_uFontHeight/2,
            achtung, 
		    _tcslen(achtung)
		        );
}


