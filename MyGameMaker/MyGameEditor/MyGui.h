#pragma once

#include <SDL2/SDL.h>
#include <imgui.h>
#include <vector>
#include <chrono>
#include "../MyGameEngine/MyWindow.h"
#include "../MyGameEngine/GameObject.h"  // Include GameObject header
#include <string>

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

private:
    void updateFPS();
    std::vector<float> fps_history;
    int fps_index;
    int frame_counter;
    float current_fps;

    std::chrono::steady_clock::time_point last_time;  // Last recorded time for FPS calculation

    // Configuration variables
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
    std::vector<GameObject> gameObjects;  // List of all GameObjects
    int selectedGameObjectIndex;          // Index of the selected GameObject
};
