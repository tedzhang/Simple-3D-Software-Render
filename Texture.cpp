#include "Texture.h"
#include <stdio.h>
bool LoadBitmapFile(BITMAP_FILE_PTR bitmap,const char* filename)
{
	FILE* hfile;
	if(!(hfile = fopen(filename,"r")))
		return false;
	fread(&bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER),1,hfile);
	if(bitmap->bitmapfileheader.bfType != 0X4D42)
	{
		fclose(hfile);
		return false;
	}

	fread(&bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER),1,hfile);
	int nbitcout = bitmap->bitmapinfoheader.biBitCount;
	if(nbitcout == 8)
	{
		//Load the palette info
	}
	int ndatasize = bitmap->bitmapfileheader.bfSize -
		            bitmap->bitmapfileheader.bfOffBits;
	//now we already get the actual datasize
	int byteperline =bitmap->bitmapinfoheader.biWidth*nbitcout/8;
	if(bitmap->bitmapinfoheader.biSizeImage == 0)
	{
		bitmap->bitmapinfoheader.biSizeImage = byteperline* bitmap->bitmapinfoheader.biHeight;
	}
	fseek(hfile,-1* bitmap->bitmapinfoheader.biSizeImage,SEEK_END);

	bitmap->buffer = (UCHAR*)malloc(bitmap->bitmapinfoheader.biSizeImage);

	fread(bitmap->buffer,sizeof(UCHAR),bitmap->bitmapinfoheader.biSizeImage,hfile);

	if(bitmap->bitmapinfoheader.biHeight >0)//reverse
	{
          UCHAR * temp =(UCHAR*)malloc(bitmap->bitmapinfoheader.biSizeImage);
		  memcpy(temp,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);
		  int biheight = bitmap->bitmapinfoheader.biHeight;
		 // UCHAR * rowaddress = bitmap->buffer+byteperline*(biheight-1);

		  for(int idx = 0; idx < biheight;++idx)
		  {
			  
			  memcpy((void*)(bitmap->buffer+(biheight-idx-1)*byteperline),
				     temp+idx*byteperline,byteperline);
		  }
		  free(temp);
	}
	return true;
  
}

bool UnLoadBitMap(BITMAP_FILE_PTR bitmap)
{
	if(bitmap->buffer)
	{
		free(bitmap->buffer);
	}
	return 1;
}

