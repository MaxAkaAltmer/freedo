#ifndef QUARZ_3DO_HEADER_DEFINTION
#define QUARZ_3DO_HEADER_DEFINTION

void __fastcall _qrz_Init();

int __fastcall _qrz_VDCurrLine();
int __fastcall _qrz_VDHalfFrame();
int __fastcall _qrz_VDCurrOverline();

bool __fastcall _qrz_QueueVDL();
bool __fastcall _qrz_QueueDSP();
bool __fastcall _qrz_QueueTimer();

void __fastcall _qrz_PushARMCycles(unsigned int clks);

unsigned int _qrz_SaveSize();
void _qrz_Save(void *buff);
void _qrz_Load(void *buff);

#endif
 