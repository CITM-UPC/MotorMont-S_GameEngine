#include <iostream>
#include <unordered_map>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
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
float movementSpeed = 0.02f;  // Speed for WASD movement

// Key state map to track pressed keys
static std::unordered_map<unsigned char, bool> keyState;

// Function to handle OpenGL errors
static void checkGLError(const std::string& message) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << message << "): " << err << std::endl;
    }
}

// Function to load texture
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

// Draw the axis
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

// Draw the floor grid
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

// Rendering function
static void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(&camera.view()[0][0]);

    texture.bind();

    drawAxis(1.0);
    drawFloorGrid(16, 0.25);
    scene.draw();

    glutSwapBuffers();
}

// OpenGL initialization
static void init_opengl() {
    glewInit();
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

// Reshape function
static void reshape_func(int width, int height) {
    glViewport(0, 0, width, height);
    camera.aspect = static_cast<double>(width) / height;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(&camera.projection()[0][0]);
}

// Update movement based on active keys
void updateMovement() {
    glm::vec3 direction(0.0f);

    if (keyState['w']) direction.z += movementSpeed;
    if (keyState['s']) direction.z -= movementSpeed;
    if (keyState['a']) direction.x += movementSpeed;
    if (keyState['d']) direction.x -= movementSpeed;

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction) * movementSpeed;
        camera.transform().translate(direction);
    }
    glutPostRedisplay();
}

// Keyboard function for key press events
static void keyboard_func(unsigned char key, int x, int y) {
    keyState[key] = true;
    updateMovement();
}

// Keyboard function for key release events
static void keyboard_up_func(unsigned char key, int x, int y) {
    keyState[key] = false;
    updateMovement();
}

// Idle function to continuously update movement
static void idle_func() {
    updateMovement();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Texture Loader Example");

    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
        std::cerr << "DevIL library version mismatch" << std::endl;
        return EXIT_FAILURE;
    }
    ilInit();
    iluInit();

    init_opengl();
    loadTexture();

    // Initial camera position
    camera.transform().pos() = glm::vec3(0, 1, 4);
    camera.transform().rotate(glm::radians(180.0), glm::vec3(0, 1, 0));

    // Load a sample model
    Mesh mesh;
    if (mesh.loadFromFile("Assets/BakerHouse/BakerHouse.fbx")) {
        auto& mesh_object = scene.emplaceChild();
        mesh_object.setMesh(std::make_shared<Mesh>(std::move(mesh)));
        mesh_object.setTextureImage(image);
    }
    else {
        std::cerr << "Failed to load BakerHouse model" << std::endl;
    }

    // Link GLUT functions
    glutDisplayFunc(display_func);
    glutReshapeFunc(reshape_func);
    glutKeyboardFunc(keyboard_func);      // Link key press event
    glutKeyboardUpFunc(keyboard_up_func); // Link key release event
    glutIdleFunc(idle_func);              // Link idle function for smooth movement

    glutMainLoop();
    return EXIT_SUCCESS;
}
