#pragma  once

#include<windows.h>
#include <crtdbg.h>//for ASSERT macro
#include <math.h>
#include <windowsx.h>

#define ASSERT(X) _ASSERT((X))
#define MAX(a,b)?((a)>(b)?(a):(b))


#define RGB32(r,g,b) (((r&0xff) << 16)|((g&0xff)<<8)|(b&0xff))
//typedef unsigned char UCHAR;