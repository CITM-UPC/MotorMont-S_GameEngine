#pragma once

#include <SDL2/SDL.h> // Incluye SDL para SDL_Event
#include <imgui.h>
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
};
