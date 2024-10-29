#include <windows.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "MyGameEngine/Camera.h"

// Window dimensions
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const unsigned int FPS = 60;
const auto FRAME_DT = std::chrono::duration<float>(1.0f / FPS);

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 view;
void main() {
    gl_Position = projection * view * vec4(position, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 fragColor;
void main() {
    fragColor = vec4(0.5, 0.5, 0.5, 1.0);
}
)";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

GLuint createShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) throw std::runtime_error("Vertex shader compilation failed");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) throw std::runtime_error("Fragment shader compilation failed");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) throw std::runtime_error("Shader program linking failed");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void drawGrid(float size, int divisions) {
    glBegin(GL_LINES);
    for (int i = -divisions; i <= divisions; ++i) {
        float pos = i * size / divisions;
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex3f(pos, 0.0f, -size);
        glVertex3f(pos, 0.0f, size);
        glVertex3f(-size, 0.0f, pos);
        glVertex3f(size, 0.0f, pos);
    }
    glEnd();
}

int main() {
    // Step 1: Register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = TEXT("OpenGLWindowClass");

    RegisterClass(&wc);

    // Step 2: Create the window
    HWND hwnd = CreateWindowEx(
        0,
        TEXT("OpenGLWindowClass"),
        TEXT("OpenGL Window"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    if (!hwnd) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }

    // Step 3: Set up pixel format for OpenGL
    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    // Step 4: Create and enable the OpenGL rendering context
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    // Step 5: Initialize GLEW after creating an OpenGL context
    glewExperimental = GL_TRUE; // Enable experimental features for GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    initOpenGL(); // Initialize OpenGL settings
    GLuint shaderProgram = createShaderProgram();

    Camera camera;
    camera.setProjection(10.0f, 0.1f, 100.0f); // Set orthographic projection
    glm::vec3 camTarget(0.0f, 0.0f, 0.0f);

    // Step 6: Show the window
    ShowWindow(hwnd, SW_SHOW);

    // Step 7: Enter the message loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // Rendering code
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update the camera's view matrix
            camera.updateViewMatrix(camTarget);

            // Use the shader program and set uniforms
            glUseProgram(shaderProgram);
            GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
            GLint viewLoc = glGetUniformLocation(shaderProgram, "view");

            if (projLoc != -1 && viewLoc != -1) {
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camera.projection[0][0]);
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera.view[0][0]);
            }
            else {
                std::cerr << "Failed to get uniform locations for projection or view" << std::endl;
            }

            // Draw the grid
            drawGrid(10.0f, 10);

            // Swap buffers
            SwapBuffers(hdc);

            // Cap the frame rate
            std::this_thread::sleep_for(FRAME_DT);
        }
    }

    // Step 8: Clean up resources
    glDeleteProgram(shaderProgram);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);

    return 0;
}
