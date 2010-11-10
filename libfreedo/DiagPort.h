// DiagPort.h: interface for the DiagPort class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DIAG_PORT_HEADER_DEFINITION_
#define _DIAG_PORT_HEADER_DEFINITION_

	void _diag_Init(int testcode);

	unsigned int __fastcall _diag_Get();
	void __fastcall _diag_Send(unsigned int val);


#endif 
