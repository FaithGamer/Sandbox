#include "pch.h"
#include "Sandbox/System/Systems.h"
#include "Sandbox/System/System.h"
#include "Sandbox/Time.h"
#include "Sandbox/Engine.h"

class MySytem : public Sandbox::System
{
public:
	MySytem() : m_count (0), m_sum(0) {}

	void Update(Sandbox::Time time)
	{
	
		//std::cout << m_timer << std::endl;
		m_timer += time;
		if (m_timer >= 1.f)
		{
			Sandbox::Engine::Stop();
			std::cout << "Average: " << m_sum / (float)m_count << std::endl;
		}
		
	}
	void FixedUpdate()
	{
		m_count++;
		m_sum += m_clock.GetElapsed();
		std::cout << "A:" << m_clock.Restart() << std::endl;
	}

private:
	Sandbox::Clock m_clock;
	Sandbox::Time m_timer;
	float m_sum;
	int m_count;
};

void SystemTest()
{

	Sandbox::Systems::Push<MySytem>();
	Sandbox::Clock t;

	Sandbox::Engine::Launch();
	std::cout << t.GetElapsed() << std::endl;

}