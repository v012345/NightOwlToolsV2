#include "math/AEGeometry.h"

NS_AE_BEGIN

template<typename T>
const AESize<T> AESize<T>::ZERO(0.0f, 0.0f);

template<typename T>
void AESize<T>::set(T w, T h)
{
	this->w = w;
	this->h = h;
}

template<typename T>
bool AESize<T>::equals(const AESize<T>& target) const
{
	return (fabs(this->w - target.w) < FLT_EPSILON) && (fabs(this->h - target.h) < FLT_EPSILON);
}

template<typename T>
const AERect<T> AERect<T>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

template<typename T>
bool AERect<T>::equals(const AERect<T>& v) const
{
	return (origin.equals(v.origin) && size.equals(v.size));
}

template<typename T>
bool AERect<T>::containsPoint(const AEVec2<T>& point) const
{
	return !(point.x < getMinX() || point.x > getMaxX() || point.y < getMinY() || point.y > getMaxY());
}

template<typename T>
bool AERect<T>::containsRect(const AERect<T>& v) const
{
	return !(v.getMinX() < getMinX() || v.getMaxX() > getMaxX() || v.getMinY() < getMinY() || v.getMaxY() > getMaxY());
}

template<typename T>
bool AERect<T>::intersectsRect(const AERect<T>& v) const
{
	return !(getMaxX() < v.getMinX() || getMinX() > v.getMaxX() || getMaxY() < v.getMinY() || getMinY() > v.getMaxY());
}

/*
template<typename T>
bool AERect<T>::containsRect(const AERect<T>& v1, const AERect<T>& v2)
{
	return !(v2.getMinX() < v1.getMinX() || v2.getMaxX() > v1.getMaxX() || v2.getMinY() < v1.getMinY() || v2.getMaxY() > v1.getMaxY());
}
*/

/*
template<typename T>
bool AERect<T>::intersectsRect(const AERect<T>& v1, const AERect<T>& v2)
{
	return !(v1.getMaxX() < v2.getMinX() || v1.getMinX() > v2.getMaxX() || v1.getMaxY() < v2.getMinY() || v1.getMinY() > v2.getMaxY());
}
*/

/*
template<typename T>
AERect<T> AERect<T>::intersect(const AERect<T>& v1, const AERect<T>& v2)
{
	T x = max(v1.getMinX(), v2.getMinX());
	T y = max(v1.getMinY(), v2.getMinY());
	return AERect<T>(x, y, min(v1.getMaxX(), v2.getMaxX()) - x, min(v1.getMaxY(), v2.getMaxY()) - y);
}
*/

NS_AE_END
