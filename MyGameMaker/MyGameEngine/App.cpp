#include "App.h"
#include "MyWindow.h"
#include <iostream>
#include <GL/glew.h>

App::App() : window(nullptr) {}

App::~App() {
    CleanUp();
}

bool App::Init() {
    try {
        window = new MyWindow("My Game Engine Window", 1280, 720);

        model.loadModel("Assets/BakerHouse/BakerHouse.obj");

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void App::Run() {
    bool running = true;
    while (running && window->isOpen()) {
        running = window->processEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model.draw();

        window->swapBuffers();
    }
}

void App::CleanUp() {
    delete window;
    window = nullptr;
}
