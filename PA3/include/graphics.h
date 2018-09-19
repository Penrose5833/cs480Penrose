#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <vector>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"



class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();
    void changeMOrbit();
    void changeEOrbit();
    void changeMRotation();
    void changeERotation();
    void changeSRotation();
    void changeMoonOrbit();
    void changeMoonRotation();
    void setParent();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    int numObjects;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

//    Object *sun;
//    Object *planet_earth;

    vector<Object*> solarObject;
    vector<glm::mat4> translations;
    float orbit;
  
};

#endif /* GRAPHICS_H */
