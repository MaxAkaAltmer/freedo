// CPU.h: interface for the CCPU class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ARM_3DO_HEADER
#define ARM_3DO_HEADER



	int __fastcall _arm_Execute();
	void _arm_Reset();
	void _arm_Destroy();
	unsigned char * _arm_Init();

	//for mas
	void __fastcall _mem_write8(unsigned int addr, unsigned char val);
        void __fastcall _mem_write16(unsigned int addr, unsigned short val);
        void __fastcall _mem_write32(unsigned int addr, unsigned int val);
        unsigned char __fastcall _mem_read8(unsigned int addr);
        unsigned short __fastcall _mem_read16(unsigned int addr);
        unsigned int __fastcall _mem_read32(unsigned int addr);

	void __fastcall WriteIO(unsigned int addr, unsigned int val);
	unsigned int __fastcall ReadIO(unsigned int addr);
	void __fastcall SelectROM(int n);

        unsigned int _arm_SaveSize();
        void _arm_Save(void *buff);
        void _arm_Load(void *buff);


#endif 
