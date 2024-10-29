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

        // Configurar cámara con zNear y zFar adecuados
        camera.setProjection(0.1f, 1000.0f);
        camera.updateViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f));  // Apunta al origen

        // Cargar y dibujar el modelo
        Mesh myMesh;
        if (!myMesh.loadModel("Assets/BakerHouse/BakerHouse.fbx")) {
            cerr << "Failed to load model: Assets/BakerHouse.fbx" << endl;
            return -1;
        }

        while (window.processEvents() && window.isOpen()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Dibuja la malla cargada
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
