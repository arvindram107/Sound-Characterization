#ifndef _Phase1
#define _Phase1
// TODO make this a static instead of global

extern  void phase1_load( 
        int (** ppfnRespond)(
	               UINT message,
	               WPARAM wParam,
	               LPARAM lParam,
				   long lTime
	                      )
		        );
void phase1_unload();
extern  int  phase1_respond(
	    UINT message,
	    WPARAM wParam,
	    LPARAM lParam,
		long lTime
		    );
extern  int  phase1_show(
	    HDC hDC
		    );

#endif
