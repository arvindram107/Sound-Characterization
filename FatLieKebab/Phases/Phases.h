extern int (** g_ppfnRespond)(
	      UINT message,
	      WPARAM wParam,
	      LPARAM lParam,
		  long lTime
	             );
extern int (** g_ppfnShow)( HDC hDC );

extern int load_phases();
extern int unload_phases();

extern unsigned int g_uCurrentPhase;
extern unsigned int g_uNumTrials;


