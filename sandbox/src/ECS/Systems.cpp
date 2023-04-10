#include "pch.h"

#include "imgui/imgui_impl_sdl2.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/ImGuiLoader.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS/GameWorld.h"

namespace Sandbox
{
	Systems::Systems() :
		m_fixedUpdateTime(0.030f),
		m_pushCount(0),
		m_maxFixedUpdate(10),
		m_events(0)
	{

	}

	Systems::~Systems()
	{
		std::set<SystemIdPriority, CompareSystemId> toDelete;

		for (auto& system : m_eventSystems)
		{
			toDelete.insert(system);
		}
		for (auto& system : m_fixedUpdateSystems)
		{
			toDelete.insert(system);
		}
		for (auto& system : m_updateSystems)
		{
			toDelete.insert(system);
		}
		for (auto& system : m_imGuiSystems)
		{
			toDelete.insert(system);
		}
		for (auto& system : m_pendingSystemIn)
		{
			toDelete.insert(system);
		}

		//Call delete only once per system

		for (auto& system : toDelete)
		{
			delete system.system;
		}


	}

	void Systems::Update()
	{
		if (!m_pendingSystemIn.empty())
			IntegratePending();
		if (!m_pendingSystemOut.empty())
			RemovePending();

		while (SDL_PollEvent(&m_events) != 0)
		{
			if (m_events.type == SDL_QUIT)
			{
				Engine::Stop();
				return;
			}

			bool ImGuiEventHandled = ImGui_ImplSDL2_ProcessEvent(&m_events);
			if (ImGuiEventHandled)
			{
				if(ImGui::GetIO().WantCaptureKeyboard)
					continue;
				if (ImGui::GetIO().WantCaptureMouse)
					continue;
			}

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
				system.system->OnFixedUpdate(m_fixedUpdateTime);
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

		BeginImGui();
		for (auto& system : m_imGuiSystems)
		{
			system.system->OnImGui();
		}
		EndImGui(Window::GetSize());
	}

	void Systems::IntegratePending()
	{
		std::set<SystemIdPriority, CompareSystemPriority> mustCallOnStart;

		for (auto& system : m_pendingSystemIn)
		{
			if (!HasSystem(system.typeId))
			{
				int usedMethodBitmask = system.system->GetUsedMethod();
				if (usedMethodBitmask & System::Method::Update)
				{
					m_updateSystems.push_back(system);
					std::sort(m_updateSystems.begin(), m_updateSystems.end(), CompareSystemPriority());
				}
				if (usedMethodBitmask & System::Method::Event)
				{
					m_eventSystems.push_back(system);
					std::sort(m_eventSystems.begin(), m_eventSystems.end(), CompareSystemPriority());
				}
				if (usedMethodBitmask & System::Method::FixedUpdate)
				{
					m_fixedUpdateSystems.push_back(system);
					std::sort(m_fixedUpdateSystems.begin(), m_fixedUpdateSystems.end(), CompareSystemPriority());
				}
				if (usedMethodBitmask & System::Method::ImGui)
				{
					m_imGuiSystems.push_back(system);
					std::sort(m_imGuiSystems.begin(), m_imGuiSystems.end(), CompareSystemPriority());
				}
				m_allSystems.insert(std::make_pair(system.typeId, system.system));
				mustCallOnStart.insert(system);
			}
			else
			{
				delete system.system;
				LOG_WARN("Trying to push system that already exists."); //To do: add debug name to systems
			}
		}

		//Call OnStart with right priority order
		for (auto system : mustCallOnStart)
		{
			system.system->OnStart();
		}

		m_pendingSystemIn.clear();
	}

	void Systems::RemovePending(std::vector<SystemIdPriority>& systems, int32_t system, std::set<SystemIdPriority, CompareSystemId>& toDelete)
	{
		int64_t index = Vector::FindIndex(systems, system);
		if (index != -1)
		{
			toDelete.insert(systems[index]);
			Vector::RemoveAt(systems, index);
			std::sort(systems.begin(), systems.end(), CompareSystemPriority());
		}
	}

	void Systems::RemovePending()
	{
		std::set<SystemIdPriority, CompareSystemId> toDelete;

		for (auto typeId : m_pendingSystemOut)
		{
			if (HasSystem(typeId))
			{
				RemovePending(m_updateSystems, typeId, toDelete);
				RemovePending(m_fixedUpdateSystems, typeId, toDelete);
				RemovePending(m_eventSystems, typeId, toDelete);
				RemovePending(m_imGuiSystems, typeId, toDelete);
				m_allSystems.erase(typeId);
			}
			else
			{
				LOG_WARN("Trying to remove system that doesn't exists"); //To do: add debug name to systems
			}
		}

		//To do: test to make sure it works properly
		//Call OnRemove only once and in the right order then delete.
		std::set<SystemIdPriority, CompareSystemPriority> toDeleteSorted;
		for (auto& system : toDelete)
		{
			toDeleteSorted.insert(system);
		}
		for (auto& system : toDeleteSorted)
		{
			system.system->OnRemove();
			delete system.system;
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
		return Systems::Instance()->m_fixedUpdateTime;
	}

	GameWorld* Systems::CreateGameWorld()
	{
		return CreateGameWorld("World_" + std::to_string(Instance()->m_worlds.pointers.size()));
	}

	GameWorld* Systems::CreateGameWorld(std::string name)
	{
		//To do error message if twice same name
		GameWorld* world = new GameWorld(name);
		Systems::Instance()->m_worlds.Push(world);
		return world;
	}

	void Systems::DestroyGameWorld(std::string name)
	{
		Systems::Instance()->m_worlds.Destroy(name);
	}

	GameWorld* Systems::GetGameWorld(std::string name)
	{
		return Systems::Instance()->m_worlds.Get(name);
	}

	GameWorld* Systems::GetMainGameWorld()
	{
		//To do, error handling
		return Systems::Instance()->m_worlds.pointers[0];
	}

	std::vector<GameWorld*>& Systems::GetGameWorlds()
	{
		return Systems::Instance()->m_worlds.pointers;
	}

	//////////////
	/// Worlds ///
	//////////////

	void Systems::Worlds::Push(GameWorld* world)
	{
		pointers.emplace_back(world);
		names.emplace_back(world->GetName());
	}

	void Systems::Worlds::Destroy(std::string name)
	{
		int64_t index = Vector::FindIndex(names, name);
		Vector::RemoveAt(names, index);
		delete pointers[index];
		Vector::RemoveAt(pointers, index);
	}

	GameWorld* Systems::Worlds::Get(std::string name)
	{
		int64_t index = Vector::FindIndex(names, name);
		return pointers[index];
	}

}

