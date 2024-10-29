#pragma once

#include "MyWindow.h"

class App {
    Window* window;

public:
    App();
    ~App();

    bool Init();
    void Run();
    void CleanUp();
};
