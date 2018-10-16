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
    void translateCamera(glm::vec3);
    void rotateCamera(int, int);
    void returnToOrigin();
  
  private:
  	glm::vec3 camPositionVec;
    glm::vec3 camDirectionVec;

    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
