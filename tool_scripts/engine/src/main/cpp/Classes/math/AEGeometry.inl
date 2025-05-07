#include "math/AEGeometry.h"

NS_AE_BEGIN

template<typename T>
inline AESize<T>::AESize()
 : w(0)
 , h(0)
{

}

template<typename T>
inline AESize<T>::AESize(T w, T h)
: w(w)
, h(h)
{

}

template<typename T>
inline AESize<T>::AESize(const AESize<T>& v)
: w(v.w)
, h(v.h)
{

}

template<typename T>
inline AESize<T>::~AESize()
{

}

template<typename T>
inline AESize<T>& AESize<T>::operator=(const AESize<T>& v)
{
	this->w = v.w;
	this->h = v.h;
	return *this;
}

template<typename T>
inline AESize<T> AESize<T>::operator+(const AESize<T>& v) const
{
	return AESize<T>(this->w + v.w, this->h + v.h);
}

template<typename T>
inline AESize<T>& AESize<T>::operator+=(const AESize<T>& v)
{
	this->w += v.w;
	this->h += v.h;
	return *this;
}

template<typename T>
inline AESize<T> AESize<T>::operator-(const AESize<T>& v) const
{
	return AESize<T>(this->w - v.w, this->h - v.h);
}

template<typename T>
inline AESize<T>& AESize<T>::operator-=(const AESize<T>& v)
{
	this->w -= v.w;
	this->h -= v.h;
	return *this;
}

template<typename T>
inline AESize<T> AESize<T>::operator*(T v) const
{
	return AESize<T>(this->w * v, this->h * v);
}

template<typename T>
inline AESize<T> AESize<T>::operator/(T v) const
{
	return AESize<T>(this->w / v, this->h / v);
}

template<typename T>
inline AERect<T>::AERect()
{
	setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

template<typename T>
inline AERect<T>::AERect(T x, T y, T w, T h)
{
	setRect(x, y, w, h);
}

template<typename T>
inline AERect<T>::AERect(const AEVec2<T>& origin, const AESize<T>& size)
{
	setRect(origin.x, origin.y, size.w, size.h);
}

template<typename T>
inline AERect<T>::AERect(const AERect<T>& v)
{
	setRect(v.origin.x, v.origin.y, v.size.w, v.size.h);
}

template<typename T>
inline AERect<T>::~AERect()
{

}

template<typename T>
inline AERect<T>& AERect<T>::operator=(const AERect<T>& v)
{
	setRect(v.origin.x, v.origin.y, v.size.w, v.size.h);
	return *this;
}

template<typename T>
void AERect<T>::setRect(T x, T y, T w, T h)
{
	origin.x = x;
	origin.y = y;

	size.w = w;
	size.h = h;
}

template<typename T>
inline T AERect<T>::getMinX() const
{
	return origin.x;
}

template<typename T>
inline T AERect<T>::getMidX() const
{
	return origin.x + size.w / 2.0f;
}

template<typename T>
inline T AERect<T>::getMaxX() const
{
	return origin.x + size.w;
}

template<typename T>
inline T AERect<T>::getMinY() const
{
	return origin.y;
}

template<typename T>
inline T AERect<T>::getMidY() const
{
	return origin.y + size.h / 2.0f;
}

template<typename T>
inline T AERect<T>::getMaxY() const
{
	return origin.y + size.h;
}

NS_AE_END
