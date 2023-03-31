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
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			for (auto& eventSystem : m_eventSystems)
			{
				eventSystem->OnEvent(event);
			}
		}

		m_fixedUpdateTimeRemainder += m_fixedUpdateClock.Restart();
		int i = 0;
		while (m_fixedUpdateTimeRemainder >= m_fixedUpdateTime)
		{
			m_fixedUpdateTimeRemainder -= m_fixedUpdateTime;
			for (auto& system : m_systems)
			{
				system->FixedUpdate();
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

		for (auto& system : m_systems)
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

