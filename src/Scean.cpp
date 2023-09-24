#include "Scean.h"
#include "GLM/ext.hpp"
#include "VoxelChunkData.h"
#include "Camera.h"
#include "PerlinNoise.h"

//0 1000
void Scean::initialize()
{
  _texture.initialize();
}

void Scean::update(const Camera& camera)
{
  _ChunkManager.update(camera._cameraPos);
  for (auto it : _chunkDatas)
  {
    it.second->updata(camera);
  }
  _texture.activeTexture();
}