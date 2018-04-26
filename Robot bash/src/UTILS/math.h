#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

struct Dimension
{
	float width = 0;
	float height = 0;
	Dimension(){}
	Dimension(float w, float h) : width(w), height(h){}
};

class Vector2
{
public:
	float x = 0;
	float y = 0;
	Vector2(){}
	Vector2(float x, float y) : x(x), y(y){}
	Vector2(const Vector2 &v);
	void Normalize();
	float Magnitude();
	float SquaredMagnitude();
	Vector2& operator=(const Vector2& v2);
	Vector2& operator+=(const Vector2& v2);
	Vector2 operator+(const Vector2& v2);
	Vector2& operator-=(const Vector2& v2);
	Vector2 operator-(const Vector2& v2);
	Vector2 operator-();
	Vector2& operator*=(float f);
	Vector2 operator*(float f);
	friend Vector2 operator*(float f, const Vector2& v);
	float Dot(const Vector2& v2);
	float Angle(Vector2& v2);
	Vector2 Reflect(Vector2 normal);
	void DEBUGPrint();
};


class Vector3
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float arr[3];
	};
	Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z){}
	void Normalize();
	float Magnitude();
	float SquaredMagnitude();
	//Vector3& operator+=(const Vector3& v2);
	//Vector3 operator+(const Vector3& v2);
	//Vector3& operator*=(float f);
	void DEBUGPrint();
};



namespace Math
{
	inline static int Min( int _valA, int _valB ) { if( _valA > _valB ) return _valB; else return _valA; }
	inline static int Max( int _valA, int _valB ) { if( _valA < _valB ) return _valB; else return _valA; }
	inline static int Clamp(int _value, int _min, int _max) { return Min(_max, Max(_min, _value)); }
	inline static float Clampf(float _value, float _min, float _max) { return fmin(_max, fmax(_min, _value)); }
	inline static int Truncate(float _value) { return (int)(_value); }
	inline static int Round(float val){ return (int)(val + 0.5f); }
	inline static float Roundf(float val){ return roundf(val); }
	inline static int Floor(float _value) { return (int)floorf(_value); }
	inline static int Ceil(float _value) { return (int)ceilf(_value); }
	inline static int SignOf(float val) { return val > 0 ? 1 : -1; }
	float RoundToN(float val, int n);
	float DistancePointToLine(Vector2 point, Vector2 lineStart, Vector2 lineEnd);

	// name already taken by windows.h
	class Rectangle
	{
	public:
		float x = 0;
		float y = 0;
		float width = 0;
		float height = 0;
		Rectangle(){}
		Rectangle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height){}
		bool Overlaps(const Rectangle& other);
		Vector2 ClosestPointInside(Vector2 v);
		Vector2 GetPosition();
	};

};

//}; // namespace
#endif
