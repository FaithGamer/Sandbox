#include "Vec2.h"

namespace sandbox
{
	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator=(const Vec2<U>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;

		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator+(const Vec2<T>& right)
	{
		return Vec2<T>(x + right.x, y + right.y);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator-(const Vec2<T>& right)
	{
		return Vec2<T>(x - right.x, y - right.y);
	}
}