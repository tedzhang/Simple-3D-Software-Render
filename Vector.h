#pragma  once 

class Matrix;

class Vector4D
{
public:
	union  
	{
		struct 
		{
			float x;float y; float z;float w;
		};
		float M[4];
	};
	Vector4D():x(0.0),y(0.0),z(0.0),w(1.0){ }
	Vector4D(float _x,float _y,float _z,float _w = 1.0):x(_x),y(_y),z(_z),w(_w){ }
	Vector4D(float *_M);
	Vector4D(const Vector4D& other);
	Vector4D& operator=(const Vector4D& rhs);

	const Vector4D operator +(const Vector4D& rhs) const;
	const Vector4D operator -(const Vector4D& rhs) const;
	const Vector4D operator *(float k) const ;
	const Vector4D operator *(const Matrix& rhs) const;
	friend const Vector4D operator *(const Matrix& lhs,const Vector4D& rhs);
	friend const Vector4D operator *(float k,const Vector4D& rhs);
	Vector4D& operator +=(const Vector4D& rhs);
	Vector4D& operator -=(const Vector4D& rhs);
	Vector4D& operator *=(float k);
	const Vector4D operator *=(const Matrix& rhs) ;

	const float DotProduct  (const Vector4D& rhs) const;
	const Vector4D CrossProduct(const Vector4D &rhs) const;
	float GetLength()const;
	void  Normalize();
    


};
extern const Vector4D ZeroVector;
typedef Vector4D POINT3D;