#include "math/AEVec2.h"

NS_AE_BEGIN

template<typename T>
const AEVec2<T> AEVec2<T>::ZERO(0.0f, 0.0f);

template<typename T>
void AEVec2<T>::set(const T* array)
{
	AEASSERT(array, "array is nil !");

	x = array[0];
	y = array[1];
}

template<typename T>
bool AEVec2<T>::equals(const AEVec2<T>& v) const
{
	return (fabs(this->x - v.x) < FLT_EPSILON) && (fabs(this->y - v.y) < FLT_EPSILON);
}

template<typename T>
void AEVec2<T>::clamp(const AEVec2<T>& min, const AEVec2<T>& max)
{
	AEASSERT(!(min.x > max.x || min.y > max.y), "min & max error !");

	if (x < min.x) x = min.x;
	if (x > max.x) x = max.x;
	if (y < min.y) y = min.y;
	if (y > max.y) y = max.y;
}

template<typename T>
AEVec2<T> AEVec2<T>::clamp(const AEVec2<T>& v, const AEVec2<T>& min, const AEVec2<T>& max)
{
	AEASSERT(!(min.x > max.x || min.y > max.y), "min & max error !");

	T x = v.x;
	if (x < min.x) x = min.x;
	if (x > max.x) x = max.x;
	T y = v.y;
	if (y < min.y) y = min.y;
	if (y > max.y) y = max.y;

	return AEVec2<T>(x, y);
}

template<typename T>
T AEVec2<T>::distance(const AEVec2<T>& v) const
{
	return sqrt(distanceSquared(v));
}

template<typename T>
T AEVec2<T>::length() const
{
	return sqrt(lengthSquared());
}

template<typename T>
void AEVec2<T>::normalize()
{
	float n = x * x + y * y;
	if (n == 1.0f) return;

	n = sqrt(n);
	if (n < 2e-37f) return;

	n = 1.0f / n;
	x *= n;
	y *= n;
}

template<typename T>
T AEVec2<T>::angle(const AEVec2<T>& v) const
{
	return AEVec2<T>::angle(*this, v);
}

template<typename T>
T AEVec2<T>::angle(const AEVec2<T>& v1, const AEVec2<T>& v2)
{
	T dz = v1.x * v2.y - v1.y * v2.x;
	return atan2f(fabsf(dz) + 1.0e-37f, dot(v1, v2));
}

template<typename T>
void AEVec2<T>::rotate(const AEVec2<T>& point, T angle)
{
	*this = point + (*this - point).rotate(AEVec2<T>(cosf(angle), sinf(angle)));
}

template<typename T>
AEVec2<T> AEVec2<T>::add(const AEVec2<T>& v1, const AEVec2<T>& v2)
{
	return AEVec2<T>(v1.x + v2.x, v1.y + v2.y);
}

template<typename T>
AEVec2<T> AEVec2<T>::subtract(const AEVec2<T>& v1, const AEVec2<T>& v2)
{
	return AEVec2<T>(v1.x - v2.x, v1.y - v2.y);
}

template<typename T>
T AEVec2<T>::dot(const AEVec2<T>& v1, const AEVec2<T>& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

NS_AE_END
