#include "Camera.h"
#include "object.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine

  glm::vec3 worldUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camTarget = glm::vec3(0.0f);

  camUpVec = glm::vec3(0.0,1.0,0.0);
  camPositionVec = glm::vec3(0.0, 3.0, 9.0);
  camDirectionVec = glm::normalize(camPositionVec - camTarget); // unit vector in Direction camera points - actually the opposite direction

  view = glm::lookAt( camPositionVec, //Eye Position
                      camPositionVec - camDirectionVec, //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::translateCamera(int X, int Y, int Z, unsigned int dt, bool moveCue)
{
  X *= .1f;
  Y *= .1f;
  Z *= 5.0f;
  glm::vec3 newPosition = camPositionVec;
  glm::vec3 focusPosition = glm::vec3(focusObject->GetModel()[3]);
  camDirectionVec = glm::normalize(camPositionVec - focusPosition);
  // std::cout << glm::distance(camPositionVec, focusPosition) << std::endl;

  newPosition -= (float)X * float(dt) * camSpeed * glm::normalize(glm::cross(camDirectionVec, camUpVec));
  newPosition += (float)Y * float(dt) * camSpeed * glm::normalize(glm::cross(camDirectionVec, glm::cross(camDirectionVec, camUpVec)));
  newPosition -= (float)Z * float(dt) * camSpeed * camDirectionVec;
  
  if(glm::distance(newPosition, focusPosition) >= 4.0)
  {
    camPositionVec = newPosition;
  }
}

void Camera::update()
{
  glm::vec3 focusPosition = glm::vec3(focusObject->GetModel()[3]);
  camDirectionVec = glm::normalize(camPositionVec - focusPosition);

  view = glm::lookAt( camPositionVec, //Eye Position
                      focusPosition, //Focus point
                      camUpVec); //Positive Y is up

  glm::vec3 camRelativeRight = glm::normalize(glm::cross(camDirectionVec, camUpVec));
  glm::vec3 camRelativeDown = glm::normalize(glm::cross(camDirectionVec, camRelativeRight));
  glm::vec3 cuePosition = camPositionVec + (camDirectionVec * -1.0f + camRelativeDown * 0.2f);
  glm::mat4 cueModel = glm::inverse(glm::lookAt(cuePosition, focusPosition, camUpVec));
  cueObject->Update(cueModel);
}


void Camera::setCueObject(Object* cue)
{
  cueObject = cue;
}

void Camera::setFocusObject(Object* focus)
{
  cueObject -> setFocusObject(focus);
  focusObject = focus;
}