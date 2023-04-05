#pragma once

#include <set>

#include "Sandbox/System/System.h"
#include "Sandbox/Singleton.h"
#include "Sandbox/Vector.h"
#include "Sandbox/TypeId.h"


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

	struct CompareSystemPriority
	{
		bool operator()(const SystemIdPriority& l, const SystemIdPriority& r) const
		{
			return l.priority > r.priority;
		}
	};

	struct CompareSystemId
	{
		bool operator()(const SystemIdPriority& l, const SystemIdPriority& r) const
		{
			return l.typeId < r.typeId;
		}
	};

	class Systems : public Singleton<Systems>
	{

		friend sptr<Systems> Singleton<Systems>::Get();
		friend void Singleton<Systems>::Kill();

	public:
		~Systems();
		//To do: ths Push interface is quite messy, find something better
		template <typename T>
		static void Push()
		{
			System* system = new T;
			if (system->GetPriority() == 0)
			{
				//Priority based on push order
				system->SetPriority(-(++Systems::Get()->m_pushCount));
			}
			Systems::Get()->m_pendingSystemIn.push_back(SystemIdPriority(system, TypeId::GetId<T>(), system->GetPriority()));
		}

		template <typename T>
		static void Remove()
		{
			m_pendingSystemOut.push_back(TypeId::GetId<T>());
		}

		template <typename T>
		static bool HasSystem()
		{
			return Systems::Get()->HasSystem(TypeId::GetId<T>());
		}

		static Time GetFixedUpdateTime();

	private:
		friend Engine;
		Systems();

	

		void Update();
		void IntegratePending();
		void RemovePending();
		void RemovePending(std::vector<SystemIdPriority>& systems, int32_t system, std::set<SystemIdPriority, CompareSystemId>& toDelete);
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

		int m_pushCount;
	};
}