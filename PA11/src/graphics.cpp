#include "graphics.h"
#include <fstream>
#include <sstream>

// struct ScoringContactResultCallback : public btCollisionWorld::ContactResultCallback
// {
//   ScoringContactResultCallback(int *scoreInput, unsigned int * elapsedDTInput, int* currentComboInput) : score(scoreInput), elapsedDT(elapsedDTInput), currentCombo(currentComboInput) {}

//   btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0,
//                int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
//   {
//     if(*elapsedDT != 0)
//     {
//       *score += 10 * *currentCombo;
//       std::cout << "Score: " << *score << " Combo: x" << *currentCombo << std::endl;
//       *currentCombo += 1;
//       *elapsedDT = 0;
//     }
    
//   }

//   int *score;
//   unsigned int * elapsedDT;
//   int *currentCombo;
// };

// struct ResetContactResultCallback : public btCollisionWorld::ContactResultCallback
// {
//   ResetContactResultCallback(int* livesInput, bool* lostLifeInput, bool* ballInPlayInput) : lives(livesInput), lostLife(lostLifeInput), ballInPlay(ballInPlayInput) {}

//   btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0,
//                int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
//   {
    
//     if(*ballInPlay)
//     {
//       *lives -= 1;
//       *lostLife = true;
//       *ballInPlay = false;

//       std::cout << "Lives: " << *lives << std::endl;
//     }
    
    
//   }

//   int* lives;
//   bool* lostLife;
//   bool* ballInPlay;
// };

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
  for(int i = 0; i < objects.size(); i++)
  {
    delete objects[i];
  }
  delete broadphase;
  broadphase = NULL;
  delete collisionConfiguration;
  collisionConfiguration = NULL;
  delete dispatcher;
  dispatcher = NULL;
  delete solver;
  solver = NULL;
  // delete dynamicsWorld;
  dynamicsWorld = NULL;
}

bool Graphics::Initialize(int width, int height, char *configFileName)
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

  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  xGravity = 0.0;
  yGravity = -9.81;
  zGravity = 0.0;
  dynamicsWorld->setGravity(btVector3(xGravity, yGravity, zGravity));
  ambientStrength = 0.2;
  // currentObjectIndex = 0;

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


  if(!LoadObjects(configFileName))
  {
    printf("Failed to load objects\n");
    return false;
  }

  // Set up the shaders
  m_gourandShader = new Shader();
  m_phongShader = new Shader();
  createShader(m_gourandShader, string("../Shaders/vertexLightingVertShader.txt"), string("../Shaders/vertexLightingFragShader.txt"));
  createShader(m_phongShader, string("../Shaders/fragLightingVertShader.txt"), string("../Shaders/fragLightingFragShader.txt"));
  m_currentShader = m_gourandShader;
  setShaderUniformLocations(m_currentShader);

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // score = 0;
  // lives = 3;
  // lifeLost = false;
  // ballInPlay = true;
  // elapsedDT = 150;
  // currentCombo = 1;

  return true;
}

bool Graphics::createShader(Shader * shaderPtr, string vertexShaderFilePath, string fragmentShaderFilePath)
{
  if(!shaderPtr->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!shaderPtr->AddShader(GL_VERTEX_SHADER, vertexShaderFilePath))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!shaderPtr->AddShader(GL_FRAGMENT_SHADER, fragmentShaderFilePath))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!shaderPtr->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }
}

bool Graphics::setShaderUniformLocations(Shader * shaderPtr)
{
  // Locate the projection matrix in the shader
  m_projectionMatrix = shaderPtr->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = shaderPtr->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = shaderPtr->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //Locate the texture sampler in the shader
  m_textureSampler = shaderPtr->GetUniformLocation("gSampler");
  if(m_textureSampler == INVALID_UNIFORM_LOCATION)
  {
    printf("m_textureSampler not found\n");
    return false;
  }

  //Locate the ambient product in the shader
  m_ambient = shaderPtr->GetUniformLocation("AmbientProduct");
  if(m_ambient == INVALID_UNIFORM_LOCATION)
  {
    printf("m_ambient not found\n");
    return false;
  }

  //Locate the specular product in the shader
  m_specular = shaderPtr->GetUniformLocation("SpecularProduct");
  if(m_specular == INVALID_UNIFORM_LOCATION)
  {
    printf("m_specular not found\n");
    return false;
  }

  //Locate the diffuse product in the shader
  m_diffuse = shaderPtr->GetUniformLocation("DiffuseProduct");
  if(m_diffuse == INVALID_UNIFORM_LOCATION)
  {
    printf("m_diffuse not found\n");
    return false;
  }

  //Locate the texture sampler in the shader
  m_shininess = shaderPtr->GetUniformLocation("Shininess");
  if(m_shininess == INVALID_UNIFORM_LOCATION)
  {
    printf("m_shininess not found\n");
    return false;
  }

  //Locate the texture sampler in the shader
  m_light = shaderPtr->GetUniformLocation("LightPosition");
  if(m_light == INVALID_UNIFORM_LOCATION)
  {
    printf("m_light not found\n");
    return false;
  }
  
  m_spot = shaderPtr->GetUniformLocation("SpotPosition");
  if(m_spot == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spot not found\n");
    return false;
  }

  m_spotDirection = shaderPtr->GetUniformLocation("SpotDirection");
  if(m_spotDirection == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spotDirection not found\n");
    return false;
  }

  m_spotCutoff = shaderPtr->GetUniformLocation("SpotCutoff");
  if(m_spotCutoff == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spotDirection not found\n");
    return false;
  }
}

bool Graphics::LoadObjects(char* configFileName)
{
  string filePath;
  string tmp;
  stringstream buffer;
  int idx = 0;

  fstream fin;
  fin.open(("../Configuration/" + string(configFileName)).c_str(), std::fstream::in);
  if(!fin)
  {
    cout << "Failed to open config file" << endl;
    return false;
  }
  buffer << fin.rdbuf();
  fin.close();


  while(!buffer.eof())
  {


    buffer >> tmp;
    cout << tmp << endl;
    if(buffer.eof())
    {
      //cout << tmp << endl;
      break;
    }
    if(tmp == "#")
    {
 
      //cout << tmp << endl;
      buffer.ignore(1000, '\n');
    }
    else
    {
    
      // cout << tmp << "s" << endl;
      objects.push_back(new Object());

      // set .obj filepath
      objects[objects.size()-1] -> setFilePath("../assets/" + tmp + ".obj");
      // std::cout << tmp << endl;

      // set rigidbody shape
      buffer >> tmp;
      // cout << tmp << endl;
      objects[objects.size() -1] -> setShape(tmp);

      // set rigidbody motion state
      buffer >> tmp;
      // cout << tmp << endl;
      objects[objects.size() -1] -> setMotionState(tmp);

      // set object scale
      glm::vec3 tmpGlmVec3;
      buffer >> tmp;
      // cout << tmp << endl;
      tmpGlmVec3.x = stof(tmp);
      buffer >> tmp;
      // cout << tmp << endl;
      tmpGlmVec3.y = stof(tmp);
      buffer >> tmp;
      // cout << tmp << endl;
      tmpGlmVec3.z = stof(tmp);
      objects[objects.size() -1] -> setScale(tmpGlmVec3);

      // set mass
      buffer >> tmp;
      // cout << tmp << endl;
      objects[objects.size() - 1] -> setMass(stof(tmp));

      // set restitution
      buffer >> tmp;
      // cout << tmp << endl;
      objects[objects.size() - 1] -> setRestitution(stof(tmp));

      // set friction
      buffer >> tmp;
      // cout << tmp << endl;
      objects[objects.size() - 1] -> setFriction(stof(tmp));

      btVector3 tmpBtVec3 = btVector3(0.0f, 0.0f, 0.0f);

      //set Inertia
      buffer >> tmp;
      // cout << tmp << endl;
      tmpBtVec3.setX(btScalar(stof(tmp)));
      buffer >> tmp;
      // cout << tmp << endl;
      tmpBtVec3.setY(btScalar(stof(tmp)));
      buffer >> tmp;
      // cout << tmp << endl;
      tmpBtVec3.setZ(btScalar(stof(tmp)));
      objects[objects.size()-1] -> setInertia(tmpBtVec3);

      // set initial position
      tmpBtVec3 = btVector3(0.0f, 0.0f, 0.0f);
      buffer >> tmp;
      // cout << tmp << endl;
      tmpBtVec3.setX(btScalar(stof(tmp)));
      buffer >> tmp;
      // cout << tmp << endl;
      tmpBtVec3.setY(btScalar(stof(tmp)));
      buffer >> tmp;
      // cout << tmp << endl;
      tmpBtVec3.setZ(btScalar(stof(tmp)));
      objects[objects.size()-1] -> setInitialPosition(tmpBtVec3);
      buffer >> tmp;
      // cout << tmp << "k"<< endl;
      // objects[objects.size()-1]->setState(stoi(tmp));
      // std::cout << "loadObject" << std::endl;

      // if(stoi(tmp) == 1)
      // {
      //   ballIndex = objects.size()-1;
      // }
      // if(stoi(tmp) == 2)
      // {
      //   scoreableIndices.push_back( objects.size()-1);
      // }
      // if(stoi(tmp) == 3)
      // {
      //   leftPaddleIndex = objects.size()-1;
      // }
      // if(stoi(tmp) == 4)
      // {
      //   rightPaddleIndex = objects.size()-1;
      // }
      // if(stoi(tmp) == 5)
      // {
      //   deathIndex = objects.size() -1;
      // }

      objects[objects.size() -1] -> loadObject(dynamicsWorld);
      

      // std::cout << tmp << std::endl;
    }

  }

   m_camera -> setCueObject(objects[1]);
  // make glass cover
  // btCollisionShape* shape = new btStaticPlaneShape(btVector3(0, -1, 0), 0);
  // btDefaultMotionState* shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0,0.5,0)));
  // btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(0, shapeMotionState, shape, btVector3(0, 0, 0));
  // btRigidBody* rigidBody = new btRigidBody(shapeRigidBodyCI);
  // dynamicsWorld-> addRigidBody(rigidBody);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  // m_cube1->Update(dt, glm::mat4(1.0f));
  // m_cube2->Update(dt, m_cube1 -> GetTranslation());
  dynamicsWorld->stepSimulation(dt/1000.0, 100000); 
  for(int i = 0; i < objects.size(); i++)
  {
      objects[i] -> Update(dt, glm::mat4(1.0f));
  }

  // ScoringContactResultCallback scoreCallback(&score, &elapsedDT, &currentCombo);
  // ResetContactResultCallback resetCallback(&lives, &lifeLost, &ballInPlay);
  // elapsedDT += dt;
  // if(elapsedDT > 150)
  // {
  //  // std::cout << elapsedDT << std::endl;
  //  for(int scoreIndex = 0; scoreIndex < scoreableIndices.size(); scoreIndex++)
  //  {
  //   dynamicsWorld->contactPairTest(objects[ballIndex]->getRigidBody(),objects[scoreableIndices[scoreIndex]]->getRigidBody(), scoreCallback);
  //  }
    
  // }

  // dynamicsWorld->contactPairTest(objects[ballIndex]->getRigidBody(),objects[deathIndex]->getRigidBody(), resetCallback);

  // if(lifeLost)
  // {
  //   if(lives > 0)
  //   {
  //     btTransform tmpTransform = objects[ballIndex]->getRigidBody()->getCenterOfMassTransform();
  //     tmpTransform.setOrigin(btVector3(2.5,.2,2.85));
  //     objects[ballIndex]->getRigidBody()->proceedToTransform(tmpTransform);
  //     // ballInPlay = true;
  //     lifeLost = false;
  //     currentCombo = 1;
  //   }
  //   else if(lives == 0)
  //   {
  //     std::cout << "Game Over" << std::endl;
  //     std::cout << "Final Score Was " << score << std::endl;
  //     btTransform tmpTransform = objects[ballIndex]->getRigidBody()->getCenterOfMassTransform();
  //     tmpTransform.setOrigin(btVector3(2.5,.2,2.85));
  //     objects[ballIndex]->getRigidBody()->proceedToTransform(tmpTransform);
  //     lives = -1;
  //   }
  // }
  // objects[0]-> Update(dt, glm::mat4(1.0f));
  //objects[1]-> Update(dt, glm::mat4(1.0f));
  // // objects[2]-> Update(dt, objects[1]->GetTranslation());
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_currentShader->Enable();


  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 
  

  //Light uniforms
  glUniform4f(m_ambient, ambientStrength, ambientStrength, ambientStrength, 1);
  glUniform4f(m_light, 1, 1, 0, 1);
  glUniform4f(m_spot, -1.1, 5, -5, 1);


  // glm::vec4 temp = glm::vec4 (0, 5, 5, 1) - (objects[5]->GetModel()*glm::vec4(0,0,0,1));
  // glUniform4f(m_spotDirection, temp.x, temp.y, temp.z, temp.w);
  // glUniform1f(m_spotCutoff, glm::radians(10.0));


  // Render the object
  for(int i = 0; i < objects.size(); i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));

    glm::vec4 diffuse = objects[i]->GetDiffuse();
    glm::vec4 specular =objects[i]->GetSpecular();
     //std::cout << glm::to_string(diffuse) << glm::to_string(specular) << objects[i]->GetShininess() << std::endl;
    glUniform4f(m_diffuse, diffuse.x, diffuse.y, diffuse.z, 1);
    glUniform4f(m_specular, specular.x, specular.y, specular.z, 1);
    glUniform1f(m_shininess, objects[i] -> GetShininess());

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

void Graphics::translateCamera(SDL_Keycode key, unsigned int dt, bool moveCue)
{
  m_camera->translateCamera(key, dt, moveCue);
}

void Graphics::rotateCamera(int X, int Y)
{
  m_camera->rotateCamera(X, Y);
}

// void Graphics::returnCameraToOrigin()
// {
//   m_camera-> returnToOrigin();
// }

// void Graphics::adjustGravity(char coordinate, float gravityAdjustment)
// {
//     if (coordinate == 'X') //rotate about x axis -> pitch
//     {
//       if(xGravity != gravityAdjustment)
//       {
//         xGravity = xGravity + gravityAdjustment;
//         dynamicsWorld->setGravity(btVector3(xGravity, yGravity, zGravity));
//       }
//     }

//     else if (coordinate == 'Z')
//     {
//       if(zGravity != gravityAdjustment)
//       {
//         zGravity = zGravity + gravityAdjustment;
//         dynamicsWorld->setGravity(btVector3(xGravity, yGravity, zGravity));
//       }
//     }
// }

void Graphics::toggleShader()
{
  if(m_gourandShader == m_currentShader)
  {
    m_currentShader = m_phongShader;
  }
  else
  {
    m_currentShader = m_gourandShader;
  }
  setShaderUniformLocations(m_currentShader);
}

// void Graphics::adjustAmbientLight()
// {
//   ambientStrength = (int(ambientStrength * 10 + 1) % 10) / 10.0 ;
//   cout << ambientStrength << endl;
// }

// void Graphics::cycleObjectSelection()
// {
//   currentObjectIndex = (currentObjectIndex + 1) % objects.size();
//   cout << currentObjectIndex << endl;
// }

// void Graphics::adjustObjectSpecularBrightness()
// {
//   objects[currentObjectIndex] -> adjustSpecularBrightness();
// }

// void Graphics::adjustObjectDiffuseBrightness()
// {
//   objects[currentObjectIndex] -> adjustDiffuseBrightness();
// }

// void Graphics::adjustObjectShininess()
// {
//   objects[currentObjectIndex] -> adjustShininess();
// }

// void Graphics::adjustSpotlightBrightness()
// {

// }

// void Graphics::adjustSpotlightSize()
// {

// }

// void Graphics::applyPaddleImpulseRight()
// {
//   objects[rightPaddleIndex]->getRigidBody()->applyImpulse(btVector3(0,0,5), btVector3(1,0,1));
// }

// void Graphics::applyPaddleImpulseLeft()
// {
//   objects[leftPaddleIndex]->getRigidBody()->applyImpulse(btVector3(0,0,-5), btVector3(1,0,1));
// }

// void Graphics::applyImpulse()
// {
//   glm::vec4 temp = glm::vec4 (0,0,0,1) - (objects[ballIndex]->GetModel()*glm::vec4(0,0,0,1));

//   if(temp.x <= -2.4 && temp.z <= -2.7 && lives > 0)
//   {
//       ballInPlay = true;
//     objects[ballIndex]->getRigidBody()->applyCentralImpulse(btVector3(0,0,-35));
//   }
// }

// void Graphics::setRightFlip(bool x)
// {
//   objects[rightPaddleIndex]->setFlip(x);
// }

// void Graphics::setLeftFlip(bool x)
// {
//   objects[leftPaddleIndex]->setFlip(x);
// }

// void Graphics::newGame()
// {
//     std::cout << "-------------------------------------------------------------------------------" << std::endl;
//     std::cout << "Starting New Game!" << std::endl;
//     std::cout << "Final Score Was " << score << std::endl;
//     std::cout << "Current Lives: 3" << std::endl;
//     btTransform tmpTransform = objects[ballIndex]->getRigidBody()->getCenterOfMassTransform();
//     tmpTransform.setOrigin(btVector3(2.5,.2,2.85));
//     objects[ballIndex]->getRigidBody()->proceedToTransform(tmpTransform);
//     lifeLost = false;
//     currentCombo = 1;
//     lives = 3;
//     score = 0;
// }