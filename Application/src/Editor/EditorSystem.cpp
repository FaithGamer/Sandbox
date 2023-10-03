#include "pch.h"

#include <filesystem>
#include "Sandbox/Log.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Texture.h"
#include <Sandbox/ECS.h>
#include <Sandbox/Render/SpriteRender.h>
#include <Sandbox/Time.h>
#include <Sandbox/Random.h>
#include <Sandbox/Input.h>

#include "EditorSystem.h"
#include "SandboxEditorUtils.h"
#include "AssetBrowser.h"
#include "Menu.h"
#include "Hierarchy.h"
#include "Game.h"

#include <random>

using namespace Sandbox;
namespace fs = std::filesystem;

namespace SandboxEditor
{
	void EditorSystem::CreateEntityTool()
	{
		//Create entity
		auto world = Systems::GetMainWorld();
		ImGui::Begin("Create Entity");
		static int count = 1;
		ImGui::InputInt("Count", &count);
		if (ImGui::Button("Create Entity"))
		{
			auto world = Systems::GetMainWorld();
			if (world == nullptr)
			{
				ImGui::End();
				return;
			}

			for (int i = 0; i < count; i++)
			{
				m_enemySystem->InstanceEnemy();
			}
		}

		//Text label
		Vec3f heroPosition;
		ForEachComponent<Hero, Transform>([&heroPosition](Hero& hero, Transform& transform)
			{
				heroPosition = transform.GetPosition();
			});
		std::string pos = std::to_string(heroPosition.x) + " " + std::to_string(heroPosition.y);
		ImGui::LabelText(pos.c_str(), "Hero Position");
		Vec3f mouse = Systems::GetMainCamera()->ScreenToWorld(GetMousePosition(), Window::GetSize());
		std::string mouseStr = std::to_string(mouse.x) + " " + std::to_string(mouse.y);
		ImGui::LabelText(mouseStr.c_str(), "Mouse Position");
		std::string enttCount = std::to_string(world->GetEntityCount());
		ImGui::LabelText(enttCount.c_str(), "Entity Count");

		ImGui::End();
	}

	void EditorSystem::OnStart()
	{
		m_enemySystem = Systems::Get<EnemySystem>();
		//Layout
		sptr<Menu> menu = makesptr<Menu>();
		menu->SetLayout(&m_layout);
		sptr<Hierarchy> hierarchy = makesptr<Hierarchy>();
		sptr<AssetBrowser> assetBrowser = makesptr<AssetBrowser>();

		m_layout.Init({ menu, hierarchy, assetBrowser });

		//Inputs 
		auto inputMap = Inputs::GetInputMap("InputMap_0");
		inputMap->SetPassThroughImGui(true);
		auto deleteButton = inputMap->CreateButtonInput("Delete");
		deleteButton->AddKey(KeyScancode::Delete);
		deleteButton->signal.AddListener(&AssetBrowser::DeleteSelection, &*assetBrowser);

		auto switchHierarchy = inputMap->CreateButtonInput("SwitchHierarchy");
		auto switchAsset = inputMap->CreateButtonInput("SwitchAsset");

		switchHierarchy->AddKey(KeyScancode::F1);
		switchAsset->AddKey(KeyScancode::F2);

		switchHierarchy->signal.AddListener(&EditorSystem::SwitchPanelHierarchy, this);
		switchAsset->signal.AddListener(&EditorSystem::SwitchPanelAsset, this);

		//Window size signal
		Window::GetResizeSignal()->AddListener(&Layout::OnWindowResized, &m_layout);
	}

	void EditorSystem::OnImGui()
	{
		m_layout.Display();
		CreateEntityTool();
	}

	void EditorSystem::SwitchPanelHierarchy(Sandbox::InputSignal* input)
	{
		m_layout.SwitchPanelActive("Hierarchy");
	}

	void EditorSystem::SwitchPanelAsset(Sandbox::InputSignal* input)
	{
		m_layout.SwitchPanelActive("AssetBrowser");
	}


}