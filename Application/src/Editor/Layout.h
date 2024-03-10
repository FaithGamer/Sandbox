#pragma once

#include <initializer_list>
#include <map>
#include <string>
#include "Sandbox/Core/std_macros.h"
#include "Sandbox/Core/Vec.h"
#include "Panel.h"

namespace SandboxEditor
{
	class Layout
	{
	public:
		struct PanelLayout
		{
			sptr<Panel> panel;
			bool active = true;
			ImVec2 position;
			ImVec2 size;
		};
		Layout();
		void Init(std::initializer_list<sptr<Panel>> panels);
		void Display();
		void SetPanelActive(std::string panelName, bool active);
		void SwitchPanelActive(std::string panelName);
		void Reset();
		ImVec2 GetDefaultPosition(std::string panelName);
		ImVec2 GetDefaultSize(std::string panelName);
		void OnWindowResized(Sandbox::Vec2u windowSize);
	private:
		std::map<std::string, PanelLayout> m_panels;
		Sandbox::Vec2f m_lastWindowSize;
	};
}