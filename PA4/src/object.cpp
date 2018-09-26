#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  orbAngle = 0.0f;
  rotAngle = 0.0f;

  parent = NULL;

  glGenBuffers(1, &VB); //2
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB); //2
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


}

Object::Object(std::string objectPath)
{  

  std::fstream file;
  file.open(objectPath, std::fstream::in);
  std::stringstream buffer;
  if(!file)
  {
    std::cerr << "Error Loading object file" << std::endl;
  }

  if (file)
  {
    buffer << file.rdbuf();
    file.close();   
  }
  
  std::string line;
  float x;
  float y;
  float z;

  std::string a, b, c;

  while(!buffer.eof())
  {
    buffer >> line;
    if (line == "v")
    {
        buffer >> x >> y >> z;
        glm::vec3 v = glm::vec3(x, y, z); 
        glm::vec3 c = glm::vec3((glm::sin(x)+1)/2, (glm::cos(y)+1)/2, (glm::cos(z)+1)/2);
        Vertices.push_back(Vertex(v, c));
    }

    if (line == "f")
    {
      buffer >> a >> b >> c;
      std::string sub_a = a.substr(0, '/');
      std::string sub_b = b.substr(0, '/');
      std::string sub_c = c.substr(0, '/');
      unsigned int ui_a = std::stoul(sub_a, nullptr, 0);
      unsigned int ui_b = std::stoul(sub_b, nullptr, 0);
      unsigned int ui_c = std::stoul(sub_c, nullptr, 0);
      Indices.push_back(ui_a);
      Indices.push_back(ui_b);
      Indices.push_back(ui_c);
    }

  }

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  orbAngle = 0.0f;
  rotAngle = 0.0f;

  parent = NULL;

  glGenBuffers(1, &VB); //2
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB); //2
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


}
Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{
    if (orbitDirection == 1)
    {
      orbAngle += orbitSpeed * dt * M_PI/1000;
    }

    else
    {
      orbAngle -= orbitSpeed * dt * M_PI/1000;
    }

    if (rotationDirection == 1)
    {
      rotAngle += rotationSpeed * dt * M_PI/1000;
    }

    else
    {
      rotAngle -= rotationSpeed * dt * M_PI/1000;
    }
  
    glm::mat4 translation;

    if (parent == NULL)
    {
      translation = glm::mat4(1.0f);
    }

    else
    {
      translation = parent->translate;
    }

  //get parent translation matrix into glm::mat4(1.0f) instead of hardcoding
  // glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (rotAngle), glm::vec3(0.0, 1.0, 0.0));
 
  glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (rotAngle), glm::vec3(0.0, 1.0, 0.0));
  translate = glm::translate(translation, glm::vec3(orbitSize * glm::cos(orbAngle) , 0.0, orbitSize * glm::sin(orbAngle)));
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleObject, scaleObject, scaleObject));
  model = translate * rotate * scale;

 }

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
 
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::changeOrb(int changeOrbit)
{
  orbitDirection = orbitDirection * changeOrbit;
}

void Object::changeRot(int changeRotation)
{
  rotationDirection = rotationDirection * changeRotation;
}

void Object::setOrbit(float orbit)
{
  orbitSize = orbit;
}


void Object::setScale(float scale)
{
  scaleObject = scale;
}

void Object::setTranslation(glm::mat4 transObj)
{
    
  translate = glm::translate(transObj, glm::vec3(orbitSize * glm::cos(orbAngle) , 0.0, orbitSize * glm::sin(orbAngle)));
     
}

void Object::setParent(Object* parentObj)
{
  parent = parentObj;
}

void Object::setOrbitSpeed(float oSpeed)
{
  orbitSpeed = oSpeed;
}

void Object::setRotationSpeed(float rSpeed)
{
  rotationSpeed = rSpeed;
}