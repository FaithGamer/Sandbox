#pragma once

template <typename T>
class Singleton
{
public:

	static sptr<T> Get()
	{
		if (m_instance == nullptr)
		{
			m_instance.reset(new T);
		}
		return m_instance;
	}

	T& operator= (const T&) = delete;

	static void Kill()
	{
		m_instance.reset();
	}

protected:
	Singleton() {}
	inline static sptr<T> m_instance = nullptr;

};
