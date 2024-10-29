#include "App.h"
#include <iostream>

App::App() : window(nullptr) {}

App::~App() {
    CleanUp();
}

bool App::Init() {
    try {
        window = new Window("My Game Engine Window", 1280, 720);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize the window: " << e.what() << std::endl;
        return false;
    }
}

void App::Run() {
    bool running = true;
    while (running && window->IsOpen()) {
        running = window->ProcessEvents();
        window->SwapBuffers();
    }
}

void App::CleanUp() {
    delete window;
    window = nullptr;
}
