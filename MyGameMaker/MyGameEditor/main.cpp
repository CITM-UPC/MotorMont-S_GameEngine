#define SDL_MAIN_HANDLED 
#define GLM_ENABLE_EXPERIMENTAL 

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <chrono>
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
#include "MyGUI.h" // Include your GUI header
#include "MyGameEngine/BoundingBox.h"
#include "MyGameEngine/Mesh.h"

using namespace std;

static bool paused = true;
static Camera camera;
static GraphicObject scene;
static Texture texture;
static std::shared_ptr<Image> image;
float movementSpeed = 0.02f;
float zoomSpeed = 0.1f;
float sensitivity = 0.005f;  // Sensitivity for mouse movement
float panSpeed = 0.01f;  // Speed for panning

static std::unordered_map<unsigned char, bool> keyState;
static bool shiftPressed = false;
static bool mouseButtonPressed = false;
static bool middleMouseButtonPressed = false;  // New flag for middle mouse button
static int lastMouseX, lastMouseY;  // To store previous mouse position
glm::vec3 localRight(1.0f, 0.0f, 0.0f);

// Variables y configuraciones globales
GraphicObject* selectedObject = nullptr;
BoundingBox selectedBoundingBox;
MyGUI* gui;



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
    glColor3ub(0, 163, 108);
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
    if (!mouseButtonPressed && !middleMouseButtonPressed) return;  // Only move when right or middle mouse button is pressed

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
            SDL_GetMouseState(&lastMouseX, &lastMouseY);
        }
        else if (event.button.button == SDL_BUTTON_MIDDLE) {  // Detect middle mouse button
            middleMouseButtonPressed = true;
            SDL_GetMouseState(&lastMouseX, &lastMouseY);
        }
        updateMovement();
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_RIGHT) mouseButtonPressed = false;
        if (event.button.button == SDL_BUTTON_MIDDLE) middleMouseButtonPressed = false;
        break;
    case SDL_MOUSEMOTION:
        if (mouseButtonPressed) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            float deltaX = static_cast<float>(lastMouseX - mouseX);
            float deltaY = static_cast<float>(mouseY - lastMouseY);

            lastMouseX = mouseX;
            lastMouseY = mouseY;

            camera.transform().rotate(deltaX * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
            camera.transform().rotate(deltaY * sensitivity, localRight);

            camera.transform().alignToGlobalUp();
        }
        else if (middleMouseButtonPressed) {  // Pan logic
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            float deltaX = static_cast<float>(mouseX - lastMouseX);
            float deltaY = static_cast<float>(mouseY - lastMouseY);

            lastMouseX = mouseX;
            lastMouseY = mouseY;

            // Move camera sideways (left-right) and vertically (up-down) for panning
            camera.transform().translate(localRight * deltaX * panSpeed);
            camera.transform().translate(glm::vec3(0.0f, 1.0f, 0.0f) * deltaY * panSpeed);
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

    loadTexture();
}

glm::vec3 ConvertMouseToWorldCoords(int mouse_x, int mouse_y, int screen_width, int screen_height) {
    float ndc_x = (2.0f * mouse_x) / screen_width - 1.0f;
    float ndc_y = 1.0f - (2.0f * mouse_y) / screen_height;
    glm::vec4 clip_coords = glm::vec4(ndc_x, ndc_y, -1.0f, 1.0f);

    glm::mat4 projection_matrix = camera.projection();
    glm::vec4 view_coords = glm::inverse(projection_matrix) * clip_coords;
    view_coords = glm::vec4(view_coords.x, view_coords.y, -1.0f, 0.0f);

    glm::mat4 view_matrix = camera.view();
    glm::vec4 world_coords = glm::inverse(view_matrix) * view_coords;

    return glm::vec3(world_coords);
}

glm::vec3 GetMousePickDir(int mouse_x, int mouse_y, int screen_width, int screen_height) {
    glm::vec3 window_coords = glm::vec3(mouse_x, screen_height - mouse_y, 0.0f);
    glm::mat4 view_matrix = camera.view();
    glm::mat4 projection_matrix = camera.projection();
    glm::vec4 viewport = glm::vec4(0, 0, screen_width, screen_height);

    glm::vec3 v0 = glm::unProject(window_coords, view_matrix, projection_matrix, viewport);
    glm::vec3 v1 = glm::unProject(glm::vec3(window_coords.x, window_coords.y, 1.0f), view_matrix, projection_matrix, viewport);
    return glm::normalize(v1 - v0);
}   



bool CheckRayAABBCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const BoundingBox& bBox) {
    float tmin = (bBox.min.x - rayOrigin.x) / rayDir.x;
    float tmax = (bBox.max.x - rayOrigin.x) / rayDir.x;
    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (bBox.min.y - rayOrigin.y) / rayDir.y;
    float tymax = (bBox.max.y - rayOrigin.y) / rayDir.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (bBox.min.z - rayOrigin.z) / rayDir.z;
    float tzmax = (bBox.max.z - rayOrigin.z) / rayDir.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    return !(tmin > tzmax || tzmin > tmax);
}
void handleMouseClick(int mouseX, int mouseY, int screenWidth, int screenHeight) {
    glm::vec3 rayOrigin = ConvertMouseToWorldCoords(mouseX, mouseY, screenWidth, screenHeight);
    glm::vec3 rayDir = GetMousePickDir(mouseX, mouseY, screenWidth, screenHeight);

    if (selectedObject && selectedObject->hasMesh()) {
        BoundingBox bbox = selectedObject->mesh().boundingBox();
        bbox = selectedObject->transform().mat() * bbox;

        if (CheckRayAABBCollision(rayOrigin, rayDir, bbox)) {
            selectedObject->setSelected(true);  // Marca el objeto como seleccionado para resaltarlo
        }
    }
}

static const auto FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

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

    // Inicializar GUI
    gui = new MyGUI(window, glContext);
    gui->initialize();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            // Detectar clic izquierdo para la selección del objeto
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                handleMouseClick(event.button.x, event.button.y, 1280, 720);
            }
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

        // Dibuja BoundingBox en verde si el objeto está seleccionado
        if (selectedObject && selectedObject->isSelected()) {
            glColor3f(0.0f, 1.0f, 0.0f); // Verde
            Mesh::drawBoundingBox(selectedObject->mesh().boundingBox());
        }

        gui->render();
        SDL_GL_SwapWindow(window);
        updateMovement();
    }

    gui->shutdown();
    delete gui;

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
