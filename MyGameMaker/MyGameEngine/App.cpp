#include "App.h"
#include "MyWindow.h"
#include <iostream>
#include <GL/glew.h>
#include "../MyGameEditor/MyGui.h"

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
