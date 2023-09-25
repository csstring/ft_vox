#include "Scean.h"
#include "GLM/ext.hpp"
#include "VoxelChunkData.h"
#include "Camera.h"
#include "PerlinNoise.h"

//0 1000
void Scean::initialize()
{
  _texture.initialize();
  _ChunkManager.initialize();
}

void Scean::update(const Camera& camera)
{
  _ChunkManager.update(camera);
  _texture.activeTexture();
}

void Scean::draw()
{
  _ChunkManager.draw();
}