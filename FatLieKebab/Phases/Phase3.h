extern int (*** g_pppfnRespondP3)(
	               UINT message,
	               WPARAM wParam,
	               LPARAM lParam,
	               long lTime     
				          );
extern int (*** g_pppfnShowP3)(
	               HDC hDC     
				          );
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
		        );
void phase3_unload();

void p3load_steps();

void phase3_init();

int phase3_respond(
	   UINT message,
	   WPARAM wParam,
	   LPARAM lParam,
	   long lTime
	          );
int phase3_show(
	   HDC hDC
	          );

int respond7ai( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int respond7aii( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int respond7aiii( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int respond7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int respond7bii( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int respond7biii( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int respond7c( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int show7ai( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int show7aii( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int show7aiii( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int show7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int show7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int show7bi( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
int show7c( UINT message, WPARAM wParam, LPARAM lParam, long lTime );
