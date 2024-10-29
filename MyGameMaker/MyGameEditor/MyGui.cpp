#include "MyGui.h"

#include "imgui.h"



#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

MyGUI::MyGUI(SDL_Window* window, void* context) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init();
}

MyGUI::~MyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}


void MyGUI::handleEvent(const SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void MyGUI::processEvent(const SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}