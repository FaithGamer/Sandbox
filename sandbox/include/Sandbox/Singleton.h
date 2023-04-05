#pragma once
#include "Sandbox/std_macros.h"


namespace Sandbox
{
	class Engine;

	template <typename T>
	class Singleton
	{
	public:
		T& operator= (const T&) = delete;

	protected:
		friend Engine;
		static sptr<T> Get()
		{
			if (m_instance == nullptr)
			{
				m_instance.reset(new T);
			}
			return m_instance;
		}
		static void Kill()
		{
			m_instance.reset();
		}
	
		Singleton() {}
		inline static sptr<T> m_instance = nullptr;

	};
}