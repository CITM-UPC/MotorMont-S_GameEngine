#pragma once

#include <SDL2/SDL.h>
#include <imgui.h>
#include <vector>
#include <chrono>  // Include chrono for timing
#include "../MyGameEngine/MyWindow.h"

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
};
