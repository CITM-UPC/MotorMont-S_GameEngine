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
float movementSpeed = 0.1f;  // Velocidad de movimiento de la cámara

// Función para verificar errores de OpenGL
static void checkGLError(const std::string& message) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << message << "): " << err << std::endl;
    }
}

// Cargar la textura
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

// Dibujar el eje
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

// Dibujar el suelo
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

// Función de renderizado
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

// Inicializar OpenGL
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

// Función de redimensionamiento de la ventana
static void reshape_func(int width, int height) {
    glViewport(0, 0, width, height);
    camera.aspect = static_cast<double>(width) / height;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(&camera.projection()[0][0]);
}

// Función de entrada de teclado para mover la cámara
static void keyboard_func(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':  // Movimiento hacia adelante
        camera.transform().translate(glm::vec3(0, 0, movementSpeed));
        break;
    case 's':  // Movimiento hacia atrás
        camera.transform().translate(glm::vec3(0, 0, -movementSpeed));
        break;
    case 'a':  // Movimiento hacia la izquierda
        camera.transform().translate(glm::vec3(-movementSpeed, 0, 0));
        break;
    case 'd':  // Movimiento hacia la derecha
        camera.transform().translate(glm::vec3(movementSpeed, 0, 0));
        break;
    }
    glutPostRedisplay();  // Solicita actualizar la pantalla después del movimiento
}

// Función idle para mantener la ventana actualizada
static void idle_func() {
    glutPostRedisplay();
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

    // Posición inicial de la cámara
    camera.transform().pos() = glm::vec3(0, 1, 4);
    camera.transform().rotate(glm::radians(180.0), glm::vec3(0, 1, 0));

    // Cargar un modelo de ejemplo
    Mesh mesh;
    if (mesh.loadFromFile("Assets/BakerHouse/BakerHouse.fbx")) {
        auto& mesh_object = scene.emplaceChild();
        mesh_object.setMesh(std::make_shared<Mesh>(std::move(mesh)));

        mesh_object.setTextureImage(image);
    }
    else {
        std::cerr << "Failed to load BakerHouse model" << std::endl;
    }

    // Vincular funciones de GLUT
    glutDisplayFunc(display_func);
    glutReshapeFunc(reshape_func);
    glutKeyboardFunc(keyboard_func);  // Vincular la función de teclado
    glutIdleFunc(idle_func);  // Actualizar pantalla constantemente

    glutMainLoop();

    return EXIT_SUCCESS;
}
