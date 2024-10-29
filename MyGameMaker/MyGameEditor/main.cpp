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

using hrclock = chrono::high_resolution_clock;
using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(1280, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static Camera camera;

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(position, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 color;

void main() {
    color = vec4(0.8, 0.3, 0.3, 1.0); // Set color to a light reddish hue
}
)";

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

void drawGrid(float size, int divisions) {
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    for (int i = -divisions; i <= divisions; ++i) {
        float pos = static_cast<float>(i) * size / divisions;
        glColor3f(0.8f, 0.8f, 0.8f);

        // Draw vertical lines along the Z-axis
        glVertex3f(pos, 0.0f, -size);
        glVertex3f(pos, 0.0f, size);

        // Draw horizontal lines along the X-axis
        glVertex3f(-size, 0.0f, pos);
        glVertex3f(size, 0.0f, pos);
    }

    glEnd();
    glLineWidth(1.0f);
}


GLuint createShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        throw exception("Vertex shader compilation failed.");
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        throw exception("Fragment shader compilation failed.");
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main(int argc, char** argv) {
    try {
        MyWindow window("OpenGL Model Rendering", WINDOW_SIZE.x, WINDOW_SIZE.y);
        init_openGL();

        GLuint shaderProgram = createShaderProgram();

        camera.setProjection(20.0f, 0.1f, 100.0f);


        Mesh myMesh;
        if (!myMesh.loadModel("Assets/BakerHouse/BakerHouse.fbx")) {
            cerr << "Failed to load model: Assets/BakerHouse.fbx" << endl;
            return -1;
        }

        while (window.processEvents() && window.isOpen()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);
            camera.applyProjection(shaderProgram);

            drawGrid(20.0f, 20);

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
