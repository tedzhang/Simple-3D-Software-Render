#pragma  once 
#include "Globals.h"
#include "basicgraphic.h"
#include "ObjectDef.h"

void circle(int xc,int yc,int radius,COLORREF c);
void line(int x1,int y1,int x2,int y2,COLORREF cr);
void Triangle(int x1,int y1,int x2,int y2,int x3,int y3,COLORREF clr);
void Triangle_v2(float  x1,float  y1,float z1,
							float  x2,float  y2,float z2,
							float  x3,float  y3,float z3,
							COLORREF clr);

void DrawGouraudTriangle(int x1,int y1,RGBA clr1,int x2,int y2,RGBA clr2,int x3,int y3,RGBA clr3);
//void  DrawTextureTriangle(float x1,float y1,float u1,float v1,float x2,float y2,float u2,float v2,float x3,float y3,
//						  float u3,float v3,BITMAP_FILE_PTR texture,POLYGON *polygon );
void DrawTextureTriangle(POLYGON& poly,CRenderObject *pObject);
void DrawTextureTriangle_invz(POLYGON& poly,CRenderObject *pObject);

