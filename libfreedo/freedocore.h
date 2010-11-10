#ifndef __3DO_SYSTEM_HEADER_DEFINITION
#define __3DO_SYSTEM_HEADER_DEFINITION

//------------------------------------------------------------------------------
#pragma pack(push,1)

struct VDLLine
//VDLP Line - ����� ��������� �� ���� ������������ ������� �������� ������� VDLP
{
        unsigned short line[320*4];//,line2[320*2*16];
        unsigned char xCLUTB[32];
	unsigned char xCLUTG[32];
	unsigned char xCLUTR[32];
        unsigned int xOUTCONTROLL;
        unsigned int xCLUTDMA;
	unsigned int xBACKGROUND;
};
struct VDLFrame
{
        VDLLine lines[240*4];
        unsigned int srcw,srch;
};

#pragma pack(pop)

#define EXT_READ_ROMS           1
#define EXT_READ_NVRAM          2
#define EXT_WRITE_NVRAM         3
#define EXT_SWAPFRAME           5       //������������ ������ ��� ������ �����, ���� � ����� ������
#define EXT_PUSH_SAMPLE         6       //�������� ������ �� �������� �����
#define EXT_GET_PBUSLEN         7
#define EXT_GETP_PBUSDATA       8
#define EXT_KPRINT              9
#define EXT_DEBUG_PRINT         10
#define EXT_FRAMETRIGGER_MT     12      //multytasking
#define EXT_READ2048            14      //for XBUS Plugin
#define EXT_GET_DISC_SIZE       15
#define EXT_ON_SECTOR           16
typedef void* (*_ext_Interface)(int, void*);

#define FDP_FREEDOCORE_VERSION  0
#define FDP_INIT                1    //set ext_interface
#define FDP_DESTROY             2
#define FDP_DO_EXECFRAME        3       //��������� 1/60 ������� ������ �������
#define FDP_DO_FRAME_MT         4      //multytasking
#define FDP_DO_EXECFRAME_MT     5      //multytasking
#define FDP_DO_LOAD             6       //��������� �� ����������� ������ ��������� ���������, ������� !NULL ���� ��� ��
#define FDP_GET_SAVE_SIZE       7       //return size of savestatemachine
#define FDP_DO_SAVE             8       //�������� � ���������� ����� ��������� ���������
#define FDP_GETP_NVRAM          9       //���������� ��������� �� NVRAM 32K
#define FDP_GETP_RAMS           10       //���������� ��������� �� RAM 3M
#define FDP_GETP_ROMS           11       //���������� ��������� �� ROM 2M
#define FDP_GETP_PROFILE        12       //���������� ��������� �� ������ �������� - ������ 3�/4 ���������
#define FDP_BUGTEMPORALFIX      13
#define FDP_SET_ARMCLOCK        14
#define FDP_SET_TEXQUALITY      15
#define FDP_GETP_WRCOUNT        16

#ifdef _WIN32

#ifdef FREEDOCORE_EXPORTS
#define FREEDOCORE_API __declspec(dllexport)
#else
#define FREEDOCORE_API __declspec(dllimport)
#endif


#else

#define FREEDOCORE_API

#endif

extern "C"
{
        FREEDOCORE_API void* _freedo_Interface(int procedure, void *datum=0);
};

#ifdef _WIN32
#ifndef FREEDOCORE_EXPORTS
#pragma comment(lib, "freedocore.lib")
#endif
#endif
//------------------------------------------------------------------------------


#endif
