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

	private:
		friend void Sandbox::Start();
		void Update()
		{
			SDL_Event event;
			while (SDL_PollEvent(&event) != 0)
			{
				for (auto& eventSystem : m_eventSystems)
				{
					eventSystem->OnEvent(event);
				}
			}
			for (auto& system : m_systems)
			{
				system->Update(0);
			}
		}

		std::vector<uptr<System>> m_systems;
		std::vector<uptr<EventSystem>> m_eventSystems;
	};
}