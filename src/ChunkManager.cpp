#include "ChunkManager.h"
#include <algorithm>

// void ChunkManager::generateChunk(glm::vec3 startPos)
// {
//   //file로 존재하는지 확인해야함
//   std::string fileName = std::to_string(startPos.x) + " " + std::to_string(startPos.z);
//   _attachBuffer.push_back({fileName, new VoxelChunkData()});

//   glm::vec3 endPos(startPos.x + 16, 0, startPos.z + 16);
//   VoxelChunkData* it = _attachBuffer.back().second;
//   it->_pos = this->_objectPos;
//   it->_uv = this->_objectUV;
//   it->_transForm.reserve(it->amount);
//   for (float x = startPos.x; x < endPos.x; ++x)
//   {
//     for (float z = startPos.z; z < endPos.z; ++z)
//     {

//       for (float y = 0 ; y < 256; ++y)
//       {
//         // double flag = _noise.generateOctavePerlin(tmp.x, tmp.y, tmp.y, 7, 2);
//         double flag = _perlinNoise.generatePerlin(x/64.0f, y/64.0f, z/64.0f);
//         if (flag > 0.3) continue;
//         it->_transForm.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)));
//       }
//     }
//   }
//   it->initialize();
// }
void ChunkManager::generateChunk(glm::vec3 startPos)
{
  //file로 존재하는지 확인해야함
  std::string fileName = std::to_string(startPos.x) + " " + std::to_string(startPos.z);
  _attachBuffer.push_back({fileName, new VoxelChunkData()});

  glm::vec3 endPos(startPos.x + 16, 0, startPos.z + 16);
  VoxelChunkData* it = _attachBuffer.back().second;
  it->_pos = this->_objectPos;
  it->_uv = this->_objectUV;
  it->_normal = this->_objectNormal;
  it->_transForm.reserve(it->amount);
  it->_texture.reserve(it->amount);
  for (float x = startPos.x; x < endPos.x; ++x)
  {
    for (float z = startPos.z; z < endPos.z; ++z)
    {
      double maxh = _perlinNoise.generateOctavePerlin(x/64.0f, z/64.0f, 0,3,2) * 256.f;
      // double maxh = _perlinNoise.generatePerlin(x/64.0f, z/64.0f, 0);
      for (float y = 0 ; y < maxh-1; ++y)
      {
        it->_transForm.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)));
        it->_texture.push_back(BoxTexture::DIRT);
      }
      it->_transForm.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(x, maxh, z)));
      it->_texture.push_back(BoxTexture::GRASS_SIDE);
    }
  }
  it->initialize();
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
      if (_chunkDatas->find(name) == _chunkDatas->end())
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
    _chunkDatas->insert(_attachBuffer[i]);
  }
  //unlock
}

void ChunkManager::update(glm::vec3 camPos)
{
  static int i = 0;

  if (i != 0) return;
  _attachChunkPos.clear();
  _camPos = camPos;
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

void ChunkManager::detachChunk()
{
  //lock
  for (int i =0; i < _detachName.size(); ++i)
  {
    delete _chunkDatas->find(_detachName[i])->second;
    _chunkDatas->erase(_detachName[i]);
  }
  //unlock
}