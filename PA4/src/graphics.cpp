#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao); 
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }
 
  numObjects = 6;

   for (int i = 0; i < numObjects; i++)
  { 
    solarObject.push_back(new Object("../objects/box.obj"));

  }

//Sun
  solarObject[0]->setOrbit(0);
  solarObject[0]->setScale(.5);
  solarObject[0]->setRotationSpeed(.25);


//Earth
  solarObject[1]->setOrbit(6);
  solarObject[1]->setScale(.25);
  solarObject[1]->setParent(solarObject[0]);
  solarObject[1]->setOrbitSpeed(.5);
  solarObject[1]->setRotationSpeed(1);

//Earths Moon
  solarObject[2]->setOrbit(3);
  solarObject[2]->setScale(.15);
  solarObject[2]->setParent(solarObject[1]);
  solarObject[2]->setOrbitSpeed(2);
  solarObject[2]->setRotationSpeed(1.5);

//Mars
  solarObject[3]->setOrbit(9);
  solarObject[3]->setScale(.1);
  solarObject[3]->setParent(solarObject[0]);
  solarObject[3]->setOrbitSpeed(1);
  solarObject[3]->setRotationSpeed(1);

//Phobos
  solarObject[4]->setOrbit(2);
  solarObject[4]->setScale(.1);
  solarObject[4]->setParent(solarObject[3]);
  solarObject[4]->setOrbitSpeed(1.5);
  solarObject[4]->setRotationSpeed(10);

//Deimos
  solarObject[5]->setOrbit(3);
  solarObject[5]->setScale(.05);
  solarObject[5]->setParent(solarObject[3]);
  solarObject[5]->setOrbitSpeed(2);
  solarObject[5]->setRotationSpeed(5);




  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

//Change Rotation upon "A" from keyboard input.
void Graphics::changeERotation()
{
  //pass to object;
  int changeRotation = -1;

  solarObject[1]->changeRot(changeRotation);
  
}

void Graphics::changeSRotation()
{
  //pass to object;
  int changeRotation = -1;
  solarObject[0]->changeRot(changeRotation);
  
}

void Graphics::changeMRotation()
{
  //pass to object;
  int changeRotation = -1;

  solarObject[3]->changeRot(changeRotation);
  
}

void Graphics::changeMoonRotation()
{
  //pass to object;
  int changeRotation = -1;
  solarObject[2]->changeRot(changeRotation);
  solarObject[4]->changeRot(changeRotation);
  solarObject[5]->changeRot(changeRotation);
  
}

//Change Orbit up a mouse click
void Graphics::changeEOrbit()
{
  //pass to object;
  int changeOrbit = -1;
  solarObject[1]->changeOrb(changeOrbit);
}

void Graphics::changeMoonOrbit()
{
  //pass to object;
  int changeOrbit = -1;
  solarObject[2]->changeOrb(changeOrbit);
  solarObject[4]->changeOrb(changeOrbit);
  solarObject[5]->changeOrb(changeOrbit);
}

void Graphics::changeMOrbit()
{
  //pass to object;
  int changeOrbit = -1;
  solarObject[3]->changeOrb(changeOrbit);
}

void Graphics::Update(unsigned int dt)
{
  for (int i = 0; i < numObjects; i++)
    {
      solarObject[i]->Update(dt);   
    }
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 
  
  //render the object
  for (int i = 0; i < numObjects; i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(solarObject[i]->GetModel())); 
    solarObject[i]->Render();
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

