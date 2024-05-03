#include "pch.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Sandbox/Internal/ImGuiLoader.h"


namespace Sandbox
{
	void LoadImGui(SDL_Window* sdlWindow, SDL_GLContext sdlGlContext, bool lightTheme)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = NULL;
	//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // Imgui does not control mouse visibility
		
		// Setup Dear ImGui style
		if (!lightTheme)
			ImGui::StyleColorsDark();
		else
			ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(sdlWindow, sdlGlContext);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void ExitImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void BeginImGui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void EndImGui(Vec2u windowSize)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(windowSize.x, windowSize.y);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}