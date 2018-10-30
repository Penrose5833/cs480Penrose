#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <Magick++.h>
#include "graphics_headers.h"
#include <string>

class Object
{
  public:
    Object();
    // Object(const char*);
    ~Object();
    bool loadObject(btDiscreteDynamicsWorld *);
    void Update(unsigned int dt, glm::mat4);
    void Render();

    void setMass(float);
    void setInertia(btVector3);
    void setInitialPosition(btVector3);
    void setFilePath(std::string filePath);


    // void reverseSpin();
    // void reverseOrbit();
    // void setOrbitRadius(float);
    // void setSpinSpeed(float);
    // void setOrbitSpeed(float);
    // void setSize(float);

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

    btTriangleMesh *objTriMesh;
    btCollisionShape *shape;
    btDefaultMotionState *shapeMotionState;
    btRigidBody *rigidBody;

    btScalar mass;
    btVector3 inertia;
    btVector3 initialPosition;
    std::string filePath;
    bool isDynamic;

    // float spinAngle;
    // float orbitAngle;
    // int spinDirection;
    // int orbitDirection;
    // float orbitRadius;
    // float spinSpeed;
    // float orbitSpeed;
    // float size;
};

#endif /* OBJECT_H */
