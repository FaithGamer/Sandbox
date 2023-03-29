#pragma once

#include "ECS/System.h"
#include "Core/Singleton.h"

namespace Sandbox
{
	class Systems : public Singleton<Systems>
	{
		friend sptr<Systems> Singleton<Systems>::Get();
		friend void Singleton<Systems>::Kill();

	public:
		Systems();

		template <typename T>
		void PushSystem()
		{
			m_systems.push_back(makeuptr<T>());
		}
		template <typename T>
		void PushEventSystem()
		{
			m_eventSystems.push_back(makeuptr<T>());
		}

		float GetFixedUpdateTime() const;

	private:
		friend void Launch();
		void Update();

		Clock m_updateClock;
		Clock m_fixedUpdateClock;
		Time m_fixedUpdateTimeRemainder;
		Time m_fixedUpdateTime;
		std::vector<uptr<System>> m_systems;
		std::vector<uptr<EventSystem>> m_eventSystems;
	};
}