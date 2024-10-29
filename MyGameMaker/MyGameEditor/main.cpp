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
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

using hrclock = chrono::high_resolution_clock;
using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(1280, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 position; // Vertex position input

// Declare uniform variables for projection and view matrices
uniform mat4 projection; 
uniform mat4 view;

void main() {
    // Calculate the final vertex position
    gl_Position = projection * view * vec4(position, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

out vec4 fragColor; // Output color of the fragment

void main() {
    fragColor = vec4(0.5, 0.5, 0.5, 1.0); // Set fragment color to gray
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

// Function to draw the grid
void drawGrid(float size, int divisions) {
    glBegin(GL_LINES);

    // Draw the grid lines along the X-axis and Z-axis
    for (int i = -divisions; i <= divisions; ++i) {
        float pos = static_cast<float>(i) * size / divisions;
        glColor3f(0.8f, 0.8f, 0.8f); // Light gray color
        glVertex3f(pos, 0.0f, -size); // Start
        glVertex3f(pos, 0.0f, size);  // End
        glVertex3f(-size, 0.0f, pos); // Start
        glVertex3f(size, 0.0f, pos);  // End
    }

    glEnd();
}

// Function to load and compile shaders
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

        Camera camera;
        camera.transform().pos() = glm::vec3(0.0f, 5.0f, 10.0f); // Position the camera above the grid
        camera.transform().fwd() = glm::vec3(0.0f, -1.0f, -1.0f); // Look towards the grid

        // Set the orthographic projection and view matrices
        glm::mat4 projMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f); // Orthographic projection
        glm::mat4 viewMatrix = camera.view();

        // Load and compile shaders
        GLuint shaderProgram = createShaderProgram();

        // Create the mesh and load the model
        Mesh myMesh;
        if (!myMesh.loadModel("Assets/BakerHouse/BakerHouse.fbx")) {
            cerr << "Failed to load model: Assets/BakerHouse/BakerHouse.fbx" << endl;
            return -1;
        }

        // Main render loop
        while (window.processEvents() && window.isOpen()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set the orthographic projection and view matrices
            glm::dmat4 projMatrix = glm::ortho(-10.0, 10.0, -10.0, 10.0, 0.1, 100.0); // Orthographic projection
            glm::dmat4 viewMatrix = camera.view();

            // Pass matrices to the shader
            GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
            GLint viewLoc = glGetUniformLocation(shaderProgram, "view");

            glUseProgram(shaderProgram); // Use the shader program before setting uniforms
            glUniformMatrix4dv(projLoc, 1, GL_FALSE, &projMatrix[0][0]); // Set the projection matrix
            glUniformMatrix4dv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]); // Set the view matrix

            // Draw the grid
            drawGrid(10.0f, 10); // 10 units size, with 10 divisions

            // Draw the mesh
            myMesh.draw();

            window.swapBuffers();
            this_thread::sleep_for(FRAME_DT); // Simple frame rate control
        }

        cout << "Exiting normally." << endl;
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }

    return 0;
}
