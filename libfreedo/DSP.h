// DSP.h: interface for the CDSP class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DSP_3DO_HEADER
#define DSP_3DO_HEADER


	unsigned int _dsp_Loop();

	unsigned short __fastcall _dsp_ReadIMem(unsigned short addr);
	void __fastcall _dsp_WriteIMem(unsigned short addr, unsigned short val);
	void __fastcall _dsp_WriteMemory(unsigned short addr,unsigned short val);
	void __fastcall _dsp_SetRunning(bool val);
	void __fastcall _dsp_ARMwrite2sema4(unsigned int val);
	unsigned int _dsp_ARMread2sema4(void);

	void _dsp_Init();
	void _dsp_Reset();
	

        unsigned int _dsp_SaveSize();
        void _dsp_Save(void *buff);
        void _dsp_Load(void *buff);


#endif 