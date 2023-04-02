#pragma once

#include "ECS/System.h"
#include "Core/Singleton.h"
#include "Containers/Vector.h"
#include "Core/TypeId.h"

namespace Sandbox
{
	struct SystemIdPriority
	{
		System* system;
		int32_t typeId;
		int priority;

		bool operator==(const SystemIdPriority& other)
		{
			return typeId == other.typeId;
		}
		bool operator==(const int32_t other)
		{
			return typeId == other;
		}
	};

	struct CompareSystem
	{
		bool operator()(const SystemIdPriority& l, const SystemIdPriority& r)
		{
			return l.priority < r.priority;
		}
	};

	class Systems : public Singleton<Systems>
	{
		friend sptr<Systems> Singleton<Systems>::Get();
		friend void Singleton<Systems>::Kill();

	public:
		Systems();
		~Systems();

		//To do: ths Push interface is quite messy, find something better
		template <typename T>
		static void Push()
		{
			System* system = new T;
			Systems::Get()->m_pendingSystemIn.push_back(PendingSystem(SystemIdPriority(system, TypeId::GetId<T>(), system->GetPriority())));
		}

		template <typename T>
		static void Remove()
		{
			m_pendingSystemOut.push_back(TypeId::Get<T>());
		}

		template <typename T>
		static bool HasSystem()
		{
			return Systems::Get()->HasSystem(TypeId::Get<T>());
		}

		static Time GetFixedUpdateTime()
		{
			return Systems::Get()->m_fixedUpdateTime;
		}

	private:
		friend void Launch(const EngineParameters& parameters);
		void Update();
		void IntegratePending();
		void RemovePending();
		bool HasSystem(int32_t typeId);

		Clock m_updateClock;
		Clock m_fixedUpdateClock;
		Time m_fixedUpdateAccumulator;
		Time m_fixedUpdateTime;
		int m_maxFixedUpdate;

		std::vector<SystemIdPriority> m_eventSystems;
		std::vector<SystemIdPriority> m_fixedUpdateSystems;
		std::vector<SystemIdPriority> m_updateSystems;
		std::vector<SystemIdPriority> m_imGuiSystems;

		std::vector<SystemIdPriority> m_pendingSystemIn;
		std::vector<int32_t> m_pendingSystemOut;

		SDL_Event m_events;
	};
}