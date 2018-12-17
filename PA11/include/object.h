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
    void Update(glm::mat4 newModel);
    void Render();

    void setMass(float);
    void setInertia(btVector3);
    void setInitialPosition(btVector3);
    void setFilePath(std::string filePath);
    void setScale(glm::vec3);
    void setShape(std::string);
    void setRestitution(float);
    void setFriction(float);
    void setMotionState(std::string);
    void setState(int);
    int getState();
    void setCueStrike();
    // void increaseStrikeSpeed();
    void setStrikeSpeed(float);
    void setFocusObject(Object* ball);

    glm::mat4 GetModel();
    glm::mat4 GetTranslation();

    glm::vec4 GetDiffuse();
    glm::vec4 GetSpecular();
    float GetShininess();

    void adjustSpecularBrightness();
    void adjustDiffuseBrightness();
    void adjustShininess();

    btRigidBody* getRigidBody();
    btVector3 getInitialPosition();

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
    // float scalex;
    // float scaley;
    // float scalez;
    glm::vec3 scale;
    float restitution;
    float friction;
    std::string rigidBodyShape;
    std::string motionState;

    glm::vec4 diffuseVec;
    glm::vec4 specularVec;
    float shininess;

    bool striking = false;
    float maxStrikeSpeed = 20.0;
    float strikeSpeedPercent = 0.5;
    float strikeDisplacement = 0.0;
    float maxStrikeDisplacement = 1.5;
    Object* focusObject;
    int state;


};

#endif /* OBJECT_H */
