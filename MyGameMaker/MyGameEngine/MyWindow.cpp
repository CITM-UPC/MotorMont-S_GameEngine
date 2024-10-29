#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "MyWindow.h"
#include <stdexcept>

Window::Window(const char* title, unsigned short width, unsigned short height)
    : _width(width), _height(height) {
    Open(title, width, height);
}

Window::~Window() {
    Close();
}

void Window::Open(const char* title, unsigned short width, unsigned short height) {
    if (IsOpen()) return;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    _window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (!_window) throw std::runtime_error("Failed to create SDL window");

    _context = SDL_GL_CreateContext(_window);
    if (!_context) throw std::runtime_error("Failed to create OpenGL context");

    if (SDL_GL_MakeCurrent(_window, _context) != 0) throw std::runtime_error("Failed to set OpenGL context");
    if (SDL_GL_SetSwapInterval(1) != 0) throw std::runtime_error("Failed to enable VSync");
}

void Window::Close() {
    if (!IsOpen()) return;

    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    _context = nullptr;
    _window = nullptr;
}

bool Window::ProcessEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            Close();
            return false;
        }
    }
    return true;
}

void Window::SwapBuffers() const {
    SDL_GL_SwapWindow(_window);
}
