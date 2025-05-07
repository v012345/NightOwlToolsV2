#include "math/AEVec2.h"

NS_AE_BEGIN

template<typename T>
inline AEVec2<T>::AEVec2()
: x(0)
, y(0)
{

}

template<typename T>
inline AEVec2<T>::AEVec2(T x, T y)
: x(x)
, y(y)
{

}

template<typename T>
inline AEVec2<T>::AEVec2(const T* array)
{
	set(array);
}

template<typename T>
inline AEVec2<T>::AEVec2(const AEVec2<T>& p1, const AEVec2<T>& p2)
{
	set(p1, p2);
}

template<typename T>
inline AEVec2<T>::AEVec2(const AEVec2<T>& v)
{
	set(v);
}

template<typename T>
inline AEVec2<T>::~AEVec2()
{

}

template<typename T>
inline const AEVec2<T> AEVec2<T>::operator+(const AEVec2<T>& v) const
{
	AEVec2<T> result(*this);
	result.add(v);
	return result;
}

template<typename T>
inline AEVec2<T>& AEVec2<T>::operator+=(const AEVec2<T>& v)
{
	add(v);
	return *this;
}

template<typename T>
inline const AEVec2<T> AEVec2<T>::operator-(const AEVec2<T>& v) const
{
	AEVec2<T> result(*this);
	result.subtract(v);
	return result;
}

template<typename T>
inline AEVec2<T>& AEVec2<T>::operator-=(const AEVec2<T>& v)
{
	subtract(v);
	return *this;
}

template<typename T>
inline const AEVec2<T> AEVec2<T>::operator-() const
{
	AEVec2<T> result(*this);
	result.negate();
	return result;
}

template<typename T>
inline const AEVec2<T> AEVec2<T>::operator*(T v) const
{
	AEVec2<T> result(*this);
	result.scale(v);
	return result;
}

template<typename T>
inline AEVec2<T>& AEVec2<T>::operator *= (T v)
{
	scale(v);
	return *this;
}

template<typename T>
inline const AEVec2<T> AEVec2<T>::operator/(const T v) const
{
	return AEVec2<T>(this->x / v, this->y / v);
}

template<typename T>
inline bool AEVec2<T>::operator<(const AEVec2<T>& v) const
{
	if (x == v.x) return y < v.y;
	return x < v.x;
}

template<typename T>
inline bool AEVec2<T>::operator>=(const AEVec2<T>& v) const
{
	return !(*this < v);
}

template<typename T>
inline bool AEVec2<T>::operator>(const AEVec2<T>& v) const
{
	if (x == v.x) return y > v.y;
	return x > v.x;
}

template<typename T>
inline bool AEVec2<T>::operator<=(const AEVec2<T>& v) const
{
	return !(*this > v);
}

template<typename T>
inline bool AEVec2<T>::operator==(const AEVec2<T>& v) const
{
	return x == v.x && y == v.y;
}

template<typename T>
inline bool AEVec2<T>::operator!=(const AEVec2<T>& v) const
{
	return x != v.x || y != v.y;
}

template<typename T>
inline void AEVec2<T>::set(T x, T y)
{
	this->x = x;
	this->y = y;
}

template<typename T>
inline void AEVec2<T>::set(const AEVec2<T>& v)
{
	this->x = v.x;
	this->y = v.y;
}

template<typename T>
inline void AEVec2<T>::set(const AEVec2<T>& p1, const AEVec2<T>& p2)
{
	this->x = p2.x - p1.x;
	this->y = p2.y - p1.y;
}

template<typename T>
inline void AEVec2<T>::setZero()
{
	x = 0.0f;
	y = 0.0f;
}

template<typename T>
inline bool AEVec2<T>::isZero() const
{
	return x == 0.0f && y == 0.0f;
}

template<typename T>
inline void AEVec2<T>::add(const AEVec2<T>& v)
{
	x += v.x;
	y += v.y;
}

template<typename T>
inline void AEVec2<T>::subtract(const AEVec2<T>& v)
{
	x -= v.x;
	y -= v.y;
}

template<typename T>
inline T AEVec2<T>::dot(const AEVec2<T>& v) const
{
	return (x * v.x + y * v.y);
}

template<typename T>
inline void AEVec2<T>::scale(T v)
{
	x *= v;
	y *= v;
}

template<typename T>
inline void AEVec2<T>::scale(const AEVec2<T>& v)
{
	x *= v.x;
	y *= v.y;
}

template<typename T>
inline T AEVec2<T>::distanceSquared(const AEVec2<T>& v) const
{
	T dx = v.x - x;
	T dy = v.y - y;
	return (dx * dx + dy * dy);
}

template<typename T>
inline T AEVec2<T>::lengthSquared() const
{
	return (x * x + y * y);
}

template<typename T>
inline void AEVec2<T>::negate()
{
	x = -x;
	y = -y;
}

template<typename T>
inline T AEVec2<T>::angle() const
{
	return atan2f(y, x);
}

template<typename T>
inline T AEVec2<T>::cross(const AEVec2<T>& v) const
{
	return x * v.y - y * v.x;
}

template<typename T>
inline AEVec2<T> AEVec2<T>::rotate(const AEVec2<T>& v) const
{
	return AEVec2<T>(x * v.x - y * v.y, x * v.y + y * v.x);
}

template<typename T>
inline AEVec2<T> AEVec2<T>::unrotate(const AEVec2& v) const
{
	return AEVec2<T>(x * v.x + y * v.y, y * v.x - x * v.y);
}

template<typename T>
inline AEVec2<T> AEVec2<T>::perp() const
{
	return AEVec2<T>(-y, x);
}

template<typename T>
inline AEVec2<T> AEVec2<T>::rperp() const
{
	return AEVec2<T>(y, -x);
}

template<typename T>
inline AEVec2<T> AEVec2<T>::middle(const AEVec2<T>& v) const
{
	return AEVec2<T>((x + v.x) / 2.0f, (y + v.y) / 2.0f);
}

template<typename T>
inline AEVec2<T> AEVec2<T>::project(const AEVec2<T>& v) const
{
	return AEVec2<T>(v * (dot(v) / v.dot(v)));
}

template<typename T>
inline AEVec2<T> AEVec2<T>::lerp(const AEVec2<T>& v, T alpha) const
{
	return AEVec2<T>(*this * (1.0f - alpha) + v * alpha);
}

template<typename T>
inline void AEVec2<T>::smooth(const AEVec2<T>& target, T elapsedTime, T responseTime)
{
	if (elapsedTime > 0)
	{
		*this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
	}
}

NS_AE_END
