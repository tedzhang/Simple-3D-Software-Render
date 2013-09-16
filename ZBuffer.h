#pragma  once 
#include "Globals.h"
//Only support 32 bit 
class ZBuffer
{
public:
	static ZBuffer *GetInstance();
	bool Create(int width,int height);
	void Clear(int data);
	void Destroy();
	float *GetZbuffer(){ return zbuffer;}
	float GetZValue(int x,int y);
	void  SetZValue(int x,int y,float data);
protected:
	ZBuffer():zbuffer(NULL){ }
private:
	int m_width;
	int m_height;
	float *zbuffer;
	int nSize;//how many dwords
	int npitch;
	static ZBuffer* instance;

};


inline bool ZBuffer::Create(int width,int height)
{
	if(zbuffer)
	{
		delete []zbuffer;
		zbuffer = NULL;
	}
	m_width = width;
	m_height = height;
	
	npitch = ((m_width<< 2) + 3) &( ~3);
	nSize = (npitch*m_height) ;//how many floats in this array
	zbuffer =(float*)malloc(nSize<<2);
	return true;

}

inline void ZBuffer::Clear(int data)
{

	for(int i = 0; i < nSize;++i)
		zbuffer[i] = data;
}

inline void ZBuffer::Destroy()
{
	delete zbuffer;
	zbuffer = NULL;
}

inline ZBuffer* ZBuffer::GetInstance()
{
	if(!instance)
	{
		instance = new ZBuffer;
	}
	return instance;
}

inline float ZBuffer::GetZValue(int x,int y)
{
	return zbuffer[npitch*y+x];
}
inline  void ZBuffer::SetZValue(int x,int y,float data)
{
	zbuffer[npitch*y+x]=data;



}