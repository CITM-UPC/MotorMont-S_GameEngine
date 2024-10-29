#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

class Window {
    SDL_Window* _window = nullptr;
    SDL_GLContext _context = nullptr;
    unsigned short _width;
    unsigned short _height;

public:
    Window(const char* title, unsigned short width, unsigned short height);
    ~Window();

    void Open(const char* title, unsigned short width, unsigned short height);
    void Close();
    bool IsOpen() const { return _window != nullptr; }
    bool ProcessEvents();
    void SwapBuffers() const;

    int width() const { return _width; }
    int height() const { return _height; }
    double aspect() const { return static_cast<double>(_width) / _height; }
};

