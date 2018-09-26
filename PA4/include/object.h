#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <sstream>
#include <fstream>

class Object
{
  public:
    Object();
    Object(std::string objectPath);
    ~Object();
    void Update(unsigned int dt);
    void Render();
    void changeOrb(int changeOrbit);
    void changeRot(int changeEarthRotation);
    void setOrbit(float orbitSize);
    void setScale(float scaleObject);
    void setTranslation(glm::mat4 transObj);
    void setParent(Object* parent);
    void setOrbitSpeed(float orbSpeed);
    void setRotationSpeed(float rotSpeed);
    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    glm::mat4 translate;
    glm::mat4 trans;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    float orbAngle;
    float rotAngle;
    int rotationDirection = 1;
    int orbitDirection = 1;
    float orbitSize;
    float scaleObject;
    float orbitSpeed;
    float rotationSpeed;
    Object* parent;

};

#endif /* OBJECT_H */
