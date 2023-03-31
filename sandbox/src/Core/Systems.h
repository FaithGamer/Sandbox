#pragma once

#include "ECS/System.h"
#include "Core/Singleton.h"
#include "Containers/Vector.h"

namespace Sandbox
{
	struct SystemId
	{
		int32_t typeId;
		uptr<System> system;
	};
	struct EventSystemId
	{
		int32_t typeId;
		uptr<EventSystem> system;
	};

	class Systems : public Singleton<Systems>
	{
		friend sptr<Systems> Singleton<Systems>::Get();
		friend void Singleton<Systems>::Kill();

	public:
		Systems();

		template <typename T>
		static void PushSystem()
		{

			Systems::Get()->m_systems.push_back(makeuptr<T>());
		}


		float GetFixedUpdateTime() const;

	private:
		friend void Launch();
		void Update();

		Clock m_updateClock;
		Clock m_fixedUpdateClock;
		Time m_fixedUpdateTimeRemainder;
		Time m_fixedUpdateTime;
		int m_maxFixedUpdate;
		std::vector<uptr<System>> m_systems;
		std::vector<uptr<EventSystem>> m_eventSystems;
	};
}