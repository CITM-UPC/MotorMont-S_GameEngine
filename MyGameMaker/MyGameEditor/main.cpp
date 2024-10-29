#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <iostream>
#include "MyGameEngine/MyWindow.h"
#include "MyGameEngine/Mesh.h"
#include "MyGameEngine/LoadModel.h"
#include "MyGameEngine/Texture.h"
#include "MyGameEngine/Camera.h"

using namespace std;

static const glm::ivec2 WINDOW_SIZE(1280, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static Camera camera;

// Función para dibujar la grilla en el plano XZ
void drawGrid(float size, int divisions) {
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.8f, 0.8f); // Color de las líneas de la grilla

    for (int i = -divisions; i <= divisions; ++i) {
        float pos = i * size / divisions;

        // Líneas en el eje X (paralelas al eje Z)
        glVertex3f(pos, 0.0f, -size);
        glVertex3f(pos, 0.0f, size);

        // Líneas en el eje Z (paralelas al eje X)
        glVertex3f(-size, 0.0f, pos);
        glVertex3f(size, 0.0f, pos);
    }

    glEnd();
    glLineWidth(1.0f);
}

static void init_openGL() {
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW." << endl;
        throw exception("GLEW initialization failed.");
    }
    if (!GLEW_VERSION_3_0) {
        cerr << "OpenGL 3.0 required but not available." << endl;
        throw exception("OpenGL 3.0 required but not available.");
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

int main(int argc, char** argv) {
    try {
        MyWindow window("OpenGL Model Rendering", WINDOW_SIZE.x, WINDOW_SIZE.y);
        init_openGL();

        camera.setProjection(45.0f, 0.1f, 1000.0f); // Ajusta fov, zNear y zFar
        camera.updateViewMatrix(glm::vec3(0.0f, 1.0f, 0.0f));

        Mesh myMesh;
        if (!myMesh.loadModel("Assets/BakerHouse/BakerHouse.fbx")) {
            cerr << "Failed to load model: Assets/BakerHouse/BakerHouse.fbx" << endl;
            return -1;
        }

        while (window.processEvents() && window.isOpen()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Dibuja la grilla en el plano XZ
            drawGrid(20.0f, 20);

            // Dibuja el modelo
            myMesh.draw();

            window.swapBuffers();
            std::this_thread::sleep_for(FRAME_DT);
        }

        cout << "Exiting normally." << endl;
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }

    return 0;
}
