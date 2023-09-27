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

#include "EditorSystem.h"
#include "SandboxEditorUtils.h"
#include "AssetBrowser.h"
#include "Menu.h"
#include "Hierarchy.h"

#include <random>

using namespace Sandbox;
namespace fs = std::filesystem;

int Rng(int min, int max)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> dist(min, max);
	return dist(generator);
}
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
				float x = Rng(-50, 50);
				float y = Rng(-50, 50);

				auto entity = world->CreateEntity();
				auto transform = entity->AddComponent<Transform>();
				transform->SetPosition({ x, y, 0 });

				auto render = entity->AddComponent<SpriteRender>();
				render->SetSprite(m_entitySprite);

				//render->SetLayer(Renderer2D::Instance()->GetLayerId("320x180"));
			}
		}
		ImGui::End();
	}
	void EditorSystem::OnUpdate(Time deltaTime)
	{
		ForEachComponent<Transform>([this, deltaTime](Transform& transform) {
			auto position = transform.GetPosition();
			position.x += (float)deltaTime * m_direction.x * 10;
			position.y += (float)deltaTime * m_direction.y * 10;
			m_position = position;
			transform.SetPosition(position);
			});
	}
	void EditorSystem::OnStart()
	{
		Renderer2D::Instance()->AddLayer("320x180");
		auto niceguy = makesptr<Texture>("assets/textures/nice_guy_head.png", TextureImportSettings(TextureFiltering::Nearest, TextureWrapping::Clamp, false, false));
		m_entitySprite = makesptr<Sprite>(niceguy);
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

		auto moveSprite = editorInputMap->CreateDirectionalInput("MoveSprite");
		std::vector<DirectionalButton> buttons
		{
			DirectionalButton(KeyScancode::W, Vec2f(0, 1)),
			DirectionalButton(KeyScancode::A, Vec2f(-1, 0)),
			DirectionalButton(KeyScancode::S, Vec2f(0, -1)),
			DirectionalButton(KeyScancode::D, Vec2f(1, 0)),
		};
		moveSprite->AddButtons(buttons);
		moveSprite->signal.AddListener(&EditorSystem::OnMove, this);

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

	void EditorSystem::OnMove(Sandbox::InputSignal* input)
	{
		m_direction = input->GetVec2f();
	}
}