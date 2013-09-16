#pragma  once 
#include "Globals.h"

typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	PALETTEENTRY palette[256];
	UCHAR *buffer;
	void GetPixel(int x,int y,BYTE &r,BYTE &g,BYTE &b);
}BITMAP_FILE,*BITMAP_FILE_PTR;

bool LoadBitmapFile(BITMAP_FILE_PTR bitmap,const char* filename);
bool UnLoadBitMap(BITMAP_FILE_PTR bitmap);

inline void BITMAP_FILE::GetPixel(int x,int y,BYTE &r,BYTE &g,BYTE &b)
{
	if(bitmapinfoheader.biBitCount == 24)
	{
		int base = 3*x+y*bitmapinfoheader.biWidth*3;
		r = buffer[base+2];
		g = buffer[base+1];
		b = buffer[base];
	}
}