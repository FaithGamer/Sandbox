#pragma once

#include <imgui/imgui.h>

namespace SandboxEditor
{
	class Panel
	{
	public:
		virtual void Display(ImVec2& position, ImVec2& size) = 0;
		virtual std::string GetName() = 0;
	};
}