#pragma once

#include <SDL2/SDL.h>
#include <imgui.h>
#include <vector>
#include <chrono>
#include "../MyGameEngine/MyWindow.h"
#include <string>


struct ImVec3 {
    float x, y, z;
    ImVec3() : x(0.0f), y(0.0f), z(0.0f) {}
    ImVec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct GameObject {
    std::string name;
    ImVec3 position;
    ImVec3 rotation;
    ImVec3 scale;
    bool hasMesh;
    bool displayTriangleNormals;
    bool displayFaceNormals;
    bool hasTexture;
    std::string texturePath;
    int textureWidth;
    int textureHeight;
    bool useCheckerTexture;

    GameObject(const std::string& name)
        : name(name), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
        hasMesh(false), displayTriangleNormals(false), displayFaceNormals(false),
        hasTexture(false), textureWidth(0), textureHeight(0), useCheckerTexture(false) {}
};

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
