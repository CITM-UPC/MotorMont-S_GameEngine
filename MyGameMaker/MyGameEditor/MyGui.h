// MyGui.h

#pragma once

#include "../MyGameEngine/MyWindow.h"  // Include MyWindow.h for IEventProcessor

class MyGUI : public IEventProcessor {
public:
    MyGUI();
    MyGUI(SDL_Window* window, void* context);
    MyGUI(MyGUI&&) noexcept = delete;
    MyGUI(const MyGUI&) = delete;
    MyGUI& operator=(const MyGUI&) = delete;
    ~MyGUI();

    void render();
    void processEvent(const SDL_Event& event) override;  // Override is valid
};
