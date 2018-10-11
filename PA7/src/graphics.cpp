#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
	for(int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
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

  // Create the object
  // m_cube1 = new Object();
  // m_cube1->setOrbitRadius(5);
  // m_cube2 = new Object();
  // m_cube2->setOrbitRadius(2);
  if(!LoadObjects())
  {
  	printf("Failed to load objects\n");
    return false;
  }

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

  //Locate the texture sampler in the shader
  m_textureSampler = m_shader->GetUniformLocation("gSampler");
  if(m_textureSampler == INVALID_UNIFORM_LOCATION)
  {
    printf("m_textureSampler not found\n");
    return false;
  }



  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

bool Graphics::LoadObjects()
{
  string filePath;
	numObjects = 3;
	for(int i = 0; i < numObjects; i++)
	{
    cout << "Please specify the file name" << endl;
    cin >> filePath;
    filePath = "../assets/" + filePath + ".obj";
    //filePath = "../assets/textured.obj";
		objects.push_back(new Object(filePath.c_str()));
	}

  //Sun
  objects[0]->setOrbitRadius(0.0f);
  objects[0]->setSpinSpeed(0.5f);
  objects[0]->setOrbitSpeed(0.0f);
  objects[0]->setSize(1.0f);

  //Earth
  objects[1]->setOrbitRadius(8.0f);
  objects[1]->setSpinSpeed(1.0f);
  objects[1]->setOrbitSpeed(0.5f);
  objects[1]->setSize(0.25f);
  objects[1]->setParent(objects[0]);

  //Earths Moon
  objects[2]->setOrbitRadius(2.5f);
  objects[2]->setSpinSpeed(3.0f);
  objects[2]->setOrbitSpeed(2.0f);
  objects[2]->setSize(0.05f);
  objects[2]->setParent(objects[1]);

	// // objects[0]->setOrbitRadius(0.0f);
	// // objects[0]->setSpinSpeed(0.5f);
	// // objects[0]->setOrbitSpeed(0.0f);
	// // objects[0]->setSize(2.0f);

	// // objects[1]->setOrbitRadius(8.0f);
	// // objects[1]->setSpinSpeed(1.0f);
	// // objects[1]->setOrbitSpeed(0.5f);
	// // objects[1]->setSize(0.75f);

	// // objects[2]->setOrbitRadius(2.5f);
	// // objects[2]->setSpinSpeed(3.0f);
	// // objects[2]->setOrbitSpeed(2.0f);
	// // objects[2]->setSize(0.25f);

	return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  // m_cube1->Update(dt, glm::mat4(1.0f));
  // m_cube2->Update(dt, m_cube1 -> GetTranslation());
	// for(int i = 0; i < objects.size(); i++)
	// {
 //    objects[i]
	// }

	// objects[0]-> Update(dt, glm::mat4(1.0f));

  for (int i = 0; i < numObjects; i++)
  {
   objects[i]->Update(dt); 
  }
	// objects[1]-> Update(dt, objects[0]->GetTranslation());
	// objects[2]-> Update(dt, objects[1]->GetTranslation());
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

  // Render the object
  for(int i = 0; i < objects.size(); i++)
  {
  	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
  	objects[i]->Render();
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

void Graphics::reverseObjectSpin()
{
  objects[0]->reverseSpin();
}

void Graphics::reverseObjectOrbit()
{
  objects[0]->reverseOrbit();
}

void Graphics::translateCamera(glm::vec3 direction)
{
  m_camera->translateCamera(direction);
}

void Graphics::rotateCamera(int X, int Y)
{
  m_camera->rotateCamera(X, Y);
}

void Graphics::returnCameraToOrigin()
{
  m_camera-> returnToOrigin();
}