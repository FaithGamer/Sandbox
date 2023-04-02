#include "pch.h"
#include "Systems.h"

namespace Sandbox
{
	Systems::Systems()
	{
		m_fixedUpdateTime = 0.030f;
	}
	Systems::~Systems()
	{
		for (auto system : m_eventSystems)
		{
			delete system.system;
		}
		for (auto system : m_fixedUpdateSystems)
		{
			delete system.system;
		}
		for (auto system : m_updateSystems)
		{
			delete system.system;
		}
		for (auto system : m_imGuiSystems)
		{
			delete system.system;
		}
		for (auto system : m_pendingSystemIn)
		{
			delete system.system;
		}
	}
	void Systems::Update()
	{
		if(!m_pendingSystemIn.empty())
			IntegratePending();
		if (!m_pendingSystemOut.empty())
			RemovePending();

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
			for (auto& system : m_fixedUpdateSystems)
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
			system.system->OnUpdate(m_updateClock.Restart());
		}

		for (auto& system : m_imGuiSystems)
		{
			//call imgui singleton start and end.
		}
	}

	void Systems::IntegratePending()
	{
		for (auto system : m_pendingSystemIn)
		{
			if (!HasSystem(system.typeId))
			{
				int usedMethodBitmask = system.system->GetUsedMethod();
				if ( usedMethodBitmask & System::Method::Update)
				{
					m_updateSystems.push_back(system);
					std::sort(m_updateSystems.begin(), m_updateSystems.end(), CompareSystem());
				}
				if (usedMethodBitmask & System::Method::Event)
				{
					m_eventSystems.push_back(system);
					std::sort(m_eventSystems.begin(), m_eventSystems.end(), CompareSystem());
				}
				if (usedMethodBitmask & System::Method::FixedUpdate)
				{
					m_fixedUpdateSystems.push_back(system);
					std::sort(m_fixedUpdateSystems.begin(), m_fixedUpdateSystems.end(), CompareSystem());
				}
				if (usedMethodBitmask & System::Method::ImGui)
				{
					m_imGuiSystems.push_back(system);
					std::sort(m_imGuiSystems.begin(), m_imGuiSystems.end(), CompareSystem());
				}
			}
			else
			{
				LOG_WARN("Trying to push system that already exists."); //To do: add debug name to systems
			}
		}
		m_pendingSystemIn.clear();
	}

	void Systems::RemovePending()
	{
		for (auto system : m_pendingSystemOut)
		{
			if (HasSystem(system))
			{
				if (Vector::Remove(m_updateSystems, system) > 0)
				{
					std::sort(m_updateSystems.begin(), m_updateSystems.end(), CompareSystem());
				}
				if (Vector::Remove(m_eventSystems, system) > 0)
				{
					std::sort(m_eventSystems.begin(), m_eventSystems.end(), CompareSystem());
				}
				if (Vector::Remove(m_fixedUpdateSystems, system) > 0)
				{
					std::sort(m_fixedUpdateSystems.begin(), m_fixedUpdateSystems.end(), CompareSystem());
				}
				if (Vector::Remove(m_imGuiSystems, system) > 0)
				{
					std::sort(m_imGuiSystems.begin(), m_imGuiSystems.end(), CompareSystem());
				}
			}
			else
			{
				LOG_WARN("Trying to remove system that doesn't exists"); //To do: add debug name to systems
			}
		}

		m_pendingSystemOut.clear();
	}

	bool Systems::HasSystem(int32_t typeId)
	{
		if (Vector::Contains(m_updateSystems, typeId) ||
			Vector::Contains(m_fixedUpdateSystems, typeId) ||
			Vector::Contains(m_eventSystems, typeId) ||
			Vector::Contains(m_imGuiSystems, typeId))
			return true;

		return false;
	}

	Time Systems::GetFixedUpdateTime()
	{
		return Systems::Get()->m_fixedUpdateTime;
	}
}

