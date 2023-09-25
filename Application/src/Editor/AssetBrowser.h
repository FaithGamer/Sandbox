#pragma once

#include <filesystem>
#include <imgui/imgui.h>
#include "Sandbox/Input/Input.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/std_macros.h"
#include "Panel.h"

namespace SandboxEditor
{
	class AssetBrowser : public Panel
	{
	public:

		AssetBrowser();
		void Display(ImVec2& position, ImVec2& size);
		std::string GetName();
		void DeleteSelection(Sandbox::InputSignal* input);

	private:

		sptr<Sandbox::Texture> m_fileIcon;
		sptr<Sandbox::Texture> m_directoryIcon;
		std::string m_assetPath;
		std::filesystem::path m_currentDirectory;
		std::filesystem::path m_selection;
		bool m_deleteSelection;
	};
}
