// MyWindow.h

#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

class IEventProcessor {
public:
    virtual void processEvent(const SDL_Event& event) = 0;
    virtual void render() = 0;  // Agregar el método render
    virtual ~IEventProcessor() = default;
};

class MyWindow {
    SDL_Window* _window = nullptr;
    void* _ctx = nullptr;
    IEventProcessor* gui = nullptr;  // Use interface pointer to avoid circular dependencies

    unsigned short _width = 1280;
    unsigned short _height = 720;
public:
    int width() const;
    int height() const;
    double aspect() const;
    auto* windowPtr() const;
    auto* contextPtr() const;
    MyWindow(const char* title, unsigned short width, unsigned short height);
    ~MyWindow();
    void open(const char* title, unsigned short width, unsigned short height);
    void close();
    bool isOpen() const;
    bool processEvents(IEventProcessor* event_processor = nullptr);
    void swapBuffers() const;
    void renderUI();
};
