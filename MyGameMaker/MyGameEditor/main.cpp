#define SDL_MAIN_HANDLED 
#define GLM_ENABLE_EXPERIMENTAL 

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp> 
#include <glm/gtx/orthonormalize.hpp>  
#include <IL/il.h>
#include <IL/ilu.h>
#include "MyGameEngine/Camera.h"
#include "MyGameEngine/Texture.h"
#include "MyGameEngine/Mesh.h"
#include "MyGameEngine/GraphicObject.h"
#include "MyGameEngine/Image.h"

using namespace std;

static bool paused = true;
static Camera camera;
static GraphicObject scene;
static Texture texture;
static std::shared_ptr<Image> image;
float movementSpeed = 0.02f;
float zoomSpeed = 0.1f;
float sensitivity = 0.005f;  // Sensitivity for mouse movement

static std::unordered_map<unsigned char, bool> keyState;
static bool shiftPressed = false;
static bool mouseButtonPressed = false;
static int lastMouseX, lastMouseY;  // To store previous mouse position
glm::vec3 localRight(1.0f, 0.0f, 0.0f);

static void checkGLError(const std::string& message) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << message << "): " << err << std::endl;
    }
}

static void loadTexture() {
    image = std::make_shared<Image>();
    if (image->loadFromFile("Assets/BakerHouse/Baker_house.png")) {
        texture.setImage(image);
        texture.wrapMode = Texture::Repeat;
        texture.filter = Texture::Linear;
    }
    else {
        std::cerr << "Failed to load texture image" << std::endl;
    }
}

static void drawAxis(double size) {
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(size, 0, 0);
    glColor3ub(0, 255, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, size, 0);
    glColor3ub(0, 0, 255);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, size);
    glEnd();
}

static void drawFloorGrid(int size, double step) {
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    for (double i = -size; i <= size; i += step) {
        glVertex3d(i, 0, -size);
        glVertex3d(i, 0, size);
        glVertex3d(-size, 0, i);
        glVertex3d(size, 0, i);
    }
    glEnd();
}

// Update movement based on active keys and mouse button state
void updateMovement() {
    if (!mouseButtonPressed) return;  // Only move when right mouse button is pressed

    // Adjust speed based on shift key state
    float currentSpeed = shiftPressed ? movementSpeed * 2 : movementSpeed;

    glm::vec3 direction(0.0f);
    if (keyState['w']) direction.z += currentSpeed;
    if (keyState['s']) direction.z -= currentSpeed;
    if (keyState['a']) direction.x += currentSpeed;
    if (keyState['d']) direction.x -= currentSpeed;

    // Add controls for up and down movement with Q and E keys
    if (keyState['q']) direction.y += currentSpeed;  // Move up
    if (keyState['e']) direction.y -= currentSpeed;  // Move down

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction) * currentSpeed;
        camera.transform().translate(direction);
    }
}

void handleInput(SDL_Event& event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        keyState[event.key.keysym.sym] = true;
        if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) shiftPressed = true;
        updateMovement();
        break;
    case SDL_KEYUP:
        keyState[event.key.keysym.sym] = false;
        if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) shiftPressed = false;
        updateMovement();
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_RIGHT) {
            mouseButtonPressed = true;
            SDL_GetMouseState(&lastMouseX, &lastMouseY);  // Initialize last mouse position
        }
        updateMovement();
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_RIGHT) mouseButtonPressed = false;
        break;
    case SDL_MOUSEMOTION:
        if (mouseButtonPressed) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Calculate mouse delta
            float deltaX = static_cast<float>(lastMouseX - mouseX);  // Inverted X for yaw
            float deltaY = static_cast<float>(mouseY - lastMouseY);  // Regular Y for pitch

            // Update last mouse position
            lastMouseX = mouseX;
            lastMouseY = mouseY;

            // Apply rotation to the camera: yaw around world Y-axis, pitch around camera's local X-axis
            camera.transform().rotate(deltaX * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));  // Yaw (left-right) on world Y-axis
            camera.transform().rotate(deltaY * sensitivity, localRight);  // Pitch (up-down) around local X-axis

            // Align the camera's orientation to global up
            camera.transform().alignToGlobalUp();
        }
        break;

    case SDL_MOUSEWHEEL:
        glm::vec3 forward = camera.transform().forward();
        if (event.wheel.y > 0) {
            camera.transform().translate(forward * -zoomSpeed);
        }
        else if (event.wheel.y < 0) {
            camera.transform().translate(forward * zoomSpeed);
        }
        break;
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            int width = event.window.data1;
            int height = event.window.data2;
            glViewport(0, 0, width, height);
            camera.aspect = static_cast<double>(width) / height;
        }
        break;
    default:
        break;
    }
}

void setupOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Load and set up textures
    loadTexture();
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Texture Loader Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
        std::cerr << "DevIL library version mismatch" << std::endl;
        return EXIT_FAILURE;
    }
    ilInit();
    iluInit();

    setupOpenGL();

    // Set initial camera position and orientation
    camera.transform().pos() = glm::vec3(0, 1, 4);
    camera.transform().rotate(glm::radians(180.0), glm::vec3(0, 1, 0));

    Mesh mesh;
    if (mesh.loadFromFile("Assets/BakerHouse/BakerHouse.fbx")) {
        auto& mesh_object = scene.emplaceChild();
        mesh_object.setMesh(std::make_shared<Mesh>(std::move(mesh)));
        mesh_object.setTextureImage(image);
    }
    else {
        std::cerr << "Failed to load BakerHouse model" << std::endl;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            handleInput(event);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(&camera.projection()[0][0]);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(&camera.view()[0][0]);

        texture.bind();
        drawAxis(1.0);
        drawFloorGrid(16, 0.25);
        scene.draw();

        SDL_GL_SwapWindow(window);
        updateMovement();
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
