#pragma once

#include "Panel.h"
#include "Layout.h"

namespace SandboxEditor
{
	class Menu : public Panel
	{
	public:
		Menu();
		void Display(ImVec2& position, ImVec2& size) override;
		std::string GetName() override;

		void SetLayout(Layout* layout);
	private:
		Layout* m_layout;
	};
}