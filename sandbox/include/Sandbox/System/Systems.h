#pragma once

#include <set>

#include "Sandbox/System/System.h"
#include "Sandbox/Singleton.h"
#include "Sandbox/Vector.h"
#include "Sandbox/TypeId.h"


namespace Sandbox
{
	class GameWorld;

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

		friend sptr<Systems> Singleton<Systems>::Instance();
		friend void Singleton<Systems>::Kill();

	public:
		
		~Systems();

		template <typename SystemType, typename... Args>
		static void Push(Args&&... args)
		{
			System* system = new SystemType(std::forward<Args>(args)...);
			if (system->GetPriority() == 0)
			{
				//Priority based on push order
				system->SetPriority(-(++Systems::Instance()->m_pushCount));
			}
			Systems::Instance()->m_pendingSystemIn.push_back(SystemIdPriority(system, TypeId::GetId<SystemType>(), system->GetPriority()));
		}
		template <typename SystemType>
		static void Push()
		{
			System* system = new SystemType;
			if (system->GetPriority() == 0)
			{
				//Priority based on push order
				system->SetPriority(-(++Systems::Instance()->m_pushCount));
			}
			Systems::Instance()->m_pendingSystemIn.push_back(SystemIdPriority(system, TypeId::GetId<SystemType>(), system->GetPriority()));
		}
		template <typename SystemType>
		static System* Get()
		{
			int32_t typeId = TypeId::GetId<SystemType>();
			auto system = m_allSystems.find(typeId);
			if (system == m_allSystems.end())
				return nullptr;
			return system->second;
		}
		template <typename SystemType>
		static void Remove()
		{
			m_pendingSystemOut.push_back(TypeId::GetId<SystemType>());
		}
		template <typename SystemType>
		static bool HasSystem()
		{
			return Systems::Instance()->HasSystem(TypeId::GetId<SystemType>());
		}

		static Time GetFixedUpdateTime();

		static GameWorld* CreateGameWorld();
		static GameWorld* CreateGameWorld(std::string name);
		static void DestroyGameWorld(std::string name);
		static GameWorld* GetGameWorld(std::string name);
		static std::vector<GameWorld*>& GetGameWorlds();
	private:

		struct Worlds
		{
			void Push(GameWorld* world);
			void Destroy(std::string name);
			GameWorld* Get(std::string name);
			std::vector<GameWorld*> pointers;
			std::vector<std::string> names;
		};
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

		Worlds m_worlds;
		std::unordered_map<int32_t, System*> m_allSystems;


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