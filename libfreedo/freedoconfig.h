#ifndef FREEDOCONFIG_H
#define FREEDOCONFIG_H

#ifdef _WIN32
#include <stdafx.h>
#include <windows.h>
#else
#include <stdlib.h>
#include <stdio.h>


extern bool __temporalfixes;
extern int HightResMode;
#define RESSCALE        HightResMode
#define DEBUG_CORE
#define _T(a) (a)

#endif

#include "types.h"


#endif // FREEDOCONFIG_H
