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
  camPositionVec = glm::vec3(0.0, 9.0, 3);
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

void Camera::translateCamera(SDL_Keycode key, unsigned int dt, bool moveCue)
{
  if(key == SDLK_i)
  {
    camPositionVec -= float(dt) * camSpeed * camDirectionVec;
  }  
  else if(key == SDLK_j)
  {
    camPositionVec += glm::normalize(glm::cross(camDirectionVec, camUpVec)) * float(dt) * camSpeed;
  }
  else if(key == SDLK_k)
  {
    camPositionVec += float(dt) * camSpeed * camDirectionVec;
  }
  else if(key == SDLK_l)
  {
    camPositionVec -= glm::normalize(glm::cross(camDirectionVec, camUpVec)) * float(dt) * camSpeed;
  }
  else if(key == SDLK_u)
  {
    camPositionVec += float(dt) * camSpeed* glm::normalize(camUpVec);
  }
  else if(key == SDLK_o)
  {
    camPositionVec -= float(dt) * camSpeed * glm::normalize(camUpVec);
  }
  camDirectionVec = glm::normalize(camPositionVec - glm::vec3(0,0,0));
  view = glm::lookAt( camPositionVec, //Eye Position
                      glm::vec3(0,0,0), //Focus point
                      camUpVec); //Positive Y is up

}

void Camera::rotateCamera(int X, int Y)
{
  // X *= .05f;
  // Y *= .05f;
  // currentYaw = glm::sin(camDirection.x);
  // currentPitch = glm::sin(camDirection.y);
  // float yaw = glm::asin(X / 1000.0f);
  // float pitch = glm::asin(Y / 1000.0f);

  // camDirectionVec.x = glm::cos(glm::radians)

  // float magnitude = glm::sqrt(glm::pow((float)X, 2.0f) + glm::pow((float)Y, 2.0f));
	// view = glm::rotate(view, yaw, glm::vec3(0, 1, 0));
 //  view = glm::rotate(view, pitch, glm::vec3(1, 0, 0));
}

void Camera::returnToOrigin()
{
    view = glm::lookAt( camPositionVec, //Eye Position
                        camPositionVec - camDirectionVec, //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

void Camera::setCueObject(Object* cue)
{
  cueObject = cue;
}