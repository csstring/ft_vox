#include "VoxelChunkData.h"
#include "GL/glew.h"
#include "AABB.h"
#include "Camera.h"
#include "Frustum.h"

void VoxelChunkData::initialize()
{
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &posID);
  glBindBuffer(GL_ARRAY_BUFFER, posID);
  glEnableVertexAttribArray(0);	
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _pos.size(), _pos.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &boxID);
  glBindBuffer(GL_ARRAY_BUFFER, boxID);
  glEnableVertexAttribArray(1);	
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glVertexAttribDivisor(1, 1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &uvID);
  glBindBuffer(GL_ARRAY_BUFFER, uvID);
  glEnableVertexAttribArray(2);	
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _uv.size(), _uv.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &normalID);
  glBindBuffer(GL_ARRAY_BUFFER, normalID);
  glEnableVertexAttribArray(3);	
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _normal.size(), _normal.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &chunkID);
  glBindBuffer(GL_ARRAY_BUFFER, chunkID);
  std::size_t vec4Size = sizeof(glm::vec4);
  glEnableVertexAttribArray(4); 
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
  glEnableVertexAttribArray(5); 
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
  glEnableVertexAttribArray(6); 
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
  glEnableVertexAttribArray(7); 
  glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
  glVertexAttribDivisor(6, 1);
  glVertexAttribDivisor(7, 1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

VoxelChunkData::~VoxelChunkData()
{
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &uvID);
  glDeleteBuffers(1, &posID);
  glDeleteBuffers(1, &chunkID);
  glDeleteBuffers(1, &normalID);
  glDeleteBuffers(1, &boxID);
}

void VoxelChunkData::updata(const Camera& camera)
{
  buffer.clear();
  _textureIDBuffer.clear();

  Frustum frustum(camera, WINDOW_WITH / WINDOW_HEIGHT);
  for (int i = 0; i < _transForm.size(); ++i)
  {
    // glm::vec3 min = glm::vec4(-0.5f,-0.5f,-0.5f,1) * transForm;
    // glm::vec3 max = glm::vec4(0.5f,0.5f,0.5f,1) * transForm;
    glm::vec3 min(-0.5f);
    glm::vec3 max(0.5f);
    AABB aabb(min, max);
    
      // std::cout << "frustum" << std::endl;
    if (aabb.isOnFrustum(frustum, _transForm[i]))
    {
      _textureIDBuffer.push_back(_texture[i]);
      buffer.push_back(_transForm[i]);
    }
  }
}