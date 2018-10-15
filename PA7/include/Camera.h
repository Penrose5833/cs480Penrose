#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void translate(SDL_Keycode, unsigned int);
    void rotateCamera(int, int, unsigned int);
  
  private:
    float camSpeed = 10.0;
    float camSensitivity = 0.01;
    float camPitch = 0;
    float camYaw = 0;

  	glm::vec3 camPositionVec;
    glm::vec3 camDirectionVec;
    glm::vec3 camUpVec;

    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
