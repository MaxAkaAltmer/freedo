// VDLP.h: interface for the CVDLP class.
//
//////////////////////////////////////////////////////////////////////

#ifndef	VDLP_3DO_HEADER
#define VDLP_3DO_HEADER

#include "freedocore.h"

	void _vdl_Init(unsigned char *vramstart);
        void _vdl_ProcessVDL( unsigned int addr);

        void _vdl_DoLineNew(int line, VDLFrame *frame);

        unsigned int _vdl_SaveSize();
        void _vdl_Save(void *buff);
        void _vdl_Load(void *buff);

#endif 
