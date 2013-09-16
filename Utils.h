#pragma  once 
#include "MathUtils.h"

inline void memcpyquad(void *dest,const void * src,int nums=4)
{
	_asm
	{
		cld
			mov esi,src
			mov edi,dest
			mov ecx,nums
			rep movsd
	}
}

inline void memzeroquad(void *dest,int nums)
{
	_asm
	{
		    cld
			mov edi,dest 
			mov ecx,nums
			mov eax,0
			rep stosd
	}
}

//void  memcpyquad(void *dest,const void *src,int nums=4);
//void  memzeroquad(void *dest,int nums);
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


