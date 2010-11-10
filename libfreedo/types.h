/*****************************************************************************
Copyright (C) 2007-2008 Гришин Максим Леонидович (http://altmer.arts-union.ru)
*****************************************************************************/

#ifndef TYPES_HEAD_DEFINITION
#define TYPES_HEAD_DEFINITION

typedef  unsigned int uint32;
typedef  unsigned char uint8;
typedef  unsigned short uint16;
typedef  unsigned long long uint64;
typedef  unsigned long long __uint64;

typedef  int int32;
typedef  char int8;
typedef  short int16;
typedef  signed long long int64;
typedef  signed long long __int64;

typedef  float real32;
typedef  double real64;
typedef  long double real80;

#ifndef NULL
 #define NULL 0
#endif

#ifdef __GNUC__
 #define __fastcall __attribute__((__fastcall__))
#endif
//------------------Векторные типы-------------------------------------------

#pragma pack(push,1)
template <class T> struct point2d { T x,y; };
template <class T> struct sprite{ point2d<T> ul,dr; };
#pragma pack(pop)
//---------------------------------------------------------------------------
//--------------------Ресурсы------------------------------------------------
#pragma pack(push,1)
struct imgdata
{
#define IMDT_BMP32      0
#define IMDT_BMP24      1
        uint32 pref;    //0xf - type,....
        uint32 w,h,d;
        uint32 data[1];
};
#pragma pack(pop)
//----------------------------------------------------------------------------

#endif
