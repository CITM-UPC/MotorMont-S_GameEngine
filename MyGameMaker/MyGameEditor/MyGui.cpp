#include "MyGUI.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <chrono>
#include <algorithm>

MyGUI::MyGUI(SDL_Window* window, void* context)
    : fps_history(100, 0.0f), fps_index(0), current_fps(0.0f), last_time(std::chrono::steady_clock::now()) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();
}

MyGUI::~MyGUI() {
    shutdown();
}

void MyGUI::initialize() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
}

void MyGUI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void MyGUI::updateFPS() {

    frame_counter++;
    if (frame_counter < 100) {
        return;
    }
    frame_counter = 0;

    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed_seconds = current_time - last_time;

    float delta_time = elapsed_seconds.count();
    current_fps = 1.0f / delta_time;

    if (current_fps > 10000.0f) {
        current_fps = 10000.0f;
    }

    fps_history[fps_index] = current_fps;
    fps_index = (fps_index + 1) % fps_history.size();

    last_time = current_time;
}

void MyGUI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Console");
    ImGui::Text("Console output goes here...");
    ImGui::End();

    updateFPS();

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.75f, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.25f, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
    ImGui::Begin("Configuration");

    ImGui::Text("Frames per Second (FPS): %.1f", current_fps);

    float max_fps = *std::max_element(fps_history.begin(), fps_history.end());
    float graph_max_fps = max_fps + 10.0f;

    ImGui::PlotLines("##FPS", fps_history.data(), fps_history.size(), fps_index, nullptr, 0.0f, graph_max_fps, ImVec2(0, 80));

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyGUI::processEvent(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}
