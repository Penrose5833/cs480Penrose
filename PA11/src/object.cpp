#include "object.h"
#include "graphics_headers.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include "glm/gtx/string_cast.hpp"
using namespace std;

Object::Object()
{  
}

// Object::Object(const char* filePath)
// {  
// 	loadObject(filePath);
// }

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
  delete objTriMesh;
  objTriMesh = NULL;
}

bool Object::loadObject(btDiscreteDynamicsWorld * dynamicsWorld)//,
				// vector<glm::vec3>& out_vertices, 
				// vector<glm::vec2>& out_uvs, 
				// vector<glm::vec3>& outNormals)
{
  stringstream buffer;
  fstream fin;
  string tmpStr;
  glm::vec3 tmpVec3Vertex, tempVec3Normal;
  glm::vec2 tmpVec2UV;
  btVector3 triArray[3];
  objTriMesh = new btTriangleMesh();
  unsigned int tmpIndex;
  srand(time(NULL));
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate);

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
    	if(scene->mMeshes[meshIndex]->HasNormals())
    	{
		    tempVec3Normal = glm::vec3(scene->mMeshes[meshIndex]->mNormals[vertIndex].x,scene->mMeshes[meshIndex]->mNormals[vertIndex].y, scene->mMeshes[meshIndex]->mNormals[vertIndex].z);
    	}
    	if(scene->mMeshes[meshIndex]->HasTextureCoords(0))
    	{
    		tmpVec2UV = glm::vec2(scene->mMeshes[meshIndex]->mTextureCoords[0][vertIndex].x,scene->mMeshes[meshIndex]->mTextureCoords[0][vertIndex].y);
    	}
      else
      {
        tmpVec2UV = glm::vec2(0, 0);
      }

  		Vertex tmpVertex(tmpVec3Vertex, tmpVec2UV, tempVec3Normal);
  		Vertices[meshIndex].push_back(tmpVertex);
    }

    for(unsigned int faceIndex = 0; faceIndex < scene->mMeshes[meshIndex]->mNumFaces; faceIndex++)
    {

      Indices[meshIndex].push_back(scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[0]);
      Indices[meshIndex].push_back(scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[1]);
      Indices[meshIndex].push_back(scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[2]);
      aiVector3D position;
      position = scene->mMeshes[meshIndex]->mVertices[scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[0]];
      triArray[0] = btVector3(position.x, position.y, position.z);
      position = scene->mMeshes[meshIndex]->mVertices[scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[1]];
      triArray[1] = btVector3(position.x, position.y, position.z);
      position = scene->mMeshes[meshIndex]->mVertices[scene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[2]];
      triArray[2] = btVector3(position.x, position.y, position.z);
      objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
      
    }
  }

  // isDynamic = (mass != 0.0f);
  shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), initialPosition));
  if(motionState == "dynamic") // perimitive shape
  {

    if(rigidBodyShape == "sphere")
    {
      shape = new btSphereShape(btScalar(scale.x));
    }
    else if(rigidBodyShape == "cube")
    {
      // cout << scale.x << " " << scale.y << " " << scale.z << '\n';
      shape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
    }
    else if(rigidBodyShape == "cylinder")
    {
      shape = new btCylinderShape(btVector3(scale.x, scale.y, scale.z));
    }
    else // default to sphere
    {
      shape = new btSphereShape(btScalar(scale.x));
    }
    shape -> calculateLocalInertia(mass, inertia);
  }
  else if(motionState == "static")
  {
    shape = new btBvhTriangleMeshShape(objTriMesh, true);
  }
  else if(motionState == "kinematic")
  {
    btConvexHullShape* tmpHullShape = new btConvexHullShape();
    int totalVertices = 0;
    for(int i = 0; i < Vertices.size(); i ++)
    {
      // btConvexHullShape(shape) -> addPoint(btVector3(Vertices[i][0], Vertices[i][1], Vertices[i][2]), true);
      totalVertices += Vertices[i].size();
    }
    // cout << totalVertices << endl;

    totalVertices *= 3;
    // btScalar hullPoints[totalVertices];
    for(int i = 0; i < Vertices.size(); i ++)
    {
      // btConvexHullShape(shape) -> addPoint(btVector3(Vertices[i][0], Vertices[i][1], Vertices[i][2]), true);

      for(int j = 0; j < Vertices[i].size(); j++)
      {
          // hullPoints[i * j + 0] = btScalar(Vertices[i][j].vertex.x);
          // hullPoints[i * j + 1] = btScalar(Vertices[i][j].vertex.y);
          // hullPoints[i * j + 2] = btScalar(Vertices[i][j].vertex.z);
        tmpHullShape -> addPoint(btVector3(Vertices[i][j].vertex.x, Vertices[i][j].vertex.y, Vertices[i][j].vertex.z), true);
      }
    }
    shape = tmpHullShape;
    // shape = new btConvexHullShape(hullPoints, Vertices.size(), sizeof(btVector3));
    // cout << "Planes: " << shape -> getNumPlanes() << '\n';

    // for(int i = 0; i < Vertices.size(); i ++)
    // {
    //   btConvexHullShape(shape) -> addPoint(btVector3(Vertices[i][0], Vertices[i][1], Vertices[i][2]), true);
    // }

    // shape -> setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    // shape -> setWorldTransform(btTransform(btQuaternion(0, 1, 0, 1)));
  }

  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

  shapeRigidBodyCI.m_friction = friction;
  rigidBody = new btRigidBody(shapeRigidBodyCI);
  // std::cout << "RR" << std::endl;
  if(motionState == "kinematic")
  {
    rigidBody -> setCollisionFlags(rigidBody-> getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  }

  // if(state == 0)
  // {
  //   rigidBody->setCcdMotionThreshold(1e-7);
  //   rigidBody->setCcdSweptSphereRadius(0.01);
  // }

  rigidBody->setRestitution(restitution);
  dynamicsWorld->addRigidBody(rigidBody); //, COLLIDE_MASK, CollidesWith);
  rigidBody->setSleepingThresholds(0,0);

  aiColor3D diffuse, specular;
  scene->mMaterials[1]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
  scene->mMaterials[1]->Get(AI_MATKEY_COLOR_SPECULAR, specular);
  scene->mMaterials[1]->Get(AI_MATKEY_SHININESS, shininess);

  diffuseVec = glm::vec4(diffuse[0], diffuse[1], diffuse[2], 1);
  specularVec = glm::vec4(specular[0], specular[1], specular[2], 1);

  aTexture.resize(scene->mNumMeshes);

  // cout << scene -> mNumMeshes <<'\n';
  for(int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
  {
    // cout << meshIndex << '\n';
    VB.resize(scene->mNumMeshes);
    IB.resize(scene->mNumMeshes);

    glGenBuffers(1, &VB[meshIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, VB[meshIndex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices[meshIndex].size(), &Vertices[meshIndex][0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB[meshIndex]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[meshIndex]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices[meshIndex].size(), &Indices[meshIndex][0], GL_STATIC_DRAW);

    aiString textureFilePath;

    // cout << scene -> mMeshes[meshIndex] -> mMaterialIndex << '\n';
    scene->mMaterials[scene -> mMeshes[meshIndex] -> mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

    std::string textureStringFilePath = "../assets/";
    textureStringFilePath += textureFilePath.C_Str();
    // cout << textureStringFilePath << '\n';

    tex = new Magick::Image(textureStringFilePath);
    tex->write(&blob, "RGBA");

    int width = tex->size().width();
    int height = tex->size().height();

    glGenTextures(1, &aTexture[meshIndex]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, aTexture[meshIndex]);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex->columns(), tex->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    glBindTexture(GL_TEXTURE_2D, 0);
  }
  // angle = 0;
  // movingUp = false;
	return true;
}

void Object::Update(unsigned int dt, glm::mat4 parentTranslation)
{

  // if(motionState == "kinematic")
  // {
  //   float deltaAngle = 7 * dt * M_PI/1000;
  //   if(state == 4)//left
  //   {
  //     if(movingUp)
  //     {
  //       angle -= deltaAngle;
  //       // trans.setOrigin(btVector3(origin.x()+0.4,origin.y(),origin.z()+0.4));
  //       if(angle <= 0 - 1)
  //       {
  //         //movingUp = false;
  //         angle = 0 - 1;
  //       }
  //     }
  //     else
  //     {
  //       if(angle <= 0)
  //       {
  //         angle += deltaAngle;
  //         //movingUp = false;
  //         // angle = M_PI;
  //         // movingUp = false;
  //       }
  //     }
  //   }
  //   else if(state == 3)//left
  //   {
  //     if(movingUp)
  //     {
  //       angle += deltaAngle;
  //       // trans.setOrigin(btVector3(origin.x()+0.4,origin.y(),origin.z()+0.4));
  //       if(angle >= 0 + 1)
  //       {
  //         //movingUp = false;
  //         angle = 0 + 1;
  //       }
  //     }
  //     else
  //     {
  //       if(angle >= 0)
  //       {
  //         angle -= deltaAngle;
  //         //movingUp = false;
  //         // angle = M_PI;
  //         // movingUp = false;
  //       }
  //     }
  //   }

  //   btTransform trans = rigidBody->getCenterOfMassTransform();
  //   //trans.setOrigin(btVector3(0,2,0));
  //   btQuaternion transrot = trans.getRotation();
  //   // btQuaternion rotquat;
  //   // rotquat = rotquat.getIdentity();
  //   // rotquat.setX(0);
  //   // rotquat.setY(-0.001);
  //   // rotquat.setZ(0);

  //   transrot.setEuler(angle,0,0);
  //   trans.setRotation(transrot);
  //   trans.setOrigin(initialPosition);
  //   // trans.setOrigin(btVector3(origin.x(),origin.y(),origin.z()));
  //   // rigidBody-> getMotionState() -> setCenterOfMassTransform(trans);
  //   rigidBody -> getMotionState() -> setWorldTransform(trans);

  // }
  btTransform trans;
  btScalar m[16];
  rigidBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::scale(glm::make_mat4(m), glm::vec3(scale.x, scale.y, scale.z));

  // if(motionState == "kinematic")
  // {

    // float deltaAngle = 0.1 * dt * M_PI/1000;
    

    // std::cout << angle << std::endl;


    // rigidBody -> setCollisionFlags(rigidBody-> getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    // btTransform trans = rigidBody->getCenterOfMassTransform();
    // //trans.setOrigin(btVector3(0,2,0));
    // btQuaternion transrot = trans.getRotation();
    // btQuaternion rotquat;
    // rotquat = rotquat.getIdentity();
    // rotquat.setX(0);
    // rotquat.setY(-0.001);
    // rotquat.setZ(0);



    // btVector3 origin = trans.getOrigin();

    // if(movingUp)
    // {
    //   angle += 0.1 * dt * M_PI/1000;

    //   
    //   trans.setOrigin(btVector3(origin.x()+0.4,origin.y(),origin.z()+0.4));
    //   if(angle >= 0.1)
    //   {
    //     movingUp = false;
    //     angle = 0;

    //   }
    // }
    // else
    // {
    //   angle += 0.1 * dt * M_PI/1000;

    //   rotquat.setEulerZYX(0,0.8 * dt * M_PI/1000,0);
    //   trans.setOrigin(btVector3(origin.x()+0.4,origin.y(),origin.z()+0.4));
    //   if(angle >= 0.1)
    //   {
    //     movingUp = true;
    //     angle = 0;
    //   }
    // }
    // transrot = rotquat * transrot;
    // btScalar x,y,z;
    // trans.getBasis().getEulerZYX(z,y,x);
    // trans.setOrigin(btVector3(x+1,y+1,z+1));
    // trans.setOrigin(btVector3(origin.x()+0.4*glm::cos(angle),origin.y(),origin.z()+0.4*glm::sin(angle)));

    
    // trans.setRotation(transrot);
    //  trans.setOrigin(btVector3(origin.x(),origin.y(),origin.z()));
    // // rigidBody-> getMotionState() -> setCenterOfMassTransform(trans);
    // rigidBody -> getMotionState() -> setWorldTransform(trans);
    // rigidBody->setActivationState(DISABLE_DEACTIVATION);
    // cout << "twisted the flipper\n";
  // }
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
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, VB[renderIndex]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));





    glActiveTexture( GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, aTexture[renderIndex]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[renderIndex]);

    glDrawElements(GL_TRIANGLES, Indices[renderIndex].size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}

void Object::setInertia(btVector3 inert)
{
  inertia = inert;
}

void Object::setMass(float x)
{
  mass = btScalar(x);
}

void Object::setInitialPosition(btVector3 pos)
{
  initialPosition = pos;
}

void Object::setFilePath(std::string path)
{
  filePath = path;
}

void Object::setScale(glm::vec3 sizeScale)
{
  scale = sizeScale;
}

void Object::setShape(string x)
{
  rigidBodyShape = x;
}

void Object::setRestitution(float x)
{
  restitution = x;
}

void Object::setFriction(float x)
{
  friction = x;
}

glm::vec4 Object::GetDiffuse()
{
  return diffuseVec;
}

glm::vec4 Object::GetSpecular()
{
  return specularVec;
}

float Object::GetShininess()
{
  return shininess;
}

void Object::adjustSpecularBrightness()
{
    float x = (int(specularVec.x * 10 + 1) % 10) / 10.0;
    float y = (int(specularVec.y * 10 + 1) % 10) / 10.0;
    float z = (int(specularVec.z * 10 + 1) % 10) / 10.0;
    specularVec = glm::vec4(x, y, z, 1);
}

void Object::adjustDiffuseBrightness()
{
    float x = (int(diffuseVec.x * 10 + 1) % 10) / 10.0;
    float y = (int(diffuseVec.y * 10 + 1) % 10) / 10.0;
    float z = (int(diffuseVec.z * 10 + 1) % 10) / 10.0;
    diffuseVec = glm::vec4(x, y, z, 1);
}

void Object::adjustShininess()
{
    shininess = int(shininess + 5) % 100;
}

void Object::setMotionState(std::string x)
{
    motionState = x;
}

btRigidBody* Object::getRigidBody()
{
  return rigidBody;
}

// void Object::setState(int input)
// {
//   state = input;
// }

// void Object::setFlip(bool x)
// {
//   movingUp = x;
// }
// void Object::setFlip(bool x)
// {
//   movingUp = x
// }