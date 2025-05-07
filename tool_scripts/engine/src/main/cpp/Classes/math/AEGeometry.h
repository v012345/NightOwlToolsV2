/************************************************************************** 
    *  @Copyright (c) 2016, A-one Soft, All rights reserved. 
 
    *  @file			: AEGeometry.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2016/01/14 
    *  @brief		: 几何相关计算信息
**************************************************************************/
#ifndef _AEGeometry_H_
#define _AEGeometry_H_

#include "platform/AEStdC.h"
#include "math/AEVec2.h"

NS_AE_BEGIN;

template<typename T>
class AESize
{
public:
	AESize();
	AESize(T w, T h);
	AESize(const AESize<T>& v);
	virtual ~AESize();

	/**
		常用操作符重载。
	*/
	inline AESize<T>& operator=(const AESize<T>& v);
	inline AESize<T> operator+(const AESize<T>& v) const;
	inline AESize<T>& operator+=(const AESize<T>& v);
	inline AESize<T> operator-(const AESize<T>& v) const;
	inline AESize<T>& operator-=(const AESize<T>& v);
	inline AESize<T> operator*(T v) const;
	inline AESize<T> operator/(T v) const;

	/**
		设置w值为w，h值为h。
	*/
	void set(T w, T h);

	/**
		判断本大小与target是否相等。
	*/
	bool equals(const AESize<T>& target) const;
	
	// 空大小
	static const AESize<T> ZERO;

	T w, h;

protected:
private:

};

template<typename T>
class AERect
{
public:
	AERect();
	AERect(T x, T y, T w, T h);
	AERect(const AEVec2<T>& origin, const AESize<T>& size);
	AERect(const AERect<T>& v);
	virtual ~AERect();

	/**
		常用操作符重载。
	*/
	AERect<T>& operator=(const AERect<T>& v);

	/**
		设置矩形信息。
	*/
	inline void setRect(T x, T y, T w, T h);

	/**
		获取矩形左x。
	*/
	inline T getMinX() const;

	/**
		获取矩形中间x。
	*/
	inline T getMidX() const;

	/**
		获取矩形右x。
	*/
	inline T getMaxX() const;

	/**
		获取矩形下y。
	*/
	inline T getMinY() const;

	/**
		获取矩形中间y。
	*/
	inline T getMidY() const;

	/**
		获取矩形上y。
	*/
	inline T getMaxY() const;

	/**
		判断该矩形与矩形v是否相等。
	*/
	bool equals(const AERect<T>& v) const;

	/**
		该矩形是否包含point。
	*/
	bool containsPoint(const AEVec2<T>& point) const;

	/**
		该矩形是否包含矩形v。
	*/
	bool containsRect(const AERect<T>& v) const;

	/**
		该矩形是否与矩形v交叉。
	*/
	bool intersectsRect(const AERect<T>& v) const;

	/**
		矩形v1是否包含矩形v2。
	*/
	static bool containsRect(const AERect<T>& v1, const AERect<T>& v2)
	{
		return !(v2.getMinX() < v1.getMinX() || v2.getMaxX() > v1.getMaxX() || v2.getMinY() < v1.getMinY() || v2.getMaxY() > v1.getMaxY());
	}

	/**
		矩形v1是否与矩形v2交叉。
	*/
	static bool intersectsRect(const AERect<T>& v1, const AERect<T>& v2)
	{
		return !(v1.getMaxX() < v2.getMinX() || v1.getMinX() > v2.getMaxX() || v1.getMaxY() < v2.getMinY() || v1.getMinY() > v2.getMaxY());
	}

	/**
		矩形v1是否与矩形v2交叉。
	*/
	static AERect<T> intersect(const AERect<T>& v1, const AERect<T>& v2)
	{
		T x = max(v1.getMinX(), v2.getMinX());
		T y = max(v1.getMinY(), v2.getMinY());
		return AERect<T>(x, y, min(v1.getMaxX(), v2.getMaxX()) - x, min(v1.getMaxY(), v2.getMaxY()) - y);
	}

	// 基本矩形
	static const AERect<T> ZERO;

	// 矩形左下角坐标
	AEVec2<T> origin;
	// 矩形大小
	AESize<T>  size;

protected:
private:

};

NS_AE_END

#include "AEGeometry.inl"

#endif //_AEGeometry_H_