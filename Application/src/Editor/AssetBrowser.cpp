#include "pch.h"
#include "AssetBrowser.h"
#include "SandboxEditorUtils.h"

using namespace Sandbox;
namespace fs = std::filesystem;

namespace SandboxEditor
{

	AssetBrowser::AssetBrowser()
	{
		m_deleteSelection = false;
		TextureImportSettings textureSettings(TextureFiltering::Nearest, TextureWrapping::Clamp, 1, false, false);
		m_fileIcon = makesptr<Texture>("assets/textures/file_icon.png", textureSettings);
		m_directoryIcon = makesptr<Texture>("assets/textures/folder_icon.png", textureSettings);
		m_assetPath = "assets";
		m_currentDirectory = std::filesystem::path(m_assetPath);
	}

	void AssetBrowser::DeleteSelection(InputSignal* input)
	{
		m_deleteSelection = true;
	}

	void AssetBrowser::Display(ImVec2& position, ImVec2& size)
	{
		// to do, how to click within window ?
		//send to recycle bin cross platform

		if (m_deleteSelection && !m_selection.empty())
		{
			bool yes = false;
			if (ShowWarningWindow("Delete: " + m_selection.filename().string() + " ?", yes))
			{
				m_deleteSelection = false;
				if (yes)
				{
					if (fs::exists(m_selection))
						std::filesystem::remove_all(m_selection);
				}
			}
		}

		ImGui::SetNextWindowPos(position);
		ImGui::SetNextWindowSize(size);

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoTitleBar;
		bool open = true;
		ImGui::Begin("Assets", &open, flags);
		ImVec2 contentBrowserSize = ImGui::GetItemRectSize();
		ImGui::SetWindowSize(contentBrowserSize);

		if (ImGui::Button("+"))
		{
			ImGui::OpenPopup("Create");
		}
		if (ImGui::BeginPopup("Create")) // <-- use last item id as popup id
		{
			if (ImGui::Selectable("Folder"))
			{
				CreateFolder(m_currentDirectory, "New_Folder");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (m_currentDirectory != std::filesystem::path(m_assetPath))
		{
			if (ImGui::Button("<"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}
		ImGui::SameLine();
		ImGui::Text(FullDirectoryPath(m_currentDirectory).c_str());

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		int columnCount = (int)(size.x / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());

			//Icon
			sptr<Texture> icon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
			ImVec4 iconColor = { 0, 0, 0, 0 };
			ImGui::PushStyleColor(ImGuiCol_Button, iconColor);
			if (ImGui::ImageButton((ImTextureID)icon->GetId(), { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 }))
			{
				m_selection = path;
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_selection = "";
					m_currentDirectory /= path.filename();
				}
			}

			//Label
			if (m_selection == path)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.5, 0.5, 1, 1 });
				ImGui::TextWrapped(filenameString.c_str());
				ImGui::PopStyleColor();
			}
			else
			{
				ImGui::Text(filenameString.c_str());
			}


			ImGui::NextColumn();
			ImGui::PopID();
		}

		position = ImGui::GetWindowPos();
		size = ImGui::GetWindowSize();

		ImGui::End();

		
	}

	std::string AssetBrowser::GetName()
	{
		return "AssetBrowser";
	}
}