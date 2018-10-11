#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <Magick++.h>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    Object(const char*);
    ~Object();
    bool loadObject(const char*);
    void Update(unsigned int dt);
    void Render();
    void reverseSpin();
    void reverseOrbit();
    void setOrbitRadius(float);
    void setSpinSpeed(float);
    void setOrbitSpeed(float);
    void setSize(float);
    void setParent(Object* parent);

    glm::mat4 GetModel();
    glm::mat4 GetTranslation();

  private:
    glm::mat4 model;
    glm::mat4 translationMat;
    glm::mat4 rotationMat;
    glm::mat4 scaleMat;
    glm::mat4 parentTranslation;

    std::vector<std::vector<Vertex>> Vertices;
    std::vector<std::vector<unsigned int>> Indices;
    std::vector<GLuint> VB;
    std::vector<GLuint> IB;
    std::vector<GLuint> aTexture;

    Object* parent;


    Magick::Blob blob;
    Magick::Image *tex;

    float spinAngle;
    float orbitAngle;
    int spinDirection;
    int orbitDirection;
    float orbitRadius;
    float spinSpeed;
    float orbitSpeed;
    float size;
};

#endif /* OBJECT_H */
