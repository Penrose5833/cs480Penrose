#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "object.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void translateCamera(SDL_Keycode key, unsigned int dt, bool moveCue);
    void translateCamera(int, int, int, unsigned int dt, bool moveCue);
    void setCueObject(Object*);
    void setFocusObject(Object*);
    void update();
  
  private:
    float camSpeed = 0.004;

    glm::vec3 camPositionVec;
    glm::vec3 camDirectionVec;
    glm::vec3 camUpVec;

    glm::mat4 projection;
    glm::mat4 view;

    Object* cueObject;
    Object* focusObject;
};

#endif /* CAMERA_H */
