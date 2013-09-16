
#include "basicgraphic.h"
#include "MathUtils.h"
#include "Rast.h"
#include "ObjectDef.h"
#include "ZBuffer.h"



#define set_pixel( x, y, c) {g_pCurSurface->SetPixel(x,y,c);}


static void plot_circle_points(int xc,int yc,int x,int y,int c)
{
	set_pixel(xc+x,yc+y,c);
	set_pixel(xc-x,yc+y,c);
	set_pixel(xc+x,yc-y,c);
	set_pixel(xc-x,yc-y,c);
	set_pixel(xc+y,yc+x,c);
	set_pixel(xc-y,yc+x,c);
	set_pixel(xc+y,yc-x,c);
	set_pixel(xc-y,yc-x,c);
}
void circle(int xc,int yc,int radius,COLORREF c)
{
	int x,y,p;
	x=0;
	y=radius;
	p=3-2*radius; 
	while(x<y){
		plot_circle_points(xc,yc,x,y,c);
		if(p<0) 
			p=p+4*x+6;
		else
		{
			p=p+4*(x-y)+10;
			y-=1;
		}
		x+=1;
	}
	if(x==y)
		plot_circle_points(xc,yc,x,y,c);
}

template<class T>
inline void swap(T& x,T &y)
{
	T temp =x;
	x = y;
	y= temp;
}
template<class T> T ABS(T x) { return(x < 0 ? -x : x); }


void line(int x1,int y1,int x2,int y2,COLORREF cr)
{
	int d, deltax, deltay, numpixels,
		dinc1, dinc2,
		xinc1, xinc2,
		yinc1, yinc2;

	// Calculate deltax and deltay for startup
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);

	// Initialize all vars based on which is the independent variable
	if (deltax >= deltay) {
		// x is independent variable
		numpixels = deltax + 1;
		dinc1 = deltay << 1;
		dinc2 = (deltay - deltax) << 1;
		d = dinc1 - deltax;
		xinc1 = xinc2 = yinc2 = 1;
		yinc1 = 0;
	}
	else {
		// y is independent variable
		numpixels = deltay + 1;
		dinc1 = deltax <<  1;
		dinc2 = (deltax - deltay) << 1;
		d = dinc1 - deltay;
		xinc1 = 0;
		xinc2 = yinc1 = yinc2 = 1;
	}

	// Make sure x and y move in the right directions
	if (x1 > x2) {
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if (y1 > y2) {
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}

	// Start drawing pixels at [x1, y1]
	for (int i = numpixels; i > 0; i--) {
		set_pixel(x1, y1, cr);
		if (d < 0) {
			d += dinc1;
			x1 += xinc1;
			y1 += yinc1;
		}
		else {
			d += dinc2;
			x1 += xinc2;
			y1 += yinc2;
		}
	}
	//POINT pt;
	//::MoveToEx(g_hMemdc,x1,y1,&pt);
	//::LineTo(g_hMemdc,x2,y2);
	////if(x1> x2)
	//{     
	//	swap(x1,x2);
	//}
	//if(y1 > y2)
	//{
	//	swap(y1,y2);
	//}
	//int dx = x2 - x1;
	//int dy = y2 - y1;
	//int y = y1;
	//int x = x1;
	//int deltax  = (dy <<1);
	//int deltay  = deltax - (dx <<1);
	//int eps = deltax -dx;//2* dy - dx
	//if( abs(dx) >= abs(dy))
	//{
	//	for(int x = x1; x <= x2;++x)
	//	{
	//		set_pixel(x,y,cr);
	//		if(eps <= 0)
	//		{
	//			//only increment x.
	//			eps += deltax;
	//		}
	//		else
	//		{
	//			eps += deltay;
	//			y++;
	//		}
	//	}

	//}
	//else
	//{
	//	eps = dy - (dx <<1) ;//dy-2dx
	//	deltax = dx <<1; 
	//	deltay = (dy - dx) << 1;
	//	for(int y = y1;y <= y2;++y)
	//	{
	//		set_pixel(x,y,cr);
	//		if(eps >=0)//should move the x step
	//		{
	//			eps += deltay;
	//			x++;
	//		}
	//		else
	//		{
	//			eps -= deltax;

	//		}
	//	}
	//}

}

/************************************************************************/
/* Version 1                                                                    */
/************************************************************************/
static void DrawTopTriangle(int x1,int y1,int x2,int y2,int x3,int y3,COLORREF clr)
{
	ASSERT(y1 == y2);
	if(x1>x2)
		swap(x1,x2);
	float deltaleft = (float)(x3-x1)/(float)(y3-y1);
	float deltaright = (float)(x3-x2)/(float)(y3-y2);
	float xleft = x1;
	float xright = x2;
	for(int y = y1;y <=y3;++y)
	{
		line((int)(xleft+0.5),y,(int)(xright+0.5),y,clr);
		xleft  += deltaleft;
		xright += deltaright;
	}

}
static void DrawBottomTriangle(int x1,int y1,int x2,int y2,int x3,int y3,COLORREF clr)
{
	ASSERT(y2 == y3);
	if(x2>x3)
		swap(x2,x3);
	float deltaleft = (float)(x1-x2)/(float)(y1-y2);
	float deltaright = (float)(x1-x3)/(float)(y1-y3);
	float xleft = x1;
	float xright = x1;
	for(int y = y1;y <=y3;++y)
	{

		line((int)(xleft+0.5),y,(int)(xright+0.5),y,clr);
		xleft  += deltaleft;
		xright += deltaright;

	}

}
void Triangle(int x1,int y1,int x2,int y2,int x3,int y3,COLORREF clr)
{
	if(((x1 == x2) && (x2 == x3)) ||
		((y1 == y2) && (y2 == y3)))
		return;
	//sort y1,y2,y3
	if(y1>y2)
	{
		swap(y1,y2);
		swap(x1,x2);
	}
	if(y1 >y3)
	{
		swap(y1,y3);
		swap(x1,x3);
	}
	if(y2>y3)
	{
		swap(y2,y3);
		swap(x2,x3);
	}
	

	if(y1 == y2)
	{
		DrawTopTriangle(x1,y1,x2,y2,x3,y3,clr);
	}
	else if( y2 == y3)
	{
		DrawBottomTriangle(x1,y1,x2,y2,x3,y3,clr);
	}
	else
	{
		int new_x = x1+ (float)(x3-x1)/(float)(y3-y1) *(y2-y1);
		DrawBottomTriangle(x1,y1,x2,y2,new_x,y2,clr);
		DrawTopTriangle(x2,y2,new_x,y2,x3,y3,clr);
	}

}

/************************************************************************/
/* Version 2                                                                    */
/************************************************************************/
static  void DrawTopTriangle_v2(float  x1,float  y1,float z1,
									  float  x2,float  y2,float z2,
									  float  x3,float  y3,float z3,
									  COLORREF clr)
{
	if(x1>x2)
	{
		swap(x1,x2);
		swap(z1,z2);
	}
	float deltaleft = (float)(x3-x1)/(y3-y1);
	float deltaright = (float)(x3-x2)/(y3-y2);
	int ystart = ceil(y1);
	int yend   = y3;
	float xleft = x1+deltaleft*(ystart-y1);
	float xright = x2+deltaright*(ystart-y1);

	//Add z buffer support
	float dy = y3-y1;
	float dzdyleft =(float)(z3-z1)/dy;
	float dzdyright =(float)(z3-z2)/dy;
	float zleft = z1;
	float zright =z2;
	ZBuffer *pzbuffer = ZBuffer::GetInstance();
    
	float dzdx = 0,zi;
	float dx ;
	for(int y = ystart;y <=yend;++y)
	{
		/*line(xleft,y,xright,y,clr);*/

		dx = xright - xleft;
		if(dx >0 || dx < 0)
		  dzdx = (float)(zright -zleft)/dx;

		zi = zleft;
		for(int x = xleft;x <= xright;++x)
		{
			if(zi < pzbuffer->GetZValue(x,y))
			{
               set_pixel(x,y,clr);
			   pzbuffer->SetZValue(x,y,zi);
			}
			zi += dzdx;
		}
		xleft  += deltaleft;
		xright += deltaright;
		zleft  += dzdyleft;
		zright += dzdyright;
	}

}
static  void DrawBottomTriangle_v2(float  x1,float  y1,float z1,
										 float  x2,float  y2,float z2,
										 float  x3,float  y3,float z3,
										 COLORREF clr)
{
	if(x2>x3)
	{
		swap(x2,x3);
		swap(z2,z3);
	}
	float deltaleft = (float)(x1-x2)/(float)(y1-y2);
	float deltaright = (float)(x1-x3)/(float)(y1-y3);
	int ystart = ceil(y1);
	int yend   = y2;
	float xleft = x1+deltaleft*(ystart-y1);
	float xright = x1+deltaright*(ystart-y1);

	//Add z buffer support
	float dy = y3 -y1;
	float dzdyleft =(float)(z2-z1)/dy;
	float dzdyright =(float)(z3-z1)/dy;
	float zleft = z1;
	float zright =z1;
	ZBuffer *pzbuffer = ZBuffer::GetInstance();

	float dzdx =0,zi,dx;
	for(int y = ystart;y <= yend;++y)
	{
		dx = xright - xleft;
		if(dx >0 || dx < 0)
			dzdx = (float)(zright -zleft)/dx;
		zi = zleft;

		//line(xleft,y,xright,y,clr);
		for(int x = xleft;x <= xright;++x)
		{
			if(zi < pzbuffer->GetZValue(x,y))
			{
				set_pixel(x,y,clr);
				pzbuffer->SetZValue(x,y,zi);
			}
			zi += dzdx;
		}
		xleft  += deltaleft;
		xright += deltaright;
		zleft  += dzdyleft;
		zright += dzdyright;

	}

}
void Triangle_v2(float  x1,float  y1,float z1,
				 float  x2,float  y2,float z2,
				 float  x3,float  y3,float z3,
				 COLORREF clr)
{
	
	//sort y1,y2,y3
	if(y1>y2)
	{
		swap(y1,y2);
		swap(x1,x2);
		swap(z1,z2);
	}
	if(y1 >y3)
	{
		swap(y1,y3);
		swap(x1,x3);
		swap(z1,z3);
	}
	if(y2>y3)
	{
		swap(y2,y3);
		swap(x2,x3);
		swap(z2,z3);
	}

	if(ABS(y1-y2) <= EPSILON)
	{
		DrawTopTriangle_v2(x1,y1,z1,x2,y2,z2,x3,y3,z3,clr);
	}
	else if( ABS(y2-y3) <= EPSILON)
	{
		DrawBottomTriangle_v2(x1,y1,z1,x2,y2,z2,x3,y3,z2,clr);
	}
	else
	{
		float new_x = x1+ (float)(x3-x1)/(float)(y3-y1) *(y2-y1);
		float new_z = z1 +(float)(z3-z1)/(y3-y1)*(y2-y1);
		DrawBottomTriangle_v2(x1,y1,z1,x2,y2,z2,new_x,y2,new_z,clr);
		DrawTopTriangle_v2(x2,y2,z2,new_x,y2,new_z,x3,y3,z3,clr);
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//gouraud colored triangle
////////////////////////////////////////////////////////////////////////////////////////


static void DrawGouraudTopTriangle(int x1,int y1,RGBA clr1,int x2,int y2,RGBA clr2,int x3,int y3,RGBA clr3)
{
	ASSERT(y1 == y2);
	if(x1>x2)
		swap(x1,x2);
	float deltaleft = (float)(x3-x1)/(float)(y3-y1);
	float deltaright = (float)(x3-x2)/(float)(y3-y2);
	Vector4D RGB1 = Vector4D(clr1.r,clr1.g,clr1.b);
	Vector4D RGB2 = Vector4D(clr2.r,clr2.g,clr2.b);
	Vector4D RGB3 = Vector4D(clr3.r,clr3.g,clr3.b);

	Vector4D dlrgbdy = (RGB3-RGB1)* (1/(y3-y1));
	Vector4D drrgbdy = (RGB3-RGB2)* (1/(y3-y1));

	float xleft = x1;
	float xright = x2;
	RGBA clr =clr1;
	Vector4D rgbleft=   Vector4D(clr1.r,clr1.g,clr1.b);
	Vector4D rgbright = Vector4D(clr2.r,clr2.g,clr2.b);
	for(int y = y1;y <=y3;++y)
	{
		if(EPSILON < abs(xright-xleft))
		{
			Vector4D di = (rgbright- rgbleft)*(1/(xright-xleft));
			for(int x = xleft; x<= xright;++x)
			{
			   
			   RGBA rgb = RGBA(rgbleft.x +di.x*(x-xleft),rgbleft.y+ di.y*(x-xleft),
				              rgbleft.z+di.z*(x-xleft));
               set_pixel(x,y,rgb.rgba);
			}
		}
		xleft  += deltaleft;
		xright += deltaright;
		rgbleft += dlrgbdy;
		rgbright += drrgbdy;
	}
}
static void DrawGouraudBottomTriangle(int x1,int y1,RGBA clr1,int x2,int y2,RGBA clr2,int x3,int y3,RGBA clr3)
{

	Vector4D RGB1 = Vector4D(clr1.r,clr1.g,clr1.b);
	Vector4D RGB2 = Vector4D(clr2.r,clr2.g,clr2.b);
	Vector4D RGB3 = Vector4D(clr3.r,clr3.g,clr3.b);

	Vector4D dlrgbdy = (RGB2-RGB1)* (1/(y2-y1));
	Vector4D drrgbdy = (RGB3-RGB1)* (1/(y3-y1));

	float deltaleft = (float)(x2-x1)/(float)(y2-y1);
	float deltaright = (float)(x3-x1)/(float)(y2-y1);

	Vector4D rgbleft = Vector4D(clr1.r,clr1.g,clr1.b);
	Vector4D rgbright = rgbleft;
   
	float xleft = x1;
	float xright = x1;
	//First set pixel for the p1
	set_pixel(x1,y1,clr1.rgba);
	for(int y = y1;y <= y3;++y)
	{
		Vector4D di;
		if(EPSILON < abs(xright-xleft))
		{
		   di = (rgbright- rgbleft)*(1/(xright-xleft));
		}
		for(int x = xleft; x<= xright;++x)
		{

			RGBA rgb = RGBA(rgbleft.x +di.x*(x-xleft),rgbleft.y+ di.y*(x-xleft),
				rgbleft.z+di.z*(x-xleft));
			set_pixel(x,y,rgb.rgba);
		}
		xleft  += deltaleft;
		xright += deltaright;
		rgbleft += dlrgbdy;
		rgbright += drrgbdy;
	} 

}
void DrawGouraudTriangle(int x1,int y1,RGBA clr1,int x2,int y2,RGBA clr2,int x3,int y3,RGBA clr3)
{
	if(((x1 == x2) && (x2 == x3)) ||
		((y1 == y2) && (y2 == y3)))
		return;
	//sort y1,y2,y3
	if(y1>y2)
	{
		swap(y1,y2);
		swap(x1,x2);
		swap(clr1,clr2);
	}
	if(y1 >y3)
	{
		swap(y1,y3);
		swap(x1,x3);
		swap(clr1,clr3);
	}
	if(y2>y3)
	{
		swap(y2,y3);
		swap(x2,x3);
		swap(clr2,clr3);
	}


	if(y1 == y2)
	{
		DrawGouraudTopTriangle(x1,y1,clr1,x2,y2,clr2,x3,y3,clr3);
	}
	else if( y2 == y3)
	{
		DrawGouraudBottomTriangle(x1,y1,clr1,x2,y2,clr2,x3,y3,clr3);
	}
	else
	{
		int new_x = x1+ (float)(x3-x1)/(float)(y3-y1) *(y2-y1);
		//
		Vector4D RGB1 = Vector4D(clr1.r,clr1.g,clr1.b);
		Vector4D RGB3 = Vector4D(clr3.r,clr3.g,clr3.b);
		Vector4D drrgbdy = (RGB3-RGB1)* (1/(y3-y1));
		Vector4D ADDED  = drrgbdy *(y2-y1);
		RGBA new_clr(ADDED.x+clr1.r,ADDED.y +clr1.g,ADDED.z +clr1.b);
		DrawGouraudBottomTriangle(x1,y1,clr1,x2,y2,clr2,new_x,y2,new_clr);
		DrawGouraudTopTriangle(x2,y2,clr2,new_x,y2,new_clr,x3,y3,clr3);
	}
}


/*************************************************************************************
/* Texture triangle
/************************************************************************************/
#define FIXP16_ROUND_UP  0x00008000
/************************************************************************/
/*  
   ...........................
   .                         .          
    .                       .      
	 .                     .   
	   .                 .
	    .             .
		  .         .
		    .    .
			   . .
			   top flat triangle

/************************************************************************/


static  void DrawTextureTopTriangle(float x1,float y1,float z1,float u1,float v1,
								   float x2,float y2,float z2,float u2,float v2,
								   float x3,float y3,float z3, float u3,float v3,
								   BITMAP_FILE_PTR texture,POLYGON *polygon)
{

	if(x1>x2)
	{
		swap(x1,x2);
		swap(u1,u2);
		swap(v1,v2);
		swap(z1,z2);
	}

	float dy = y3-y1;
	float dudyleft = (float)(u3-u1)/dy;
	float dvdyleft = (float)(v3-v1) /dy;
	float dudyright= (float)(u3-u2) /dy;
	float dvdyright = (float)(v3-v2)/dy;


	float uleft =  u1 ;
	float vleft =  v1 ;
	float uright = u2 ;
	float vright = v2;


	float deltaleft = (float)(x3-x1)/(y3-y1);
	float deltaright = (float)(x3-x2)/(y3-y2);
	int ystart = ceil(y1);
	int yend   = y3;
	float xleft = x1+deltaleft*(ystart-y1);
	float xright = x2+deltaright*(ystart-y1);

	//lighting support
	UCHAR lightedr =1,lightedg =1,lightedb =1;
	if(polygon->state & POLYGON_LIGHTING_PROCESSED)
	{
		lightedr = polygon->ret_color.r;
		lightedg = polygon->ret_color.g;
		lightedb = polygon->ret_color.b;
	}
	//Z buffer support
	float dzdyleft =(float)(z3-z1)/dy;
	float dzdyright =(float)(z3-z2)/dy;
	float zleft = z1;
	float zright =z2;
	ZBuffer *pzbuffer = ZBuffer::GetInstance();


	for(int y = ystart;y <=yend;++y)
	{
		float iux = uleft;
		float ivx = vleft;
		float izx = zleft;

		float dudx ,dvdx;
		float dzdx = 0;
	
		int xstart = (int) xleft;
		int xend   = (int )xright;

		int dx = xend - xstart;
        dudx = 0;dvdx = 0;
		if(dx >0 || dx < 0)
		{
			dudx =(uright-uleft)/dx;
			dvdx = (vright-vleft)/dx;
			dzdx = (zright - zleft)/dx;
		}
		for(int x= xstart;x <= xend;++x)
		{
			//add zbuffer support
			if(izx < pzbuffer->GetZValue(x,y))
			{
				BYTE r,g,b;
				int ui = iux ;
				int vi  = ivx ;
				float du = iux -ui;
				float dv = ivx -vi;
			    
				BYTE ra[12];//r1,g1,b1, r2,g2,b2, r3,g3,b3, r4,g4,b4
				texture->GetPixel(ui,vi,ra[0],ra[1],ra[2]);
				texture->GetPixel(ui+1,vi,ra[3],ra[4],ra[5]);
				texture->GetPixel(ui,vi+1,ra[6],ra[7],ra[8]);
				texture->GetPixel(ui+1,vi+1,ra[9],ra[10],ra[11]);
				float k1 =(1-du)*(1-dv);
				float k2 = du*(1-dv);
				float k3 = du*dv;
				float k4 =(1-du)*dv;
				r = ra[0]*k1+ra[3]*k2+ra[6]*k3+ra[9]*k4;
				g = ra[1]*k1+ra[4]*k2 +ra[7]*k3+ra[10]*k4;
				b = ra[2]*k1 +ra[5]*k2+ra[8]*k3+ra[11]*k4;

				r *= lightedr;
				g *= lightedg;
				b *= lightedb;
				set_pixel(x,y,RGB(r,g,b));
				pzbuffer->SetZValue(x,y,izx);

			}
			
			iux += dudx;
			ivx += dvdx;
			izx += dzdx;

		}
		xleft  += deltaleft;
		xright += deltaright;
		uleft  += dudyleft;
		vleft  += dvdyleft;
		uright += dudyright;
		vright += dvdyright;
		zleft  += dzdyleft;
		zright += dzdyright;
	}
	 
}
static  void DrawTextureBottomTriangle(float x1,float y1,float z1,float u1,float v1,
									  float x2,float y2,float z2,float u2,float v2,
									  float x3,float y3,float z3, float u3,float v3,
									  BITMAP_FILE_PTR texture,POLYGON *polygon)
{

	if(x2 > x3) {
		swap(x2,x3);
		swap(u2,u3);
		swap(v2,v3);
		swap(z2,z3);
	}

	float deltaleft = (float)(x1-x2)/(y1-y2);
	float deltaright = (float)(x1-x3)/(y1-y3);
	int ystart = ceil(y1);
	int yend   = y2;
	float xleft = x1+deltaleft*(ystart-y1);
	float xright = x1+deltaright*(ystart-y1);

	int dy = (y3-y1);

	float dudyleft = (float)(u2-u1)/dy;
	float dvdyleft = (float)(v2-v1)/dy;

	float dudyright= (float)(u3-u1) /dy;
	float dvdyright = (float)(v3-v1)/dy;

	
	
	float uleft = u1;
	float vleft = v1;
	float uright = u1;
	float vright = v1;

	//Z buffer support
	float dzdyleft =(float)(z2-z1)/dy;
	float dzdyright =(float)(z3-z1)/dy;
    float zleft = z1;
	float zright =z1;
	ZBuffer *pzbuffer = ZBuffer::GetInstance();

    int xstart,xend;

	//lighting support on texture polygon
	UCHAR lightedr =1,lightedg =1,lightedb =1;
	if(polygon->state & POLYGON_LIGHTING_PROCESSED)
	{
		lightedr = polygon->ret_color.r;
		lightedg = polygon->ret_color.g;
		lightedb = polygon->ret_color.b;
	}

	for(int y = ystart;y <= yend;++y)
	{

		float iux = uleft;
		float ivx = vleft;
		float izx = zleft;

		xstart = (int)xleft;
		xend   = (int)xright;

		float dudx =0.0,dvdx=0.0;
	    float dzdx =0.0;
		int dx = xend - xstart;

		if(abs(dx) !=0)
		{
			dudx =(float)(uright-uleft)/dx;
			dvdx = (float)(vright-vleft)/dx;
			dzdx = (float)(zright-zleft)/dx;
		}
	    

		for(int x= xstart;x <= xend;++x)
		{
			//add zbuffer support
			if(izx < pzbuffer->GetZValue(x,y))
			{

				int ui  = iux ;
				int vi  = ivx;
				float du = iux -ui;
				float dv = ivx -vi;

                BYTE r,g,b;
				BYTE ra[12];//r1,g1,b1, r2,g2,b2, r3,g3,b3, r4,g4,b4
				texture->GetPixel(ui,vi,ra[0],ra[1],ra[2]);
				texture->GetPixel(ui+1,vi,ra[3],ra[4],ra[5]);
				texture->GetPixel(ui,vi+1,ra[6],ra[7],ra[8]);
				texture->GetPixel(ui+1,vi+1,ra[9],ra[10],ra[11]);
				float k1 =(1-du)*(1-dv);
				float k2 = du*(1-dv);
				float k3 = du*dv;
				float k4 =(1-du)*dv;
				r = ra[0]*k1+ra[3]*k2+ra[6]*k3+ra[9]*k4;
				g = ra[1]*k1+ra[4]*k2 +ra[7]*k3+ra[10]*k4;
				b = ra[2]*k1 +ra[5]*k2+ra[8]*k3+ra[11]*k4;


				r *= lightedr;
				g *= lightedg;
				b *= lightedb;
				set_pixel(x,y,RGB(r,g,b));
				pzbuffer->SetZValue(x,y,izx);

			}

			iux += dudx;
			ivx += dvdx;
			izx += dzdx;
		}
		xleft  += deltaleft;
		xright += deltaright;
		uleft  += dudyleft;
		vleft  += dvdyleft;
		uright += dudyright;
		vright += dvdyright;
        zleft  += dzdyleft;
		zright += dzdyright;
	}

	

}

static  void DrawTextureTriangle(float x1,float y1,float z1,float u1,float v1,
									   float x2,float y2,float z2,float u2,float v2,
									   float x3,float y3,float z3, float u3,float v3,
									   BITMAP_FILE_PTR texture,POLYGON *polygon)
{
 
	 /* float z = (x2-x1)*(y3-y1)-(y2-y3)*(x3-x1);
	  if(z <0) return;*/
		//sort y1,y2,y3
		if(y1>y2)
		{
			swap(y1,y2);
			swap(x1,x2);
			swap(z1,z2);
			swap(u1,u2);
			swap(v1,v2);
		}
		if(y1 >y3)
		{
			swap(y1,y3);
			swap(x1,x3);
			swap(z1,z3);
			swap(u1,u3);
			swap(v1,v3);
		}
		if(y2>y3)
		{
			swap(y2,y3);
			swap(x2,x3);
			swap(z2,z3);
			swap(u2,u3);
			swap(v2,v3);
		}
		

		if(ABS(y1-y2) <= EPSILON)
		{
		   DrawTextureTopTriangle(x1,y1,z1,u1,v1,x2,y2,z2,u2,v2,x3,y3,z3,u3,v3,texture,polygon);
		}
		else if(ABS(y2-y3)<= EPSILON)
		{
		  DrawTextureBottomTriangle(x1,y1,z1,u1,v1,x2,y2,z2,u2,v2,x3,y3,z3,u3,v3,texture,polygon);
		}
		else
		{

		   float new_x = x1+ (float)(x3-x1)/(y3-y1) *(y2-y1);
		   float new_z = z1+(float)(z3-z1)/(y3-y1)*(y2-y1);
		   float du = (float)(u3-u1)/(y3-y1)*(y2-y1);
		   float dv = (float)(v3-v1)/(y3-y1)*(y2-y1);
		   float  new_u = u1 +du;
		   float  new_v = v1 +dv;

		   DrawTextureBottomTriangle(x1,y1,z1,u1,v1,x2,y2,z2,u2,v2,new_x,y2,new_z,new_u,new_v,texture,polygon);
		   DrawTextureTopTriangle(x2,y2,z2,u2,v2,new_x,y2,new_z,new_u,new_v,x3,y3,z3,u3,v3,texture,polygon);
		}
}

//This version support z buffer
void DrawTextureTriangle(POLYGON& poly,CRenderObject *pObject)
{
	int vindex0 = poly.v[0];
	int vindex1 = poly.v[1];
	int vindex2 = poly.v[2];
	VERTICESLIST& pointlist = pObject->m_translateverticesList;
	POINT2DLIST & TextureCoordinatesList = pObject->m_texturecoordinatesList;

	int tindex0 = poly.text[0];
	int tindex1 = poly.text[1];
	int tindex2 = poly.text[2];
	POINT3D & p1 = pointlist[vindex0].vertex;
	POINT3D & p2 = pointlist[vindex1].vertex;
	POINT3D & p3 = pointlist[vindex2].vertex;
	POINT2D &text1 = TextureCoordinatesList[tindex0];
	POINT2D &text2 = TextureCoordinatesList[tindex1];
	POINT2D &text3 = TextureCoordinatesList[tindex2];
	DrawTextureTriangle(p1.x,p1.y,p1.z,text1.x,text1.y,
		                p2.x,p2.y,p2.z,text2.x,text2.y,
						p3.x,p3.y,p3.z,text3.x,text3.y,
						poly.texture,&poly);

}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// The following version support 1/z buffer
//////////////////////////////////////////////////////////////////////////////////////////////
static  void DrawTextureTopTriangle_invz(float x1,float y1,float z1,float u1,float v1,
									float x2,float y2,float z2,float u2,float v2,
									float x3,float y3,float z3, float u3,float v3,
									BITMAP_FILE_PTR texture,POLYGON *polygon)
{

	if(x1>x2)
	{
		swap(x1,x2);
		swap(u1,u2);
		swap(v1,v2);
		swap(z1,z2);
	}

	float dy = y3-y1;
	float dudyleft = (float)(u3-u1)/dy;
	float dvdyleft = (float)(v3-v1) /dy;
	float dudyright= (float)(u3-u2) /dy;
	float dvdyright = (float)(v3-v2)/dy;


	float uleft =  u1 ;
	float vleft =  v1 ;
	float uright = u2 ;
	float vright = v2;


	float deltaleft = (float)(x3-x1)/(y3-y1);
	float deltaright = (float)(x3-x2)/(y3-y2);
	int ystart = ceil(y1);
	int yend   = y3;
	float xleft = x1+deltaleft*(ystart-y1);
	float xright = x2+deltaright*(ystart-y1);

	//lighting support
	UCHAR lightedr =1,lightedg =1,lightedb =1;
	if(polygon->state & POLYGON_LIGHTING_PROCESSED)
	{
		lightedr = polygon->ret_color.r;
		lightedg = polygon->ret_color.g;
		lightedb = polygon->ret_color.b;
	}
	//Z buffer support
	float dzdyleft =(float)(z3-z1)/dy;
	float dzdyright =(float)(z3-z2)/dy;
	float zleft = z1;
	float zright =z2;
	ZBuffer *pzbuffer = ZBuffer::GetInstance();


	for(int y = ystart;y <=yend;++y)
	{
		float iux = uleft;
		float ivx = vleft;
		float izx = zleft;

		float dudx ,dvdx;
		float dzdx = 0;

		int xstart = (int) xleft;
		int xend   = (int )xright;

		int dx = xend - xstart;
		dudx = 0;dvdx = 0;
		if(dx >0 || dx < 0)
		{
			dudx =(uright-uleft)/dx;
			dvdx = (vright-vleft)/dx;
			dzdx = (zright - zleft)/dx;
		}
		for(int x= xstart;x <= xend;++x)
		{
			//add zbuffer support
			if(izx > pzbuffer->GetZValue(x,y))
			{
				BYTE r,g,b;
				int ui  = iux/izx ;
				int vi  = ivx/izx ;
				
				//support linear sampling
				float du = iux/izx -ui;
				float dv = ivx/izx -vi;

				BYTE ra[12];//r1,g1,b1, r2,g2,b2, r3,g3,b3, r4,g4,b4
				texture->GetPixel(ui,vi,ra[0],ra[1],ra[2]);
				texture->GetPixel(ui+1,vi,ra[3],ra[4],ra[5]);
				texture->GetPixel(ui,vi+1,ra[6],ra[7],ra[8]);
				texture->GetPixel(ui+1,vi+1,ra[9],ra[10],ra[11]);
				float k1 =(1-du)*(1-dv);
				float k2 = du*(1-dv);
				float k3 = du*dv;
				float k4 =(1-du)*dv;
				r = ra[0]*k1+ra[3]*k2+ra[6]*k3+ra[9]*k4;
				g = ra[1]*k1+ra[4]*k2 +ra[7]*k3+ra[10]*k4;
				b = ra[2]*k1 +ra[5]*k2+ra[8]*k3+ra[11]*k4;

				r *= lightedr;
				g *= lightedg;
				b *= lightedb;
				set_pixel(x,y,RGB(r,g,b));
				pzbuffer->SetZValue(x,y,izx);

			}

			iux += dudx;
			ivx += dvdx;
			izx += dzdx;

		}
		xleft  += deltaleft;
		xright += deltaright;
		uleft  += dudyleft;
		vleft  += dvdyleft;
		uright += dudyright;
		vright += dvdyright;
		zleft  += dzdyleft;
		zright += dzdyright;
	}

}
static  void DrawTextureBottomTriangle_invz(float x1,float y1,float tz1,float tu1,float tv1,
									   float x2,float y2,float tz2,float tu2,float tv2,
									   float x3,float y3,float tz3, float tu3,float tv3,
									   BITMAP_FILE_PTR texture,POLYGON *polygon)
{

	if(x2 > x3) {
		swap(x2,x3);
		swap(tu2,tu3);
		swap(tv2,tv3);
		swap(tz2,tz3);
	}

	float deltaleft = (float)(x1-x2)/(y1-y2);
	float deltaright = (float)(x1-x3)/(y1-y3);
	int ystart = ceil(y1);
	int yend   = y2;
	float xleft = x1+deltaleft*(ystart-y1);
	float xright = x1+deltaright*(ystart-y1);

	int dy = (y3-y1);

	float dudyleft = (float)(tu2-tu1)/dy;
	float dvdyleft = (float)(tv2-tv1)/dy;

	float dudyright= (float)(tu3-tu1) /dy;
	float dvdyright = (float)(tv3-tv1)/dy;



	float uleft = tu1;
	float vleft = tv1;
	float uright = tu1;
	float vright = tv1;

	//Z buffer support
	float dzdyleft =(float)(tz2-tz1)/dy;
	float dzdyright =(float)(tz3-tz1)/dy;
	float zleft = tz1;
	float zright =tz1;
	ZBuffer *pzbuffer = ZBuffer::GetInstance();

	int xstart,xend;

	//lighting support on texture polygon
	UCHAR lightedr =1,lightedg =1,lightedb =1;
	if(polygon->state & POLYGON_LIGHTING_PROCESSED)
	{
		lightedr = polygon->ret_color.r;
		lightedg = polygon->ret_color.g;
		lightedb = polygon->ret_color.b;
	}

	for(int y = ystart;y <= yend;++y)
	{

		float iux = uleft;
		float ivx = vleft;
		float izx = zleft;

		xstart = (int)xleft;
		xend   = (int)xright;

		float dudx =0.0,dvdx=0.0;
		float dzdx =0.0;
		int dx = xend - xstart;

		if(abs(dx) !=0)
		{
			dudx =(float)(uright-uleft)/dx;
			dvdx = (float)(vright-vleft)/dx;
			dzdx = (float)(zright-zleft)/dx;
		}


		for(int x= xstart;x <= xend;++x)
		{
			//add zbuffer support
			if(izx > pzbuffer->GetZValue(x,y))
			{

				int ui  = iux /izx;
				int vi  = ivx/izx;

				BYTE r,g,b;

				//support linear sampling
				float du = iux/izx -ui;
				float dv = ivx/izx -vi;

				BYTE ra[12];//r1,g1,b1, r2,g2,b2, r3,g3,b3, r4,g4,b4
				texture->GetPixel(ui,vi,ra[0],ra[1],ra[2]);
				texture->GetPixel(ui+1,vi,ra[3],ra[4],ra[5]);
				texture->GetPixel(ui,vi+1,ra[6],ra[7],ra[8]);
				texture->GetPixel(ui+1,vi+1,ra[9],ra[10],ra[11]);
				float k1 =(1-du)*(1-dv);
				float k2 = du*(1-dv);
				float k3 = du*dv;
				float k4 =(1-du)*dv;
				r = ra[0]*k1+ra[3]*k2+ra[6]*k3+ra[9]*k4;
				g = ra[1]*k1+ra[4]*k2 +ra[7]*k3+ra[10]*k4;
				b = ra[2]*k1 +ra[5]*k2+ra[8]*k3+ra[11]*k4;

				r *= lightedr;
				g *= lightedg;
				b *= lightedb;
				set_pixel(x,y,RGB(r,g,b));
				pzbuffer->SetZValue(x,y,izx);

			}

			iux += dudx;
			ivx += dvdx;
			izx += dzdx;
		}
		xleft  += deltaleft;
		xright += deltaright;
		uleft  += dudyleft;
		vleft  += dvdyleft;
		uright += dudyright;
		vright += dvdyright;
		zleft  += dzdyleft;
		zright += dzdyright;
	}



}

static  void DrawTextureTriangle_invz(float x1,float y1,float z1,float u1,float v1,
								 float x2,float y2,float z2,float u2,float v2,
								 float x3,float y3,float z3, float u3,float v3,
								 BITMAP_FILE_PTR texture,POLYGON *polygon)
{

	/* float z = (x2-x1)*(y3-y1)-(y2-y3)*(x3-x1);
	if(z <0) return;*/
	//sort y1,y2,y3
	if(y1>y2)
	{
		swap(y1,y2);
		swap(x1,x2);
		swap(z1,z2);
		swap(u1,u2);
		swap(v1,v2);
	}
	if(y1 >y3)
	{
		swap(y1,y3);
		swap(x1,x3);
		swap(z1,z3);
		swap(u1,u3);
		swap(v1,v3);
	}
	if(y2>y3)
	{
		swap(y2,y3);
		swap(x2,x3);
		swap(z2,z3);
		swap(u2,u3);
		swap(v2,v3);
	}


	if(ABS(y1-y2) <= EPSILON)
	{
		DrawTextureTopTriangle_invz(x1,y1,1/z1,u1/z1,v1/z1,x2,y2,1/z2,u2/z2,v2/z2,x3,y3,1/z3,u3/z3,v3/z3,texture,polygon);
	}
	else if(ABS(y2-y3)<= EPSILON)
	{
		DrawTextureBottomTriangle_invz(x1,y1,1/z1,u1/z1,v1/z1,x2,y2,1/z2,u2/z2,v2/z2,x3,y3,1/z3,u3/z3,v3/z3,texture,polygon);
	}
	else
	{

		float new_x = x1+ (float)(x3-x1)/(y3-y1) *(y2-y1);
		float new_tz = 1/z1+(float)(1/z3-1/z1)/(y3-y1)*(y2-y1);
		float dtu = (float)(u3/z3-u1/z1)/(y3-y1)*(y2-y1);
		float dtv = (float)(v3/z3-v1/z1)/(y3-y1)*(y2-y1);
		float  new_tu = u1/z1 +dtu;
		float  new_tv = v1/z1 +dtv;

		DrawTextureBottomTriangle_invz(x1,y1,1/z1,u1/z1,v1/z1,x2,y2,1/z2,u2/z2,v2/z2,new_x,y2,new_tz,new_tu,new_tv,texture,polygon);
		DrawTextureTopTriangle_invz(x2,y2,1/z2,u2/z2,v2/z2,new_x,y2,new_tz,new_tu,new_tv,x3,y3,1/z3,u3/z3,v3/z3,texture,polygon);
	}
}

//This version support 1/z buffer
void DrawTextureTriangle_invz(POLYGON& poly,CRenderObject *pObject)
{
	int vindex0 = poly.v[0];
	int vindex1 = poly.v[1];
	int vindex2 = poly.v[2];
	VERTICESLIST& pointlist = pObject->m_translateverticesList;
	POINT2DLIST & TextureCoordinatesList = pObject->m_texturecoordinatesList;

	int tindex0 = poly.text[0];
	int tindex1 = poly.text[1];
	int tindex2 = poly.text[2];
	POINT3D & p1 = pointlist[vindex0].vertex;
	POINT3D & p2 = pointlist[vindex1].vertex;
	POINT3D & p3 = pointlist[vindex2].vertex;
	POINT2D &text1 = TextureCoordinatesList[tindex0];
	POINT2D &text2 = TextureCoordinatesList[tindex1];
	POINT2D &text3 = TextureCoordinatesList[tindex2];
	DrawTextureTriangle_invz(p1.x,p1.y,p1.z,text1.x,text1.y,
		p2.x,p2.y,p2.z,text2.x,text2.y,
		p3.x,p3.y,p3.z,text3.x,text3.y,
		poly.texture,&poly);

}
