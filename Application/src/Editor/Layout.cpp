#include "pch.h"
#include "Layout.h"
#include <Sandbox/Render/Window.h>
#include <Sandbox/Core/Log.h>

using namespace Sandbox;

#define MAIN_MENU_HEIGHT 19 //to do, change this dynamically

namespace SandboxEditor
{
	Layout::Layout()
	{
		m_lastWindowSize = Window::GetSize();
	}

	void Layout::Init(std::initializer_list<sptr<Panel>> panels)
	{
		for (auto& panel : panels)
		{
			PanelLayout layout;
			layout.active = true;
			layout.panel = panel;
			layout.position = GetDefaultPosition(panel->GetName());
			layout.size = GetDefaultSize(panel->GetName());
			m_panels.insert(std::make_pair(panel->GetName(), layout));
		}
	}

	void Layout::Display()
	{
		for (auto& panelIt : m_panels)
		{
			auto& panel = panelIt.second;
			if (panel.active)
				panel.panel->Display(panel.position, panel.size);
		}
	}

	void Layout::SetPanelActive(std::string panelName, bool active)
	{
		auto panelFind = m_panels.find(panelName);
		if (panelFind == m_panels.end())
		{
			LOG_WARN("SetPanelActive: " + panelName + " doesn't exists.");
			return;
		}
		panelFind->second.active = active;
	}

	void Layout::SwitchPanelActive(std::string panelName)
	{
		auto panelFind = m_panels.find(panelName);
		if (panelFind == m_panels.end())
		{
			LOG_WARN("SetPanelActive: " + panelName + " doesn't exists.");
			return;
		}
		panelFind->second.active = !panelFind->second.active;
	}

	void Layout::Reset()
	{
		for (auto& layout : m_panels)
		{
			layout.second.active = true;
			layout.second.position = GetDefaultPosition(layout.second.panel->GetName());
			layout.second.size = GetDefaultSize(layout.second.panel->GetName());
		}
	}

	ImVec2 Layout::GetDefaultPosition(std::string panelName)
	{
		auto windowSize = Window::GetSize();
		if (panelName == "Menu")
		{
			float x = 0;
			float y = 0;
			return { x, y };
		}
		else if (panelName == "Hierarchy")
		{
			float x = 0;
			float y = MAIN_MENU_HEIGHT;
			return { x, y };
		}
		else if (panelName == "AssetBrowser")
		{
			float x = windowSize.x * 0.25f;
			float y = windowSize.y * 0.75f;
			return { x, y };
		}

		return { windowSize.x * 0.3f, windowSize.y * 0.3f };
	}

	ImVec2 Layout::GetDefaultSize(std::string panelName)
	{
		Vec2f windowSize = Window::GetSize();

		if (panelName == "Menu")
		{
			float x = windowSize.x;
			float y = ImGui::GetFontSize();
			return { x, y };
		}
		else if (panelName == "Hierarchy")
		{
			float x = windowSize.x * 0.25f;
			float y = windowSize.y - MAIN_MENU_HEIGHT;
			return { x, y };
		}
		else if (panelName == "AssetBrowser")
		{
			float x = windowSize.x * 0.75f;
			float y = windowSize.y * 0.25f;
			return { x, y };
		}

		return { windowSize.x * 0.25f, windowSize.y * 0.25f };
	}

	void Layout::OnWindowResized(Vec2u windowSize)
	{
		Vec2f size = windowSize;
		Vec2f ratio(windowSize.x / m_lastWindowSize.x, windowSize.y / m_lastWindowSize.y);
		for (auto& layout : m_panels)
		{
			layout.second.position.x *= ratio.x;
			if (layout.second.position.y != MAIN_MENU_HEIGHT)
				layout.second.position.y *= ratio.y;
			layout.second.size.x *= ratio.x;
			if (layout.second.size.y == m_lastWindowSize.y - MAIN_MENU_HEIGHT)
				layout.second.size.y = size.y - MAIN_MENU_HEIGHT;
			else
				layout.second.size.y *= ratio.y;
		}
		m_lastWindowSize = size;
	}
}