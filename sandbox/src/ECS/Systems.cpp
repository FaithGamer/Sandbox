#include "pch.h"

#include "imgui/imgui_impl_sdl2.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Internal/ImGuiLoader.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS/World.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Input/Mouse.h"

namespace Sandbox
{
	Systems::Systems() :
		m_pushCount(0),
		m_maxFixedUpdate(3),
		m_events(0),
		m_imGuiEnabled(true)
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
		for (auto& system : m_renderSystems)
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
		Time::delta = (float)m_updateClock.Restart();
		float deltaScaled = (float)Time::delta * Time::timeScale;

		//Making sure at least one microseconds has elapsed.
		if (deltaScaled < 0.000001f)
		{
			deltaScaled = 0.000001f;
		}

		if (!m_pendingSystemIn.empty())
			IntegratePending();
		if (!m_pendingSystemOut.empty())
			RemovePending();

		while (SDL_PollEvent(&m_events) != 0)
		{
			HandleWindowEvents(m_events);
			bool imGuiEventHandled = false;
#ifndef SANDBOX_NO_WINDOW
#ifdef SANDBOX_IMGUI
			if (m_imGuiEnabled)
				ImGui_ImplSDL2_ProcessEvent(&m_events);
#endif
#endif
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
		while (m_fixedUpdateAccumulator >= Time::fixedDelta)
		{
			Time scaledFixedDelta = (float)Time::fixedDelta * Time::timeScale;
			m_fixedUpdateAccumulator -= Time::fixedDelta;
			for (auto& system : m_fixedUpdateSystems)
			{
				system.system->OnFixedUpdate(scaledFixedDelta);
			}
			if (++i > m_maxFixedUpdate)
			{
				//To do: decide behaviour
				//breaking here would not make the simulation perfeclty reproductible.
				//Not breaking could lead to crash the application in case of overload
				m_fixedUpdateAccumulator = 0;
				break;
			}
		}

		for (auto& system : m_updateSystems)
		{
			//If less than one microseconds elapse between two call
			//the m_updateClock.Restart increment doesn't accurately describe time passing by.
			
			system.system->OnUpdate(deltaScaled);
		}
#ifndef SANDBOX_NO_WINDOW
		if (m_mainCamera == nullptr)
			return;
		if (!Window::GetRenderWhenMinimized() && Window::GetMinimized())
			return;

		Window::ClearWindow();
		Renderer2D::Instance()->SetRenderTarget(Window::Instance());
		Renderer2D::Instance()->Begin(*m_mainCamera);
		for (auto& system : m_renderSystems)
		{
			system.system->OnRender();
		}
		Renderer2D::Instance()->End();

#ifdef SANDBOX_IMGUI
		if (m_imGuiEnabled)
		{
			BeginImGui();
			for (auto& system : m_imGuiSystems)
			{
				system.system->OnImGui();
			}
			EndImGui(Window::GetSize());
		}
#endif

		Window::RenderWindow();
	}
#endif

	void Systems::HandleWindowEvents(SDL_Event& event)
	{
		if (event.type == SDL_QUIT)
		{
			Engine::Stop();
			return;
		}
		else if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				Window::SetSize((float)event.window.data1, (float)event.window.data2);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				Window::SetSize((float)event.window.data1, (float)event.window.data2);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				Window::Instance()->FocusSignal.SendSignal(true);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				Window::Instance()->FocusSignal.SendSignal(false);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				Window::Instance()->MinimizedSignal.SendSignal(true);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				Window::Instance()->MinimizedSignal.SendSignal(false);
				break;
			}

		}
	}

	void Systems::IntegratePending()
	{
		std::multiset<SystemIdPriority, CompareSystemPriority> mustCallOnStart;

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
				if (usedMethodBitmask & System::Method::Render)
				{
					m_renderSystems.push_back(system);
					std::sort(m_renderSystems.begin(), m_renderSystems.end(), CompareSystemPriority());
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
		for (auto& system : mustCallOnStart)
		{
			system.system->OnStart();
		}

		m_pendingSystemIn.clear();
	}

	void Systems::RemovePending(std::vector<SystemIdPriority>& systems, int32_t system, std::set<SystemIdPriority, CompareSystemId>& toDelete)
	{
		int64_t index = Container::FindIndex(systems, system);
		if (index != -1)
		{
			toDelete.insert(systems[index]);
			Container::RemoveAt(systems, index);
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
				RemovePending(m_renderSystems, typeId, toDelete);
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
		if (Container::Contains(m_updateSystems, typeId) ||
			Container::Contains(m_fixedUpdateSystems, typeId) ||
			Container::Contains(m_eventSystems, typeId) ||
			Container::Contains(m_imGuiSystems, typeId) ||
			Container::Contains(m_renderSystems, typeId))
			return true;

		return false;
	}

	void Systems::EnableImGui(bool enabled)
	{
		Systems::Instance()->m_imGuiEnabled = enabled;
	}

	bool Systems::IsImGuiEnabled()
	{
		return Systems::Instance()->m_imGuiEnabled;
	}

	World* Systems::CreateWorld()
	{
		return CreateWorld("World_" + std::to_string(Instance()->m_worlds.pointers.size()));
	}

	World* Systems::CreateWorld(std::string name)
	{
		//To do error message if twice same name
		World* world = new World(name);
		Systems::Instance()->m_worlds.Push(world);
		return world;
	}

	void Systems::DestroyWorld(std::string name)
	{
		//To do, queue up the destruction process.
		Systems::Instance()->m_worlds.Destroy(name);
	}

	void Systems::SetMainCamera(Camera* camera)
	{
		auto instance = Instance();
		for (auto& world : instance->m_worlds.pointers)
		{
			auto view = world->registry.view<Camera>();
			for (auto& camera : view)
			{
				view.get<Camera>(camera).isMain = false;
			}
		}
		camera->isMain = true;
		instance->m_mainCamera = camera;
	}

	void Systems::SetMainWorld(World* world)
	{
		auto instance = Instance();
		instance->m_worlds.mainWorld = world;
	}

	void Systems::SetFixedUpdateTime(float seconds)
	{
		Time::fixedDelta = seconds;
	}

	void Systems::SetTimeScale(float scale)
	{
		Time::timeScale = scale;
	}

	World* Systems::GetWorld(std::string name)
	{
		return Systems::Instance()->m_worlds.Get(name);
	}

	World* Systems::GetMainWorld()
	{
		//To do, error handling
		return Systems::Instance()->m_worlds.mainWorld;
	}

	std::vector<World*>& Systems::GetWorlds()
	{
		return Systems::Instance()->m_worlds.pointers;
	}

	Camera* Systems::GetMainCamera()
	{
		return Instance()->m_mainCamera;
	}

	Vec2f Systems::GetMouseWorldPos()
	{
		Camera* camera = Instance()->m_mainCamera;
		if (!camera)
		{
			LOG_WARN("Systems::GetMouseWorldPos, no main camera, Vec2f(0, 0) returned");
			return Vec2f(0, 0);
		}
		return camera->ScreenToWorld(GetMousePosition(), Window::GetSize());
	}

	//////////////
	/// Worlds ///
	//////////////

	void Systems::Worlds::Push(World* world)
	{
		pointers.emplace_back(world);
		names.emplace_back(world->GetName());
		if (pointers.size() == 1)
			mainWorld = world;
	}

	void Systems::Worlds::Destroy(std::string name)
	{
		int64_t index = Container::FindIndex(names, name);
		Container::RemoveAt(names, index);
		delete pointers[index];
		Container::RemoveAt(pointers, index);
	}

	World* Systems::Worlds::Get(std::string name)
	{
		int64_t index = Container::FindIndex(names, name);
		return pointers[index];
	}

}

