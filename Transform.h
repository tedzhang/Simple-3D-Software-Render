#pragma  once 

#include "Vector.h"
#include "Matrix.h"
#include "MathUtils.h"

inline void Translate(POINT3D& pt,float x,float y,float z)
{
	pt.x += x;
	pt.y += y;
	pt.z += z;
}

inline void  Scale(POINT3D &pt,float kx,float ky,float kz)
{
	pt.x  *= 1/kx;
	pt.y  *= 1/ky;
	pt.z  *= 1/kz;
}

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
inline void Rotate(POINT3D& pt,int axistype,float deg)
{
	float rad = DEG2RAD * deg;
	float sinrad = sin(rad);
	float cosrad = cos(rad);
    float _x = pt.x;
	float _y = pt.y;
	float _z = pt.z;
	switch(axistype)
	{
	case X_AXIS:
		{
			pt.y = _y *cosrad - _z * sinrad;
			pt.z = _y *sinrad + _z * cosrad;
		}
		break;
	case Y_AXIS:
		{
			pt.x = _x * cosrad + _z *sinrad;
			pt.z = _x * (-sinrad)+ _z *cosrad;
		}
		break;
	case Z_AXIS:
		{
           pt.x = _x *cosrad - _y*sinrad;
		   pt.y = _x *sinrad + _y*cosrad;
		}
		break;
	default:
		{
			ASSERT(0);
		}
		break;

	}

}

inline void  BuildTranslateMatrix(Matrix& retMatrix,float x,float y,float z)
{
	retMatrix.Identity();
	retMatrix._41=x;
	retMatrix._42=y;
	retMatrix._43 =z;
}

inline void BuildScaleMatrix(Matrix& retMatrix,float kx,float ky,float kz)
{
	retMatrix.Identity();

	retMatrix._11 = kx;
	retMatrix._22 = ky;
	retMatrix._33 = kz;
}

//x,y,z 's unit is degree 
inline void Bulid_XYZ_RotationMatrix(Matrix &retMatrix,float x,float y,float z)
{
	float _x = DEG2RAD * x;
	float _y = DEG2RAD * y;
	float _z = DEG2RAD * z;


	retMatrix.Identity();
	//rotate around x axis
	//| 1  0    0  0 |
	//| 0 cosa sina 0|
	//| 0 -sina cosa 0|
	//| 0 0      0    1|
	if(fabs(_x)>EPSILON)
	{ 
		retMatrix._22 = cos(_x);
		retMatrix._23 = sin(_x);
		retMatrix._32 = - retMatrix._23;
		retMatrix._33 = retMatrix._22;
	}
	//rotate around y axis
	//| cosa 0 -sina 0|
	//| 0    1   0   0|
	//| sina 0 cosa 0 |
	//| 0    0  0   1|
	else if(fabs(_y)>EPSILON)
	{
		retMatrix._11 = cos(_y);
		retMatrix._13 = -sin(_y);
		retMatrix._31 = - retMatrix._13;
		retMatrix._33 = retMatrix._11;
	}
	//rotate around z axis
	//| cosa sina 0 0 |
	//| -sina cosa 0 0|
	//| 0   0   1  0|
	//|0    0  0  1|
	else if(fabs(_z) >EPSILON)
	{
		retMatrix._11 = cos(_z);
		retMatrix._12 = sin(_z);
		retMatrix._21 =-retMatrix._12;
		retMatrix._22 =retMatrix._11;
	}
	
}