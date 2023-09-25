#pragma once

#include "Panel.h"

namespace SandboxEditor
{
	class Hierarchy : public Panel
	{
		void Display(ImVec2& position, ImVec2& size) override;
		std::string GetName() override;
	};
}