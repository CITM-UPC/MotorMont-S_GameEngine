#pragma once
#include "../MyGameEditor/MyGui.h"
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
    MyGUI mygui;

};
