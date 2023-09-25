#include "ChunkManager.h"
#include "GL/glew.h"
#include "Camera.h"

ChunkManager::~ChunkManager()
{
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &uvID);
  glDeleteBuffers(1, &posID);
  glDeleteBuffers(1, &chunkID);
  glDeleteBuffers(1, &normalID);
  glDeleteBuffers(1, &boxID);
}

void ChunkManager::initialize()
{
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &posID);
  glBindBuffer(GL_ARRAY_BUFFER, posID);
  glEnableVertexAttribArray(0);	
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _objectPos.size(), _objectPos.data(), GL_STATIC_DRAW);
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _objectUV.size(), _objectUV.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &normalID);
  glBindBuffer(GL_ARRAY_BUFFER, normalID);
  glEnableVertexAttribArray(3);	
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _objectNormal.size(), _objectNormal.data(), GL_STATIC_DRAW);
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

void ChunkManager::generateChunk(glm::vec3 startPos)
{
  //file로 존재하는지 확인해야함
  std::string fileName = std::to_string(startPos.x) + " " + std::to_string(startPos.z);
  _attachBuffer.push_back({fileName, new VoxelChunkData()});

  glm::vec3 endPos(startPos.x + 16, 0, startPos.z + 16);
  VoxelChunkData* it = _attachBuffer.back().second;
  it->_startPos = glm::vec3(startPos.x, 0, startPos.z);
  for (float x = startPos.x; x < endPos.x; ++x)
  {
    for (float z = startPos.z; z < endPos.z; ++z)
    {
      int maxh = _perlinNoise.generateOctavePerlin(x/64.0f, z/64.0f, 0,3,2) * 256.f;
      // double maxh = _perlinNoise.generatePerlin(x/64.0f, z/64.0f, 0);
      for (int y = 0 ; y < maxh-1; ++y)
      {
        it->_transForm[y].push_back(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)));
        it->_texture[y].push_back(BoxTexture::DIRT);
      }
      it->_transForm[maxh-1].push_back(glm::translate(glm::mat4(1.0f), glm::vec3(x, maxh, z)));
      it->_texture[maxh-1].push_back(BoxTexture::GRASS_SIDE);
    }
  }
}

void ChunkManager::surroundCheck()
{
  int32 curx = _camPos.x - (static_cast<int32>(_camPos.x) % 16);
  int32 curz = _camPos.z - (static_cast<int32>(_camPos.z) % 16);
  int32 minx = std::clamp(curx - 4 * 16, 0, 16384);
  int32 maxx = std::clamp(curx + 4 * 16, 0, 16384);
  int32 minz = std::clamp(curz - 4 * 16, 0, 16384);
  int32 maxz = std::clamp(curz + 4 * 16, 0, 16384);
  for (int x = minx; x < maxx; x += 16)
  {
    for (int z = minz; z < maxz; z += 16)
    {
      std::string name = std::to_string(x) + " " + std::to_string(z);
      if (_chunkDatas.find(name) == _chunkDatas.end())
      {
        _attachChunkPos.push_back(glm::vec3(x,0,z));
      }
    }
  }
}

void ChunkManager::attachChunk()
{
  //lock
  for (int i = 0; i < _attachBuffer.size(); ++i)
  {
    _chunkDatas.insert(_attachBuffer[i]);
  }
  //unlock
}

void ChunkManager::update(const Camera& camera)
{
  static int i = 0;
  static int j = 0;
  if (i == 0){
    _attachChunkPos.clear();
    _camPos = camera._cameraPos;
    i++;
    surroundCheck();
    if (_attachChunkPos.size())
    {
      for (const glm::vec3& xyz : _attachChunkPos)
      {
        generateChunk(xyz);
      }
      attachChunk();
    }
    if (_detachName.size())
      detachChunk();
  }

    _drawTransFromBuffer.clear();
    _drawTextureID.clear();

    for (auto& it : _chunkDatas)
    {
      it.second->updata(camera, _drawTextureID,_drawTransFromBuffer);
    }
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, chunkID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _drawTransFromBuffer.size(), _drawTransFromBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, boxID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _drawTextureID.size(), _drawTextureID.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
}

void ChunkManager::draw()
{
  glBindVertexArray(_VAO);
  glDrawArraysInstanced(GL_TRIANGLES, 0, _objectPos.size(), _drawTransFromBuffer.size());
  glBindVertexArray(0);
  // std::cout << _drawTransFromBuffer.size() << std::endl;
}

void ChunkManager::detachChunk()
{
  //lock
  for (int i =0; i < _detachName.size(); ++i)
  {
    delete _chunkDatas.find(_detachName[i])->second;
    _chunkDatas.erase(_detachName[i]);
  }
  //unlock
}