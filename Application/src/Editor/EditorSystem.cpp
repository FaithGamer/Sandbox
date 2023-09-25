#include "pch.h"

#include <filesystem>
#include "Sandbox/Log.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Input/ButtonInput.h"
#include <Sandbox/ECS.h>

#include "EditorSystem.h"
#include "SandboxEditorUtils.h"
#include "AssetBrowser.h"
#include "Menu.h"
#include "Hierarchy.h"

using namespace Sandbox;
namespace fs = std::filesystem;

namespace SandboxEditor
{
	void CreateEntityTool()
	{
		ImGui::Begin("Create Entity");
		if (ImGui::Button("Create Entity"))
		{
			auto world = Systems::GetMainWorld();
			if (world != nullptr)
			{
				world->CreateEntity();
			}
		}
		ImGui::End();
	}
	void EditorSystem::OnStart()
	{
		//Layout
		sptr<Menu> menu = makesptr<Menu>();
		menu->SetLayout(&m_layout);
		sptr<Hierarchy> hierarchy = makesptr<Hierarchy>();
		sptr<AssetBrowser> assetBrowser = makesptr<AssetBrowser>();

		m_layout.Init({ menu, hierarchy, assetBrowser });


		//Inputs 
		auto editorInputMap = Inputs::CreateInputMap();
		editorInputMap->SetPassThroughImGui(true);
		auto deleteButton = editorInputMap->CreateButtonInput("Delete");
		deleteButton->AddKey(KeyScancode::Delete);
		deleteButton->signal.AddListener(&AssetBrowser::DeleteSelection, &*assetBrowser);

		auto switchHierarchy = editorInputMap->CreateButtonInput("SwitchHierarchy");
		auto switchAsset = editorInputMap->CreateButtonInput("SwitchAsset");

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