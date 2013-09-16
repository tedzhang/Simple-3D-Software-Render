#include "Matrix.h"
#include "Utils.h"
#include "Globals.h"

Matrix::Matrix(float *_m)
{
	memcpyquad(M,_m,16);

}
Matrix::Matrix()
{
	memzeroquad(M,16);
}

Matrix::Matrix(float m11,float m12,float m13,float m14,
			   float m21,float m22,float m23,float m24,
			   float m31,float m32,float m33,float m34,
			   float m41,float m42,float m43,float m44)
{
	_11 = m11,_12 = m12,_13 = m13,_14= m14;
	_21 = m21,_22 = m22,_23 = m23,_24 = m24;
	_31 = m31,_32 = m32,_33 = m33,_34 = m34;
	_41 = m41,_42 = m42,_43 = m43,_44 = m44;
}
Matrix::Matrix(const Matrix& other)
{
	memcpyquad(M,other.M,16);
}

void Matrix::Identity()
{
	memzeroquad(M,16);
	_11 = 1;
	_22 = 1;
	_33 = 1;
	_44 = 1;
}
Matrix& Matrix::operator=(const Matrix& other)
{
	if(this != &other)
		memcpyquad(M,other.M,16);
	return *this;

}
const Matrix Matrix::operator+(const Matrix& other)const
{
	Matrix temp(*this);
	temp += other;
	return temp;

}
const Matrix Matrix::operator-(const Matrix& other)const
{
	Matrix temp(*this);
	temp -= other;
	return temp;
}
const Matrix Matrix::operator*(const Matrix& other)const
{
	Matrix temp(*this);
	temp *= other;
	return temp;
}
const Matrix Matrix::operator*(float k)const
{
	Matrix temp(*this);
	temp *= k;
	return temp;
}

const Matrix operator*(float k,const Matrix& rhs)
{
	return rhs*k;
}

Matrix& Matrix::operator +=(const Matrix& other)
{
	for(int i = 0; i <4;i++)
	{
		for(int j = 0; j < 4;j++)
			M[i][j] += other.M[i][j];
	}
	return *this;
}
Matrix& Matrix::operator -=(const Matrix& other)
{
	for(int i = 0; i <4;i++)
	{
		for(int j = 0; j < 4;j++)
			M[i][j] -= other.M[i][j];
	}
	return *this;
}
Matrix& Matrix::operator *=(const Matrix& other)
{
	for(int i = 0; i <4;i++)//row
	{
		for(int j = 0; j < 4;j++)
		{
			float sum = 0.0;
			for(int k = 0; k <4;k++)
			{
				sum += M[i][k]*other.M[k][j];
			}
			M[i][j] = sum;
			
		}
	}
	return *this;

}
Matrix& Matrix::operator *=(float k)
{
	for(int i =0; i < 4;++i)
		for(int j = 0; j <4;++j)
		{
			M[i][j] *= k;
		}

		return *this;
}

