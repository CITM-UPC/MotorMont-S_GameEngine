#include <iostream>
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
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    for (double i = -size; i <= size; i += step) {
        glVertex3d(i, 0, -size);
        glVertex3d(i, 0, size);
        glVertex3d(-size, 0, i);
        glVertex3d(size, 0, i);
    }
    glEnd();
}

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
    glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void reshape_func(int width, int height) {
    glViewport(0, 0, width, height);
    camera.aspect = static_cast<double>(width) / height;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(&camera.projection()[0][0]);
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

    glutDisplayFunc(display_func);
    glutReshapeFunc(reshape_func);
    glutMainLoop();

    return EXIT_SUCCESS;
}
