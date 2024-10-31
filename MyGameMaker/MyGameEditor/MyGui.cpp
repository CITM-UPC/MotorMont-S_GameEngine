#include "MyGUI.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "../MyGameEngine/GameObject.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <SDL2/SDL.h>
#include <cstdlib>

bool showConsole = true;
bool showHierarchy = true;
bool showConfiguration = true;
bool showInspector = true;
bool showAbout = false;

MyGUI::MyGUI(SDL_Window* window, void* context)
    : fps_history(100, 0.0f), memory_usage_history(100, 0.0f),
    fps_index(0), memory_index(0),
    current_fps(0.0f), current_memory_usage(0.0f),
    last_time(std::chrono::steady_clock::now()),
    selectedGameObjectIndex(-1)
{
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

void MyGUI::updateMemoryUsage() {
#ifdef __linux__
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long long totalMemory = memInfo.totalram;
    totalMemory *= memInfo.mem_unit;
    long long freeMemory = memInfo.freeram;
    freeMemory *= memInfo.mem_unit;
    current_memory_usage = (float)(totalMemory - freeMemory) / (1024.0f * 1024.0f); // Convertimos a MB

#elif defined(_WIN32)
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    current_memory_usage = (float)(memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024.0f * 1024.0f); // Convertimos a MB
#endif

    memory_usage_history[memory_index] = current_memory_usage;
    memory_index = (memory_index + 1) % memory_usage_history.size();
}


void MyGUI::updateFPS() {

    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed_seconds = current_time - last_time;

    float delta_time = elapsed_seconds.count();
    current_fps = 1.0f / delta_time;


    fps_history[fps_index] = current_fps;
    fps_index = (fps_index + 1) % fps_history.size();

    last_time = current_time;
}

void MyGUI::addConsoleMessage(const std::string& message) {
    if (consoleMessages.size() > 100) { // Limitar el número de mensajes
        consoleMessages.pop_front();
    }
    consoleMessages.push_back(message);
}


void MyGUI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            if (ImGui::MenuItem("Quit")) {
                SDL_Quit();
                exit(0);
            }
            if (ImGui::MenuItem("Visit GitHub")) {
#if defined(_WIN32) || defined(_WIN64)
                system("start https://github.com/CITM-UPC/MotorMont-S_GameEngine");
#elif defined(__APPLE__)
                system("open https://github.com/CITM-UPC/MotorMont-S_GameEngine");
#else
                system("xdg-open https://github.com/CITM-UPC/MotorMont-S_GameEngine");
#endif
            }
            if (ImGui::MenuItem("About")) {
                showAbout = true;
            }

            ImGui::Separator();
            ImGui::MenuItem("Show Console", nullptr, &showConsole);
            ImGui::MenuItem("Show Hierarchy", nullptr, &showHierarchy);
            ImGui::MenuItem("Show Configuration", nullptr, &showConfiguration);
            ImGui::MenuItem("Show Inspector", nullptr, &showInspector);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (showAbout) {
        ImVec2 windowPos = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f - 150, ImGui::GetIO().DisplaySize.y * 0.5f - 50);
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);

        ImGui::Begin("About Motor Montes", &showAbout, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::Text("Motor Montes v1.0");
        ImGui::Text("Developed by Biel, Ivo, and Sergi");
        ImGui::Text("Visit our Github page for more info!");

        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseClicked(0)) {
            showAbout = false;
        }
        ImGui::End();
    }

    float topBarHeight = ImGui::GetFrameHeight();



    if (showConsole) {
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y * 0.75f + topBarHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.25f), ImGuiCond_Always);
        ImGui::Begin("Console");
        for (const auto& msg : consoleMessages) {
            ImGui::TextUnformatted(msg.c_str());
        }
        ImGui::End();
    }

    if (showConfiguration) {
        updateFPS();
		updateMemoryUsage();
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.8f, topBarHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Configuration");
        ImGui::Text("Frames per Second (FPS): %.1f", current_fps);

        float max_fps = *std::max_element(fps_history.begin(), fps_history.end());
        float graph_max_fps = max_fps + 10.0f;
        ImGui::PlotLines("##FPS", fps_history.data(), fps_history.size(), fps_index, nullptr, 0.0f, graph_max_fps, ImVec2(0, 80));

        ImGui::Text("Memory Usage (MB): %.1f", current_memory_usage);
        float max_memory = *std::max_element(memory_usage_history.begin(), memory_usage_history.end());
        float graph_max_memory = max_memory + 10.0f;
        ImGui::PlotLines("##MemoryUsage", memory_usage_history.data(), memory_usage_history.size(), memory_index, nullptr, 0.0f, graph_max_memory, ImVec2(0, 80));

        ImGui::Text("Renderer Configuration:");
        ImGui::Text("Window Configuration:");
        ImGui::Text("Input Configuration:");
        ImGui::Text("Texture Configuration:");
        ImGui::End();
    }

    if (showInspector) {
        ImGui::SetNextWindowPos(ImVec2(0, topBarHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.75f), ImGuiCond_Always);
        ImGui::Begin("Inspector");

        // Display details of the selected GameObject if any is selected
        if (selectedGameObjectIndex != -1 && selectedGameObjectIndex < gameObjects.size()) {
            GameObject& selectedObject = gameObjects[selectedGameObjectIndex];

            // Editable transform information
            ImGui::Text("Transform");
            ImGui::DragFloat3("Position", &selectedObject.position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &selectedObject.rotation.x, 0.1f);
            ImGui::DragFloat3("Scale", &selectedObject.scale.x, 0.1f, 0.1f, 10.0f);

            if (selectedObject.hasMesh) {
                ImGui::Text("Mesh");
                ImGui::Checkbox("Display Normals (Triangles)", &selectedObject.displayTriangleNormals);
                ImGui::Checkbox("Display Normals (Faces)", &selectedObject.displayFaceNormals);
            }

            if (selectedObject.hasTexture) {
                ImGui::Text("Texture");

                // Define a fixed-size buffer for InputText to ensure safe string handling
                char texturePathBuffer[256];
#if defined(_MSC_VER) // Microsoft compiler, using strncpy_s for safety
                strncpy_s(texturePathBuffer, sizeof(texturePathBuffer), selectedObject.texturePath.c_str(), _TRUNCATE);
#else // Non-MSVC compilers, using strncpy with manual null-termination
                strncpy(texturePathBuffer, selectedObject.texturePath.c_str(), sizeof(texturePathBuffer) - 1);
                texturePathBuffer[sizeof(texturePathBuffer) - 1] = '\0';
#endif

                // InputText handling, updating texturePath if edited
                if (ImGui::InputText("Path", texturePathBuffer, sizeof(texturePathBuffer))) {
                    selectedObject.texturePath = texturePathBuffer;
                }

                ImGui::Text("Size: %dx%d", selectedObject.textureWidth, selectedObject.textureHeight);
                ImGui::Checkbox("Use Checker Texture", &selectedObject.useCheckerTexture);
            }
        }
        ImGui::End();
    }

    if (showHierarchy) {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.8f, ImGui::GetIO().DisplaySize.y * 0.5f + topBarHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Hierarchy");

        // Display all GameObjects in the hierarchy list
        for (int i = 0; i < gameObjects.size(); ++i) {
            if (ImGui::Selectable(gameObjects[i].name.c_str(), selectedGameObjectIndex == i)) {
                selectedGameObjectIndex = i;
            }
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyGUI::processEvent(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}
