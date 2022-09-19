#pragma once 

namespace sandbox
{
	template<class T>
	class Vec2
	{
	public:

		Vec2() : x((T)0), y((T)0) {}
		Vec2(T X, T Y) : x(X), y(Y) {}

		T x;
		T y;

		template <class U>
		Vec2<T>& operator=(const Vec2<U>& vec);
		Vec2<T>& operator+(const Vec2<T>& right);
		Vec2<T>& operator-(const Vec2<T>& right);

	};

	typedef Vec2<double> Vec2d;
	typedef Vec2<int> Vec2i;
	typedef Vec2<unsigned int> Vec2u;


}
#include "Vec2.tpp"
