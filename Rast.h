#pragma  once 
#include "Globals.h"
#include <stdio.h>
class CSurface
{
public:
	enum ePixelFormat
	{
		kRGBINDEX8,
		kRGB5_6_5,
		kRGB8_8_8,
	};
	ePixelFormat        pixel_format;
	int                 pitch;
	int                 colorbits;
	HDC                 dst_hDC;
	HDC                 dib_hDC;
	BITMAPINFO          bmi;
	HBITMAP             hbmDIB;
	HBITMAP             hOldBitmap;
	PBYTE               pbPixels;
	int                 width;
	int                 height;

	DWORD           backgroundclr;
	int             buffersize;//how many dwords
	CSurface():hbmDIB(NULL){ }
	void Create(HDC hdc,int nWidth,int nHeight);
	void Destroy();
	void Draw();
	void SetPixel(int x,int y,COLORREF color);
	void SetPixel(int x,int y,BYTE r,BYTE g,BYTE b);
	void SetClearColor(COLORREF color);
	void Clear();
	~CSurface()
	{
		Destroy();
	}


};

inline void CSurface::Create(HDC hdc,int nWidth,int nHeight)
{
	dst_hDC = hdc;
	width = nWidth;
	height = nHeight;

	LPBITMAPINFO pbmi = &bmi;
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = nWidth;
	pbmi->bmiHeader.biHeight= -nHeight;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = GetDeviceCaps(hdc, BITSPIXEL);
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = 0;
	pbmi->bmiHeader.biXPelsPerMeter = 0;
	pbmi->bmiHeader.biYPelsPerMeter = 0;
	pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biClrImportant = 0;

	colorbits = pbmi->bmiHeader.biBitCount;
	pitch = (nWidth * (colorbits >>3) + 3) & ~3;

	HDC hic = CreateIC("display", NULL, NULL, NULL);
	dib_hDC = CreateCompatibleDC(hic);
	hbmDIB = CreateDIBSection(hic,pbmi,DIB_RGB_COLORS,(void**)&pbPixels,0,0);
	hOldBitmap = (HBITMAP)SelectObject(dib_hDC, hbmDIB);
	DeleteDC(hic);

	switch(colorbits)
	{
	case 8:
		pixel_format  = kRGBINDEX8;
		break;
	case 16:
		pixel_format = kRGB5_6_5;
		break;
	case 32:
		pixel_format = kRGB8_8_8;
		break;
	default:
		ASSERT(0);
		break;
	}

	
     buffersize = ((height *pitch) >> 2);
	
}


inline void CSurface::Clear()
{
	////set the memory dc's color to the settings color

	//memset(pbPixels,clr,buffersize);
	LPDWORD pbuffer = (LPDWORD)pbPixels;
    for(int i = 0; i < buffersize;++i)
	{
		*pbuffer = backgroundclr;
		pbuffer++;
	}
	
	/* _asm
	{
		cld 
		mov  eax,backgroundclr
		mov  edi,pbPixels
		mov  ecx,buffersize
		rep  stosd
	}*/
}
inline void CSurface:: Destroy()
{
	if (hbmDIB) {
		SelectObject(dib_hDC, hOldBitmap);
		DeleteDC(dib_hDC);
		DeleteObject(hbmDIB);
	}
	width = 0;
	height = 0;

}

inline void CSurface::Draw()
{
	BitBlt(dst_hDC,0,0,width,height,dib_hDC,0,0,SRCCOPY);
}


inline void CSurface::SetPixel(int x,int y,COLORREF color)
{
	//if(x == width) ASSERT(0);
	
	
	//y = height-1-y;
	LPDWORD p = (LPDWORD)(pbPixels + pitch *y)+x; // !!!!!be careful.one pixel
	                                              // hold 4 bits
	*p= RGB32(GetRValue(color),GetGValue(color),GetBValue(color));
}

inline void CSurface::SetPixel(int x,int y,BYTE r,BYTE g,BYTE b)
{
	//y = height-1-y;
	LPDWORD lpdw = (LPDWORD)(pbPixels + pitch *y )+x; 
	*lpdw = RGB32(r,g,b);
}
inline void CSurface::SetClearColor(COLORREF color)
{
	backgroundclr = RGB32(GetRValue(color),GetGValue(color),GetBValue(color));
}

extern CSurface *g_pCurSurface;
////////////////////////////////////////////////////////////////
typedef void* HRENDERCONTEXT;
inline HRENDERCONTEXT CreateSurface(/*HDC hdc,int nWidth,int nHeight*/)
{
	 CSurface *p = new CSurface;
	 /*p->Create(hdc,nWidth,nHeight);*/
     return (HRENDERCONTEXT)p;
}
inline  void MakeCurrentContext(HRENDERCONTEXT psurface)
{
  g_pCurSurface =  (CSurface*)psurface;
}

inline void SetClearColor(COLORREF clr)
{
    g_pCurSurface->SetClearColor(clr);
}

inline void DeleteContext()
{
	if(g_pCurSurface)
		delete g_pCurSurface;
}