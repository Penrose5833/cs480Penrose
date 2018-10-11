#include "object.h"
#include "graphics_headers.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include "glm/gtx/string_cast.hpp"
using namespace std;

Object::Object()
{  
  // Vertices = {
  //   {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
  //   {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
  //   {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
  //   {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
  //   {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
  //   {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
  //   {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
  //   {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  // };

  // Indices = {
  //   2, 3, 4,
  //   8, 7, 6,
  //   1, 5, 6,
  //   2, 6, 7,
  //   7, 8, 4,
  //   1, 4, 8,
  //   1, 2, 4,
  //   5, 8, 6,
  //   2, 1, 6,
  //   3, 2, 7,
  //   3, 7, 4,
  //   5, 1, 8
  // };

  // // The index works at a 0th index
  // for(unsigned int i = 0; i < Indices.size(); i++)
  // {
  //   Indices[i] = Indices[i] - 1;
  // }

  // spinAngle = 0.0f;
  // orbitAngle = 0.0f;
  // spinDirection = 1;
  // orbitDirection = 1;

  // glGenBuffers(1, &VB);
  // glBindBuffer(GL_ARRAY_BUFFER, VB);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  // glGenBuffers(1, &IB);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  spinAngle = 0.0f;
  orbitAngle = 0.0f;
  spinDirection = 1;
  orbitDirection = 1;
  orbitRadius = 0;
  spinSpeed = 1.0f;
  orbitSpeed = 1.0f;
  size = 1.0f;
}

Object::Object(const char* filePath)
{  
	loadObject(filePath);

  // spinAngle = 0.0f;
  // orbitAngle = 0.0f;
  // spinDirection = 1;
  // orbitDirection = 1;

 

  spinAngle = 0.0f;
  orbitAngle = 0.0f;
  spinDirection = 1;
  orbitDirection = 1;
  orbitRadius = 0;
  spinSpeed = 1.0f;
  orbitSpeed = 1.0f;
  size = 1.0f;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

bool Object::loadObject(const char* path)//,
				// vector<glm::vec3>& out_vertices, 
				// vector<glm::vec2>& out_uvs, 
				// vector<glm::vec3>& outNormals)
{


  stringstream buffer;
  fstream fin;
  string tmpStr;
  glm::vec3 tmpVec3Vertex, tmpVec3Color;
  glm::vec2 tmpVec2UV;
  unsigned int tmpIndex;
  srand(time(NULL));
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);



  if(scene == NULL)
  {
    return false;
  }

  
  Vertices.resize(scene->mNumMeshes);
  Indices.resize(scene->mNumMeshes);
  for( int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
  {

    for(unsigned int vertIndex = 0; vertIndex < scene-> mMeshes[meshIndex]->mNumVertices; vertIndex++)
    {
    	tmpVec3Vertex = glm::vec3(scene->mMeshes[meshIndex]->mVertices[vertIndex].x, scene->mMeshes[meshIndex]->mVertices[vertIndex].y, scene->mMeshes[meshIndex]->mVertices[vertIndex].z);

    	if(scene->mMeshes[meshIndex]->HasTextureCoords(0))
    	{
    		tmpVec2UV = glm::vec2(scene->mMeshes[meshIndex]->mTextureCoords[0][vertIndex].x,scene->mMeshes[meshIndex]->mTextureCoords[0][vertIndex].y);
    	}
      else
      {
        tmpVec2UV = glm::vec2(0, 0);
      }

  		Vertex tmpVertex(tmpVec3Vertex, tmpVec2UV);
  		Vertices[meshIndex].push_back(tmpVertex);
    }

    for(unsigned int faceIndex = 0; faceIndex < scene->mMeshes[meshIndex]->mNumFaces; faceIndex++)
    {
      Indices[meshIndex].push_back(scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[0]);
      Indices[meshIndex].push_back(scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[1]);
      Indices[meshIndex].push_back(scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[2]);
    }

  }


  aTexture.resize(scene->mNumMeshes);

  for(int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
  {
    VB.resize(scene->mNumMeshes);
    IB.resize(scene->mNumMeshes);

    glGenBuffers(1, &VB[meshIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, VB[meshIndex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices[meshIndex].size(), &Vertices[meshIndex][0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB[meshIndex]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[meshIndex]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices[meshIndex].size(), &Indices[meshIndex][0], GL_STATIC_DRAW);
  
    aiString textureFilePath;

    scene->mMaterials[meshIndex+1]->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

    std::string textureStringFilePath = "../assets/";
    textureStringFilePath += textureFilePath.C_Str();


    tex = new Magick::Image(textureStringFilePath);
    tex->write(&blob, "RGBA");

    int width = tex->size().width();
    int height = tex->size().height();

    parent = NULL;

    glGenTextures(1, &aTexture[meshIndex]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, aTexture[meshIndex]);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex->columns(), tex->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    glBindTexture(GL_TEXTURE_2D, 0);
  }

	return true;
}

void Object::Update(unsigned int dt)
{
  // glm::mat4 rotation;
  // glm::mat4 translation;
  // glm::mat4 scale;

  if(spinDirection > 0)
  {
    spinAngle += spinSpeed * dt * M_PI/1000;
  }
  else
  {
    spinAngle -= spinSpeed * dt * M_PI/1000; 
  }

  if(orbitDirection > 0)
  {
    orbitAngle +=  orbitSpeed * dt * M_PI/1000;
  }
  else
  {
    orbitAngle -= orbitSpeed * dt * M_PI/1000;
  }

  if (parent == NULL)
  {
    parentTranslation = glm::mat4(1.0f);
  }

  else
  {
    parentTranslation = parent->translationMat;
  }
  
  glm::vec3 translationVector(orbitRadius*glm::cos(orbitAngle), 0, orbitRadius*glm::sin(orbitAngle));

  //model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0, 0.0, 0.0));
  rotationMat = glm::rotate(glm::mat4(1.0f), (spinAngle), glm::vec3(0.0, 1, 0.0));
  translationMat = glm::translate(parentTranslation, translationVector);
  scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(size));

  model = translationMat * rotationMat * scaleMat;
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetTranslation()
{
  return translationMat;
}

void Object::Render()
{
  for(int renderIndex = 0; renderIndex < VB.size(); renderIndex++)
  {  
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VB[renderIndex]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));



    glActiveTexture( GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, aTexture[renderIndex]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[renderIndex]);

    glDrawElements(GL_TRIANGLES, Indices[renderIndex].size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }
}

void Object::reverseSpin()
{
  spinDirection = spinDirection * -1;
  //spinDirection = spinDirection;
}

void Object::reverseOrbit()
{
  orbitDirection = orbitDirection * -1;
  //orbitDirection = orbitDirection;
}

void Object::setOrbitRadius(float radius)
{
  orbitRadius = radius;
}

void Object::setSpinSpeed(float speed)
{
  spinSpeed = speed;
}

void Object::setOrbitSpeed(float speed)
{
  orbitSpeed = speed;
}

void Object::setSize(float sizeScale)
{
  size = sizeScale;
}

void Object::setParent(Object* parentObj)
{
  parent = parentObj;
}