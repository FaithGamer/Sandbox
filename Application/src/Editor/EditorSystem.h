#pragma once
#include <filesystem>
#include <imgui/imgui.h>

#include "Sandbox/ECS/System.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Layout.h"

namespace SandboxEditor
{
	class EditorSystem : public Sandbox::System
	{
	public:
		void OnStart();
		void OnImGui();

		void SwitchPanelHierarchy(Sandbox::InputSignal* input);
		void SwitchPanelAsset(Sandbox::InputSignal* input);
	private:
		Layout m_layout;
	};
}