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
    void Update(unsigned int dt, float);
    void Render();
    void reverseSpin();
    void reverseOrbit();
    void setOrbitRadius(float);
    void setSpinSpeed(float);
    void setOrbitSpeed(float);
    void setSize(float);
    void setParent(Object*);
    void setName(std::string);
    std::string getName();

    glm::mat4 GetModel();
    glm::mat4 GetTranslation();

  private:
    glm::mat4 model;
    glm::mat4 translationMat;
    glm::mat4 rotationMat;
    glm::mat4 scaleMat;

    std::vector<std::vector<Vertex>> Vertices;
    std::vector<std::vector<unsigned int>> Indices;
    std::vector<GLuint> VB;
    std::vector<GLuint> IB;
    std::vector<GLuint> aTexture;

    Magick::Blob blob;
    Magick::Image *tex;

    float spinAngle;
    float orbitAngle;
    float orbitRadius;
    float spinSpeed;
    float orbitSpeed;
    float size;
    Object* parent;
    std::string name;
};

#endif /* OBJECT_H */
