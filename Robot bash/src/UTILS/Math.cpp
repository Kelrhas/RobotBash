#include "stdafx.h"
#include "Math.h"

Vector2::Vector2(const Vector2 &v)
{
	this->x = v.x;
	this->y = v.y;
}

float Vector2::Magnitude()
{
	return sqrtf(x*x + y*y);
}

float Vector2::SquaredMagnitude()
{
	return (x*x + y*y);
}

void Vector2::Normalize()
{
	float magnitude = this->Magnitude();
	if (magnitude > 0)
	{
		x /= magnitude;
		y /= magnitude;
	}
}

Vector2& Vector2::operator=(const Vector2& v2)
{
	this->x = v2.x;
	this->y = v2.y;
	
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& v2)
{
	this->x += v2.x;
	this->y += v2.y;
	return *this;
}

Vector2 Vector2::operator+(const Vector2& v2)
{
	Vector2 vRes = *this;
	return (vRes += v2);
}

Vector2& Vector2::operator-=(const Vector2& v2)
{
	this->x -= v2.x;
	this->y -= v2.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& v2)
{
	Vector2 vRes = *this;
	return (vRes -= v2);
}
Vector2 Vector2::operator-()
{
	return Vector2(-this->x, -this->y);
}

Vector2& Vector2::operator*=(float f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}

Vector2 Vector2::operator*(float f)
{
	Vector2 res = *this;
	return (res *= f);
}

Vector2 operator*(float f, const Vector2 &v)
{
	Vector2 res = v;
	return (res *= f);
}

inline float Vector2::Dot(const Vector2& v2)
{
	return this->x * v2.x + this->y * v2.y;
}

inline float Vector2::Angle(Vector2& v2)
{
	float magA = this->Magnitude();
	float magB = v2.Magnitude();
	float dot = this->Dot(v2);
	float angleCos = dot / (magA * magB);

	return acosf(angleCos);
}

Vector2 Vector2::Reflect(Vector2 normal)
{
	Vector2 res = *this;
	normal.Normalize();
	float dot = this->Dot(normal);

	res -= 2 * dot * normal;

	return res;
}

void Vector2::DEBUGPrint()
{
	char buffer[32];
	sprintf_s(buffer, "(%.02f, %.02f)\n", x, y);
	OutputDebugStringA(buffer);
}



inline bool Math::Rectangle::Overlaps(const Math::Rectangle& other)
{
	if (this->x > other.x + other.width || other.x > this->x + this->width ||
		this->y > other.y + other.height || other.y > this->y + this->height)
	{
		return false;
	}
	return true;
}

inline Vector2 Math::Rectangle::ClosestPointInside(Vector2 v)
{
	v.x = Clampf(v.x, this->x, this->x + this->width);
	v.y = Clampf(v.y, this->y, this->y + this->height);
	return v;
}

inline Vector2 Math::Rectangle::GetPosition()
{
	return Vector2(this->x, this->y);
}


namespace Math
{
	float RoundToN(float val, int n)
	{
		int multiplier = (int)(val / n);

		if (fmod(val, (float)n) < (n *0.5f))
		{
			return (float)multiplier * n;
		}
		else
		{
			return (float)(multiplier + 1) * n;
		}
	}


	float DistancePointToLine(Vector2 point, Vector2 lineStart, Vector2 lineEnd)
	{
		float lineLength = sqrtf(pow(lineEnd.x - lineStart.x, 2) + pow(lineEnd.y - lineStart.y, 2));
		float distance = (lineEnd.y - lineStart.y)*point.x;
		distance -= (lineEnd.x - lineStart.x) * point.y;
		distance += lineEnd.x * lineStart.y;
		distance -= lineEnd.y * lineStart.x;

		return fabs(distance) / lineLength;
	}
}




float Vector3::Magnitude()
{
	return sqrtf(x*x + y*y + z*z);
}

inline float Vector3::SquaredMagnitude()
{
	return (x*x + y*y + z*z);
}

void Vector3::Normalize()
{
	float magnitude = this->Magnitude();
	if (magnitude > 0)
	{
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}
}
/*
inline Vector3& Vector3::operator+=(const Vector3& v2)
{
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	return *this;
}

inline Vector3 Vector3::operator+(const Vector3& v2)
{
	Vector3 vRes = *this;
	return (vRes += v2);
}

inline Vector3& Vector3::operator*=(float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
	return *this;
}

inline Vector3 operator*(Vector3 &v, float f)
{
	return (v *= f);
}

inline Vector3 operator*(float f, Vector3 &v)
{
	return (v *= f);
}
*/
void Vector3::DEBUGPrint()
{
	char buffer[32];
	sprintf_s(buffer, "(%.02f, %.02f, %.02f)\n", x, y, z);
	OutputDebugStringA(buffer);
}