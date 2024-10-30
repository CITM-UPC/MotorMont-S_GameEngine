#pragma once
#include "GraphicObject.h"
#include "MyWindow.h"
#include "Mesh.h"
#include "Camera.h"
#include <vector>

class App {
    MyWindow* window;
    Mesh model;
    std::vector<GraphicObject> gameObjects;
    Camera camera;  // Declaración de camera como miembro de App

public:
    App();
    ~App();

    bool Init();
    void Run();
    void CleanUp();
    GraphicObject* getObjectUnderCursor(float mouseX, float mouseY);
};
