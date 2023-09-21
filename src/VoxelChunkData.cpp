#include "VoxelChunkData.h"
#include "GL/glew.h"

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

  glGenBuffers(1, &uvID);
  glBindBuffer(GL_ARRAY_BUFFER, uvID);
  glEnableVertexAttribArray(2);	
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _uv.size(), _uv.data(), GL_STATIC_DRAW);
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _transForm.size(), _transForm.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

VoxelChunkData::~VoxelChunkData()
{
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &uvID);
  glDeleteBuffers(1, &posID);
  glDeleteBuffers(1, &chunkID);
}