#include "pch.h"

#include <filesystem>
#include "Sandbox/Log.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Input/ButtonInput.h"

#include "EditorSystem.h"
#include "SandboxEditorUtils.h"
#include "AssetBrowser.h"
#include "Menu.h"
#include "Hierarchy.h"

using namespace Sandbox;
namespace fs = std::filesystem;

namespace SandboxEditor
{
	void EditorSystem::OnStart()
	{
		//Layout
		sptr<Menu> menu = makesptr<Menu>();
		sptr<Hierarchy> hierarchy = makesptr<Hierarchy>();
		sptr<AssetBrowser> assetBrowser = makesptr<AssetBrowser>();

		m_layout.Init({ menu, hierarchy, assetBrowser });


		//Inputs 
		auto editorInputMap = Inputs::CreateInputMap();
		editorInputMap->SetPassThroughImGui(true);
		auto deleteButton = editorInputMap->CreateButtonInput("Delete");
		deleteButton->AddKey(KeyScancode::Delete);
		deleteButton->signal.AddListener(&AssetBrowser::DeleteSelection, &*assetBrowser);

		Window::GetResizeSignal()->AddListener(&Layout::OnWindowResized, &m_layout);

	}

	void EditorSystem::OnImGui()
	{
		m_layout.Display();
	}
}