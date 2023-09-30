#include "pch.h"

#include <filesystem>
#include "Sandbox/Log.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Input/Inputs.h"
#include <Sandbox/Input/Input.h>
#include <Sandbox/Input/ButtonInput.h>
#include <Sandbox/Input/DirectionalInput.h>
#include <Sandbox/ECS.h>
#include <Sandbox/Render/SpriteRender.h>
#include <Sandbox/Time.h>
#include <Sandbox/Random.h>

#include "EditorSystem.h"
#include "SandboxEditorUtils.h"
#include "AssetBrowser.h"
#include "Menu.h"
#include "Hierarchy.h"
#include "Game.h"

#include <random>

using namespace Sandbox;
namespace fs = std::filesystem;

struct Mover
{
	Vec2f direction;
	Clock clock;
	Time time;
};


namespace SandboxEditor
{
	void EditorSystem::CreateEntityTool()
	{
		ImGui::Begin("Create Entity");
		static int count = 1;
		ImGui::InputInt("Count", &count);
		std::string pos = std::to_string(m_position.x) + " " + std::to_string(m_position.y);
		std::string dir = std::to_string(m_direction.x) + " " + std::to_string(m_direction.y);

		ImGui::Text(pos.c_str());
		ImGui::Text(dir.c_str());
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