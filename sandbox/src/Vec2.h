#pragma once 


template<typename T>
class Vec2
{
public:

	Vec2() : x((T)0), y((T)0) {}
	Vec2(T X, T Y) : x(X), y(Y) {}

	T x;
	T y;

	template <typename T>
	Vec2<T>& operator+(const Vec2<T>& right)
	{
		return Vec2<T>(x + right.x; y + right.y);
	}

	template <typename T>
	Vec2<T>& operator-(const Vec2<T>& right)
	{
		return Vec2<T>(x + right.x; y + right.y);
	}

	template <typename T>
	Vec2<T>& operator+(const Vec2<T>& right)
	{
		return Vec2<T>(x + right.x; y + right.y);
	}

};



typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2u;

