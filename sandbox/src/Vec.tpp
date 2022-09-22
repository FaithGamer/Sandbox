#include "Vec.h"
#include "Log.h"

namespace sandbox
{
	///////////////////////////////////////////////////
	/// Vec2 //////////////////////////////////////////
	///////////////////////////////////////////////////

	template <class T> template <class U>
	Vec2<T>::Vec2(const Vec3<U>& vec3) : x((T)vec3.x), y((T)vec3.y)
	{

	}

	//////////////////////////
	/// Operators agaisnt Vec2
	//////////////////////////

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator=(const Vec2<U>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator+=(const Vec2<U>& vec)
	{
		x += (T)vec.x;
		y += (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator-=(const Vec2<U>& vec)
	{
		x -= (T)vec.x;
		y -= (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator+(const Vec2<U>& vec)
	{
		return Vec2<T>(x + (T)vec.x, y + (T)vec.y);
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator-(const Vec2<U>& vec)
	{
		return Vec2<T>(x - (T)vec.x, y - (T)vec.y);
	}

	//////////////////////////
	/// Operators agaisnt Vec3
	//////////////////////////

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator=(const Vec3<U>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator+=(const Vec3<U>& vec)
	{
		x += (T)vec.x;
		y += (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator-=(const Vec3<U>& vec)
	{
		x -= (T)vec.x;
		y -= (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator+(const Vec3<U>& vec)
	{
		return Vec2<T>(x + (T)vec.x, y + (T)vec.y);
	}

	template <class T> template <class U>
	Vec2<T>& Vec2<T>::operator-(const Vec3<U>& vec)
	{
		return Vec2<T>(x - (T)vec.x, y - (T)vec.y);
	}

	////////////////////////////
	/// More complex Operations
	////////////////////////////

	template<class T>
	void Vec2<T>::Normalize()
	{
		T magnitude = sqrt(x * x + y * y);
		if (magnitude > 0 || magnitude < 0)
		{
			x /= magnitude;
			y /= magnitude;
		}
		else
		{
			LOG_WARN("Trying to normalize a vector with a magnitude of 0");
		}
	}

	template<class T>
	T Vec2<T>::Magnitude() const
	{
		return sqrt(x * x + y * y);
	}

	template<class T>
	Vec2<T> Vec2<T>::Normalized() const
	{
		T magnitude = sqrt(x * x + y * y);
		if (magnitude > 0 || magnitude < 0)
		{
			return Vec2<T>(x / magnitude, y / magnitude);
		}
		else
		{
			LOG_WARN("Trying to normalize a vector with a magnitude of 0");
			return *this;
		}
	}

	template<class T>
	float Vec2<T>::Dot(const Vec2<T>& vec) const
	{
		return this->x * vec.x + this->y * vec.y;
	}


	///////////////////////////////////////////////
	/// Vec3 //////////////////////////////////////
	///////////////////////////////////////////////

	template <class T> template <class U>
	Vec3<T>::Vec3(const Vec2<U>& vec2) : x((T)vec2.x), y((T)vec2.y), z((T)0)
	{

	}

	//////////////////////////
	/// Operators agaisnt Vec3
	//////////////////////////

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator=(const Vec3<U>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		z = (T)vec.z;
		return *this;
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator+=(const Vec3<U>& vec)
	{
		x += (T)vec.x;
		y += (T)vec.y;
		z += (T)vec.z;
		return *this;
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<U>& vec)
	{
		x -= (T)vec.x;
		y -= (T)vec.y;
		z -= (T)vec.z;
		return *this;
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator+(const Vec3<U>& vec)
	{
		return Vec3<T>(x + (T)vec.x, y + (T)vec.y, z + (T)vec.z);
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator-(const Vec3<U>& vec)
	{
		return Vec3<T>(x + (T)vec.x, y + (T)vec.y, z + (T)vec.z);
	}

	//////////////////////////
	/// Operators agaisnt Vec2
	//////////////////////////

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator=(const Vec2<U>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator+=(const Vec2<U>& vec)
	{
		x += (T)vec.x;
		y += (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator-=(const Vec2<U>& vec)
	{
		x -= (T)vec.x;
		y -= (T)vec.y;
		return *this;
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator+(const Vec2<U>& vec)
	{
		return Vec3<T>(x + (T)vec.x, y + (T)vec.y, z);
	}

	template <class T> template <class U>
	Vec3<T>& Vec3<T>::operator-(const Vec2<U>& vec)
	{
		return Vec3<T>(x - (T)vec.x, y - (T)vec.y, z);
	}

	template<class T>
	void Vec3<T>::Normalize()
	{
		T magnitude = sqrt(x * x + y * y + z * z);
		if (magnitude > 0 || magnitude < 0)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		else
		{
			LOG_WARN("Trying to normalize a vector with a magnitude of 0");
		}
	}

	template<class T>
	T Vec3<T>::Magnitude() const
	{
		return sqrt(x * x + y * y);
	}

	template<class T>
	Vec3<T> Vec3<T>::Normalized() const
	{
		T magnitude = sqrt(x * x + y * y + z * z);
		if (magnitude > 0 || magnitude < 0)
		{
			return Vec3<T>(x / magnitude, y / magnitude, z / magnitude);
		}
		else
		{
			LOG_WARN("Trying to normalize a vector with a magnitude of 0");
			return *this;
		}
	}

	template<class T>
	float Vec3<T>::Dot(const Vec3<T>& vec) const
	{
		return this->x * vec.x + this->y * vec.y + this->z * vec.z;
	}

}