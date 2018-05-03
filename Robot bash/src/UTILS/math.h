#ifndef MATH_H
#define MATH_H

#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2

#define DEG_TO_RAD(deg) (deg * M_PI / 180.0)
#define RAD_TO_DEG(rad) (rad * 180.0 / M_PI)

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
};

//}; // namespace
#endif
