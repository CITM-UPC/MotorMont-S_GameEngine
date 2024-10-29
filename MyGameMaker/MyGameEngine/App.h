#pragma once

#include "MyWindow.h"
#include "Mesh.h"

class App {
    MyWindow* window;
    Mesh model;

public:
    App();
    ~App();

    bool Init();
    void Run();
    void CleanUp();
};
