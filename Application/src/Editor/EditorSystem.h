#pragma once
#include <filesystem>
#include <imgui/imgui.h>

#include "Sandbox/ECS/System.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Sprite.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Layout.h"

namespace SandboxEditor
{
	class EditorSystem : public Sandbox::System
	{
	public:
		void OnUpdate(Sandbox::Time deltaTime);
		void OnStart();
		void OnImGui();

		void SwitchPanelHierarchy(Sandbox::InputSignal* input);
		void SwitchPanelAsset(Sandbox::InputSignal* input);
		void OnMove(Sandbox::InputSignal* input);
	private:
		void CreateEntityTool();
		Layout m_layout;
		Sandbox::Vec2f m_position;
		Sandbox::Vec2f m_direction;
		sptr<Sandbox::Sprite> m_entitySprite;
	};
}