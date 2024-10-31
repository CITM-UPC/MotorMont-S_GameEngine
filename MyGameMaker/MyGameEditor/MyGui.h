#pragma once

#include <SDL2/SDL.h>
#include <imgui.h>
#include <vector>
#include <chrono>
#include "../MyGameEngine/MyWindow.h"
#include "../MyGameEngine/GameObject.h"  // Include GameObject header
#include <string>
#include <deque> // Agregar al inicio del archivo


class MyGUI : public IEventProcessor
{
public:
    MyGUI(SDL_Window* window, void* context);
    MyGUI(MyGUI&&) noexcept = delete;
    MyGUI(const MyGUI&) = delete;
    MyGUI& operator=(const MyGUI&) = delete;
    ~MyGUI();

    void initialize();
    void shutdown();
    void render();
    void processEvent(const SDL_Event& event) override;
    void addConsoleMessage(const std::string& message);

    std::vector<GameObject> gameObjects;  // List of all GameObjects
    int selectedGameObjectIndex;          // Index of the selected GameObject

private:
    void updateFPS();
    void updateMemoryUsage();
    std::vector<float> fps_history;
    std::vector<float> memory_usage_history;
    int fps_index;
    int memory_index;
    int frame_counter;
    float current_fps;
    float current_memory_usage;

    std::chrono::steady_clock::time_point last_time;

    struct RendererConfig {
        // Add renderer-specific configurations here
    } rendererConfig;

    struct WindowConfig {
        // Add window-specific configurations here
    } windowConfig;

    struct InputConfig {
        // Add input-specific configurations here
    } inputConfig;

    struct TextureConfig {
        // Add texture-specific configurations here
    } textureConfig;

    // GameObject management
    
    std::deque<std::string> consoleMessages; // Buffer para mensajes de consola
};
