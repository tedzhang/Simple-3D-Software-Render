#pragma  once 

class Matrix
{
public:

	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float M[4][4];
	};
	Matrix(float *_m);
	Matrix();
	Matrix(float m11,float m12,float m13,float m14,
		float m21,float m22,float m23,float m24,
		float m31,float m32,float m33,float m34,
		float m41,float m42,float m43,float m44);
	Matrix(const Matrix& other);

	void Identity();
	Matrix& operator=(const Matrix& other);
	const Matrix operator+(const Matrix& other)const;
	const Matrix operator-(const Matrix& other)const;
	const Matrix operator*(const Matrix& other)const;
	const Matrix operator*(float k)const;
	friend const Matrix operator*(float k,const Matrix& rhs);
	Matrix& operator +=(const Matrix& other);
	Matrix& operator -=(const Matrix& other);
	Matrix& operator *=(const Matrix& other);
	Matrix& operator *=(float k);


};