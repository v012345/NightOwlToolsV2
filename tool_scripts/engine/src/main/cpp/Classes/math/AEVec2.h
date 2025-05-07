/************************************************************************** 
    *  @Copyright (c) 2016, A-one Soft, All rights reserved. 
 
    *  @file			: AEVec2.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2016/01/14 
    *  @brief		: 变量 
**************************************************************************/
#ifndef _AEVec2_H_
#define _AEVec2_H_

#include "common/AEMacros.h"

NS_AE_BEGIN

template<typename T>
inline T clampf(T v, T min_inclusive, T max_inclusive)
{
	if (min_inclusive > max_inclusive) std::swap(min_inclusive, max_inclusive);
    return v < min_inclusive ? min_inclusive : v < max_inclusive ? v : max_inclusive;
}

template <typename T>
class AEVec2
{
public:
	/**
		构造一个新向量初始值为0。
	*/
	AEVec2();

	/**
		用指定x, y值构造一个新向量 参数xx是x的值，参数yy是y的值。
	*/
	AEVec2(T x, T y);

	/**
		用指定array构造一个新的向量，array依序包含x、y的值。
	*/
	AEVec2(const T* array);

	/**
		构造方向由点p1指向点p2的向量。
	*/
	AEVec2(const AEVec2<T>& p1, const AEVec2<T>& p2);

	/**
		构造一个新向量，从一个指定向量获取值。
	*/
	AEVec2(const AEVec2<T>& v);

	virtual ~AEVec2();

	/**
		常用操作符重载。
	*/
	inline const AEVec2<T> operator+(const AEVec2<T>& v) const;
	inline AEVec2<T>& operator+=(const AEVec2<T>& v);
	inline const AEVec2<T> operator-(const AEVec2<T>& v) const;
	inline AEVec2<T>& operator-=(const AEVec2<T>& v);
	inline const AEVec2<T> operator-() const;
	inline const AEVec2<T> operator*(T v) const;
	inline AEVec2<T>& operator*=(T v);
	inline const AEVec2<T> operator/(T v) const;
	inline bool operator<(const AEVec2<T>& v) const;
	inline bool operator>=(const AEVec2<T>& v) const;
	inline bool operator>(const AEVec2<T>& v) const;
	inline bool operator<=(const AEVec2<T>& v) const;
	inline bool operator==(const AEVec2<T>& v) const;
	inline bool operator!=(const AEVec2<T>& v) const;

	/**
		设置向量的x分量值为x，y分量值为y。
	*/
	inline void set(T x, T y);

	/**
		给定array序列，依序设置向量各分量的值。
	*/
	void set(const T* array);

	/**
		将指定向量v的值赋给本向量。
	*/
	inline void set(const AEVec2<T>& v);

	/**
		得到p1指向p2的一个有向向量。
	*/
	inline void set(const AEVec2<T>& p1, const AEVec2<T>& p2);

	/**
		向量值置为0。
	*/
	inline void setZero();

    /**
		向量值是否为0。
    */
    inline bool isZero() const;

    /**
		向量加法，加上v向量的值。
    */
	inline void add(const AEVec2<T>& v);

	/**
		本向量减去向量v。
	*/
	inline void subtract(const AEVec2<T>& v);

	/**
		返回本向量和向量v的点积。
	*/
	inline T dot(const AEVec2<T>& v) const;

	/**
		判断两个向量是否相等。
	*/
	bool equals(const AEVec2<T>& v) const;

	/**
		用指定的放缩系数对向量的各分量进行放缩。
	*/
	inline void scale(T v);

	/**
		用scale向量的x分量来放缩本向量的x分量，用scale向量的y分量来放缩本向量的y分量。
	*/
	inline void scale(const AEVec2<T>& v);

    /**
		将向量值限制在指定的[min,max]区间内。
    */
	void clamp(const AEVec2<T>& min, const AEVec2<T>& max);

    /**
		返回本向量到向量v的距离。
    */
	T distance(const AEVec2<T>& v) const;

    /**
		返回本向量到向量v的距离的平方值。
    */
	inline T distanceSquared(const AEVec2<T>& v) const;

    /**
		计算向量的长度并返回。
    */
    T length() const;

    /**
		计算向量长度的平方并返回。
    */
    inline T lengthSquared() const;

    /**
		向量求负。
    */
    inline void negate();

    /**
		求向量的标准化向量。
    */
    void normalize();

	/**
		向量与x轴夹角，单位是弧度。
	*/
	inline T angle() const;

	/**
		返回两个向量的夹角，单位是弧度。
	*/
	T angle(const AEVec2<T>& v) const;

	/**
		计算两个二维向量的叉乘。
	*/
	inline T cross(const AEVec2<T>& v) const;

    /**
		向量绕指定点point旋转angle度，其中angle的单位为弧度。
    */
	void rotate(const AEVec2<T>& point, T angle);

	/**
		绕v向量旋转。
		返回向量的角度为this.angle()+v.angle(), 长度为this.length()*v.length()。
	*/
	inline AEVec2<T> rotate(const AEVec2<T>& v) const;

	/**
		绕v向量旋转前的向量值。
		返回向量的角度为this.angle()+v.angle(), 长度为this.length()*v.length()。
	*/
	inline AEVec2<T> unrotate(const AEVec2<T>& v) const;

	/**
		计算v逆时针旋转90度的垂线–cross(v,perp(v))>=0。
	*/
	inline AEVec2<T> perp() const;

	/**
		计算向量v顺时针旋转90度后的值—cross(v,rperp(v))<=0。
	*/
	inline AEVec2<T> rperp() const;
    
    /**
		用向量计算两点间的中点。
     */
	inline AEVec2<T> middle(const AEVec2<T>& v) const;

	/**
		计算该向量在另一向量other上的投影。
	*/
	inline AEVec2<T> project(const AEVec2<T>& v) const;

    /**
		两个点a和b之间的线性插值。
     */
	inline AEVec2<T> lerp(const AEVec2<T>& v, T alpha) const;

	/**
		平滑更新向量的当前位置，指向目标向量target。
		responseTime定义了平滑时间量，该值越大结果越平滑，相应的延迟时间越长。
		如果希望向量紧跟target向量， 提供一个相对elapsedTime小很多的responseTime值即可。
	*/
	inline void smooth(const AEVec2<T>& target, T elapsedTime, T responseTime);

	/**
		返回两个向量的夹角，单位是弧度。
	*/
	static T angle(const AEVec2<T>& v1, const AEVec2<T>& v2);

	/**
		向量加法，v1加上v2向量的值。
	*/
	static AEVec2<T> add(const AEVec2<T>& v1, const AEVec2<T>& v2);

	/**
		向量v1减去向量v2。
	*/
	static AEVec2<T> subtract(const AEVec2<T>& v1, const AEVec2<T>& v2);

	/**
		将一个点限制在min_inclusive和max_inclusive之间，返回该点的二维向量形式。
	*/
	static AEVec2<T> clamp(const AEVec2<T>& v, const AEVec2<T>& min, const AEVec2<T>& max);

	/**
		返回本向量v1和向量v2的点积。
	*/
	static T dot(const AEVec2<T>& v1, const AEVec2<T>& v2);

	static const AEVec2<T> ZERO;

	// 分向量x，y
	T x, y;

protected:
private:

};

NS_AE_END

#include "AEVec2.inl"

#endif //_AEVec2_H_