// SPORT.h: interface for the SPORT class.
//
//////////////////////////////////////////////////////////////////////

#ifndef	SPORT_3DO_HEADER
#define SPORT_3DO_HEADER


	void _sport_Init(unsigned char *vmem);

	 //take source for SPORT
	int __fastcall _sport_SetSource(unsigned int index);

	void __fastcall _sport_WriteAccess(unsigned int index, unsigned int mask);

unsigned int _sport_SaveSize();
void _sport_Save(void *buff);
void _sport_Load(void *buff);

#endif 
