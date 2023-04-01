#include "pch.h"
#include "Systems.h"

namespace Sandbox
{
	Systems::Systems()
	{
		m_fixedUpdateTime = 0.030f;
	}
	void Systems::Update()
	{
		IntegratePending();
		while (SDL_PollEvent(&m_events) != 0)
		{
			for (auto& eventSystem : m_eventSystems)
			{
				if (eventSystem.system->OnEvent(m_events))
				{
					break;
				}
			}
		}

		m_fixedUpdateAccumulator += m_fixedUpdateClock.Restart();
		int i = 0;
		while (m_fixedUpdateAccumulator >= m_fixedUpdateTime)
		{
			m_fixedUpdateAccumulator -= m_fixedUpdateTime;
			for (auto& system : m_fixedUpdateSystem)
			{
				system.system->OnFixedUpdate();
			}
			if (++i > m_maxFixedUpdate)
			{
				//To do: decide behaviour
				//breaking here would not make the simulation perfeclty reproductible.
				//Not breaking could lead to crash the application in case of overload
			}
		}

		//Making sure at least one microseconds has elapsed.
		if (m_updateClock.GetElapsed() < 0.000001f)
		{
			return;
		}

		for (auto& system : m_updateSystems)
		{
			//If less than one microseconds elapse between two call
			//the m_updateClock.Restart increment doesn't accurately describe time passing by.
			system->Update(m_updateClock.Restart());
		}
	}

	float Systems::GetFixedUpdateTime() const
	{
		return m_fixedUpdateTime;
	}
}

