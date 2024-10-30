
#include "App.h"
#include "MyWindow.h"
#include "Camera.h"   
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>  
#include <limits>
#include "BoundingBox.h"

GraphicObject* App::getObjectUnderCursor(float mouseX, float mouseY) {
    glm::vec3 rayDirection = camera.calculateRayFromMouse(mouseX, mouseY, window->width(), window->height());

    float closestDistance = std::numeric_limits<float>::max();
    GraphicObject* selectedObject = nullptr;

    for (auto& obj : gameObjects) {
        BoundingBox bbox = obj.worldBoundingBox();
        if (bbox.intersects(rayDirection, camera.position())) {
            float distance = glm::distance(glm::vec3(camera.position()), glm::vec3(bbox.center()));

            if (distance < closestDistance) {
                closestDistance = distance;
                selectedObject = &obj;
                std::cout << "Hit: ";
            }
        }
    }


    return selectedObject;
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
