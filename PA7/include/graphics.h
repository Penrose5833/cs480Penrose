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
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();
    // void reverseObjectSpin();
    // void reverseObjectOrbit();
    bool LoadObjects();
    void translateCamera(SDL_Keycode, unsigned int);
    void rotateCamera(int, int, unsigned int);
    Object* getObjectPtr(string);
    void changeSpeed(int);

  private:
    float scalePower;
    float speedScale;
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_textureSampler;

    // Object *m_cube1;
    // Object *m_cube2;

    vector<Object*> objects;

};

#endif /* GRAPHICS_H */
