#pragma once
extern  void phase2_load( 
        int (** ppfnRespond)(
	               UINT message,
	               WPARAM wParam,
	               LPARAM lParam,
				   long lTime
	                      )
		        );
void phase2_unload();
extern  int  phase2_respond(
	    UINT message,
	    WPARAM wParam,
	    LPARAM lParam,
		long lTime
		    );
extern  int  phase2_show(
	    HDC hDC
		    );

