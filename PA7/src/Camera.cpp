#include "Camera.h"
#include <iostream>

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

	camUpVec = glm::vec3(0.0,1.0,0.0);
	camPositionVec = glm::vec3(0.0, 10000, 0);
	camDirectionVec = glm::normalize(camPositionVec - glm::vec3(0.0f)); // unit vector in Direction camera points - actually the opposite direction

	view = glm::lookAt( camPositionVec, //Eye Position
	                  camPositionVec - camDirectionVec, //Focus point
	                  camUpVec); //Positive Y is up

	projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
	                             float(w)/float(h), //Aspect Ratio, so Circles stay Circular
	                             0.01f, //Distance to the near plane, normally a small value like this
	                             1000000000000000.0f); //Distance to the far plane, 
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

void Camera::translate(SDL_Keycode key, unsigned int dt)
{
  if(key == SDLK_w)
  {
    camPositionVec -= float(dt) * camSpeed * camDirectionVec;
  }  
  else if(key == SDLK_a)
  {
    camPositionVec += glm::normalize(glm::cross(camDirectionVec, camUpVec)) * float(dt) * camSpeed;
  }
  else if(key == SDLK_s)
  {
    camPositionVec += float(dt) * camSpeed * camDirectionVec;
  }
  else if(key == SDLK_d)
  {
    camPositionVec -= glm::normalize(glm::cross(camDirectionVec, camUpVec)) * float(dt) * camSpeed;
  }
  else if(key == SDLK_UP)
  {
    camPositionVec += float(dt) * camUpVec;
  }
  else if(key == SDLK_DOWN)
  {
    camPositionVec -= float(dt) * camUpVec;
  }
  view = glm::lookAt( camPositionVec, //Eye Position
                      camPositionVec - camDirectionVec, //Focus point
                      camUpVec); //Positive Y is up
}

void Camera::rotateCamera(int X, int Y, unsigned int dt)
{
	camPitch += float(Y) * camSensitivity * (dt);
	camYaw += float(X) * camSensitivity * (dt);
	// std::cout << "rotating camera " << X << " " << Y << std::endl;

	if(camPitch > 89.0f)
	{
		camPitch =  89.0f;
	}
	if(camPitch < -89.0f)
	{
		camPitch = -89.0f;
	}

 	camDirectionVec = glm::normalize(
    glm::vec3(glm::cos(glm::radians(camPitch)) * glm::cos(glm::radians(camYaw)),
    glm::sin(glm::radians(camPitch)),
    glm::cos(glm::radians(camPitch)) * glm::sin(glm::radians(camYaw))));

    view = glm::lookAt( camPositionVec, //Eye Position
                        camPositionVec - camDirectionVec, //Focus point
                        camUpVec); //Positive Y is up
}