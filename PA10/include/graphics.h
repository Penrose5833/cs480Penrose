#ifndef GRAPHICS_H
#define GRAPHICS_H

using namespace std;
#include <iostream>
#include <vector>

#include "graphics_headers.h"
#include "Camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char* configFileName);
    void Update(unsigned int dt);
    void Render();
    bool LoadObjects(char* configFileName);
    void translateCamera(glm::vec3);
    void rotateCamera(int, int);
    void returnCameraToOrigin();
    void adjustGravity(char, float);
    bool createShader(Shader *, std::string vertFilePath, std::string fragFilePath);
    bool setShaderUniformLocations(Shader *);
    void toggleShader();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_gourandShader;
    Shader *m_phongShader;
    Shader *m_currentShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_textureSampler;

    GLint m_ambient;
    GLint m_specular;
    GLint m_diffuse;
    GLint m_shininess;
    GLint m_light;
    GLint m_spot;
    GLint m_spotCutoff;
    GLint m_spotDirection;

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

    float xGravity;
    float yGravity;
    float zGravity;


    vector<Object*> objects;

};

#endif /* GRAPHICS_H */
