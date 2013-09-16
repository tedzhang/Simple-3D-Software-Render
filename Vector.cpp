#include "Vector.h"
#include "Utils.h"
#include "Matrix.h"

const Vector4D ZeroVector(0,0,0,1);
inline Vector4D::Vector4D(const Vector4D& other)
{
    memcpyquad(this->M,other.M);
}
inline Vector4D::Vector4D(float *_M)
{
	memcpyquad(this->M,_M);
}

Vector4D& Vector4D::operator=(const Vector4D& rhs)
{
   if(this != &rhs)
   {
        memcpyquad(M,rhs.M);
   }
   return *this;

}
const Vector4D Vector4D::operator +(const Vector4D& rhs)const
{
	Vector4D ret(*this);
	ret += rhs;
	return ret;
}

const Vector4D Vector4D::operator -(const Vector4D& rhs) const
{
	Vector4D ret(*this);
	ret -= rhs;
	return ret;
}

const Vector4D Vector4D::operator *(float k) const
{
	return Vector4D(k*x ,k*y,k*z,w*k);
}
Vector4D& Vector4D::operator *=(float k)
{
	x *= k;
	y *= k;
	z *= k;
	w *= k;
	return *this;
}
const Vector4D operator *(float k,const Vector4D& rhs)
{
	return rhs *k;
}

Vector4D& Vector4D::operator +=(const Vector4D& rhs)
{
     x += rhs.x;
	 y += rhs.y;
	 z += rhs.z;
	 w  = 1.0;
	 return *this;
}
Vector4D& Vector4D::operator -=(const Vector4D& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w  = 1.0;
	return *this;
}

const float Vector4D::DotProduct(const Vector4D& rhs) const
{
    return x*rhs.x+y* rhs.y+z*rhs.z;
}
const Vector4D Vector4D::CrossProduct(const Vector4D &rhs) const
{

  return Vector4D(y*rhs.z - rhs.y * z ,
	              rhs.x * z - x * rhs.z,
				  x * rhs.y - rhs.x * y,
				  1.0);
}
inline float Vector4D::GetLength()const
{
	return sqrt(x*x + y*y+z*z);
}

void Vector4D::Normalize()
{
	float factor = 1/GetLength();
	x *=factor;
	y *=factor;
	z *= factor;
	w  = 1.0;
}

const Vector4D Vector4D::operator *(const Matrix& rhs) const
{
    Vector4D ret(*this);
	ret *= rhs;
	return ret;
}
const Vector4D operator *(const Matrix& lhs,const Vector4D& rhs)
{
      return rhs*lhs;
}
const Vector4D Vector4D::operator *=(const Matrix& rhs) 
{
    float _x = x*rhs._11  + y* rhs._21+z*rhs._31+ w*rhs._41;
	float _y = x *rhs._12 + y *rhs._22+ z*rhs._32+w*rhs._42;
	float _z = x *rhs._13 + y * rhs._23+ z * rhs._33+w*rhs._43;
	float _w = x* rhs._14  + y* rhs._24 + z * rhs._34  + w* rhs._44;
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	return *this;
}