#pragma once 

#include <glm/glm.hpp>

namespace Sandbox
{
	template<class T>
	class Vec3;

	template<class T>
	class Vec2
	{
	public:

		Vec2() : x((T)0), y((T)0) {}
		Vec2(T X, T Y) : x(X), y(Y) {}
		template <class U>
		Vec2(const Vec3<U>& vec3);

		T x;
		T y;

		//////////////////////////
		/// Operators agaisnt Vec2
		//////////////////////////

		template <class U>
		Vec2<T>& operator=(const Vec2<U>& vec);

		template <class U>
		Vec2<T>& operator+=(const Vec2<U>& vec);
		template <class U>
		Vec2<T>& operator-=(const Vec2<U>& vec);

		template <class U>
		Vec2<T>& operator+(const Vec2<U>& vec);
		template <class U>
		Vec2<T>& operator-(const Vec2<U>& vec);


		//////////////////////////
		/// Operators agaisnt Vec3
		//////////////////////////

		template <class U>
		Vec2<T>& operator=(const Vec3<U>& vec);

		template <class U>
		Vec2<T>& operator+=(const Vec3<U>& vec);
		template <class U>
		Vec2<T>& operator-=(const Vec3<U>& vec);

		template <class U>
		Vec2<T>& operator+(const Vec3<U>& vec);
		template <class U>
		Vec2<T>& operator-(const Vec3<U>& vec);
		
		////////////////////////////
		/// More complex Operations
		////////////////////////////

		void Normalize();
		T Magnitude() const;
		Vec2<T> Normalized() const;
		float Dot(const Vec2<T>& vec) const;


	};

	template<class T>
	class Vec3
	{
	public:

		Vec3() : x((T)0), y((T)0), z((T)0) {}
		Vec3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
		template <class U>
		Vec3(const Vec2<U>& vec2);

		T x;
		T y;
		T z;

		//////////////////////////
		/// Operators agaisnt Vec3
		//////////////////////////

		template <class U>
		Vec3<T>& operator=(const Vec2<U>& vec);
		
		template <class U>
		Vec3<T>& operator+=(const Vec2<U>& vec);
		template <class U>
		Vec3<T>& operator-=(const Vec2<U>& vec);

		template <class U>
		Vec3<T>& operator+(const Vec2<U>& vec);
		template <class U>
		Vec3<T>& operator-(const Vec2<U>& vec);

		//////////////////////////
		/// Operators agaisnt Vec2
		//////////////////////////
		
		template <class U>
		Vec3<T>& operator=(const Vec3<U>& vec);

		template <class U>
		Vec3<T>& operator+=(const Vec3<U>& vec);
		template <class U>
		Vec3<T>& operator-=(const Vec3<U>& vec);

		template <class U>
		Vec3<T>& operator+(const Vec3<U>& vec);
		template <class U>
		Vec3<T>& operator-(const Vec3<U>& vec);

		////////////////////////////
		/// More complex Operations
		////////////////////////////

		void Normalize();
		T Magnitude() const;
		Vec3<T> Normalized() const;
		float Dot(const Vec3<T>& vec) const;

	};

	typedef glm::vec2 Vec2f;
	typedef glm::i32vec2 Vec2i;
	typedef glm::u32vec2 Vec2u;

	typedef glm::vec3 Vec3f;
	typedef glm::i32vec3 Vec3i;
	typedef glm::u32vec3 Vec3u;

	typedef glm::vec4 Vec4f;

	typedef glm::mat4 Mat4;
	typedef glm::mat3 Mat3;

}
#include "Vec.tpp"
