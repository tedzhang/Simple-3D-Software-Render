#pragma	    once

#include <math.h>
#include <float.h>
#include <stdlib.h>

#define TWO_PI          6.2831853f
#define EPSILON         0.0001f
#define EPSILON_SQUARED EPSILON*EPSILON
#define RAD2DEG         57.2957f
#define DEG2RAD         0.0174532f

template <class T>
inline T clamp(T x, T min, T max)
{ 
	return (x < min) ? min : (x > max) ? max : x;
}
template<class T>
inline void Swap(T& x,T&y)
{
	T temp = x;
	x =temp;
	temp = y;
}

inline int round(float f)
{
	return int(f + 0.5f);
}

inline float getNextRandom(){
	return (float)rand()/(RAND_MAX + 1);
}

inline int getClosest(int arg, int firstVal, int secondVal)
{
	int difference1 = 0,
		difference2 = 0;

	difference1 = abs(arg - firstVal);
	difference2 = abs(arg - secondVal);
	if(difference1 < difference2)
		return firstVal;

	return secondVal;
}

inline int getClosestPowerOfTwo(int digit)
{
	if(!digit)
		return 1;

	double log2  = log(double(abs(digit)))/log(2.0),
		flog2 = floor(log2),
		frac  = log2 - flog2;

	return (frac < 0.5) ? (int)pow(2.0, flog2) : (int)pow(2.0, flog2 + 1.0);
}

inline float fastSquareRoot(float x)
{
	__asm{
		fld x;
		fsqrt;
		fstp x;
	}
	return x;
}

inline float fastCos(float x)
{
	__asm{
		fld x;
		fcos;
		fstp x;
	}
	return x;
}

inline float fastSin(float x)
{
	__asm{
		fld x;
		fsin;
		fstp x;
	}
	return x;
}