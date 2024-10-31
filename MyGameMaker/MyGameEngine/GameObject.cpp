#include "GameObject.h"
#include <iostream>
using namespace std;

GameObject::GameObject(const std::string& name)
    : name(name),                                 // Initialize name here
    position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f),
    hasMesh(false),
    displayTriangleNormals(false),
    displayFaceNormals(false),
    hasTexture(false),
    texturePath(""),
    textureWidth(0),
    textureHeight(0),
    useCheckerTexture(false)
{}

void GameObject::paint()
{
    cout << "GameObject::paint()" << endl;
    cout << "Name: " << name << endl;  // Display the name
    cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << endl;
    cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")" << endl;
    cout << "Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << endl;

    if (hasMesh) {
        cout << "Mesh is present." << endl;
        cout << "Display Triangle Normals: " << (displayTriangleNormals ? "Yes" : "No") << endl;
        cout << "Display Face Normals: " << (displayFaceNormals ? "Yes" : "No") << endl;
    }

    if (hasTexture) {
        cout << "Texture Path: " << texturePath << endl;
        cout << "Texture Size: " << textureWidth << "x" << textureHeight << endl;
        cout << "Use Checker Texture: " << (useCheckerTexture ? "Yes" : "No") << endl;
    }
}

void GameObject::setPosition(float x, float y, float z) {
    position = { x, y, z };
}

void GameObject::setRotation(float x, float y, float z) {
    rotation = { x, y, z };
}

void GameObject::setScale(float x, float y, float z) {
    scale = { x, y, z };
}

void GameObject::setTexture(const std::string& path, int width, int height) {
    hasTexture = true;
    texturePath = path;
    textureWidth = width;
    textureHeight = height;
}

void GameObject::toggleMeshNormals(bool triangleNormals, bool faceNormals) {
    hasMesh = true;
    displayTriangleNormals = triangleNormals;
    displayFaceNormals = faceNormals;
}
