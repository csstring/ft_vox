#pragma once
#include "Common.h"
#include "VoxelChunkData.h"

class Scean
{
  private:
  public:
    VoxelChunkData _chunkData;
    Scean(){};
    ~Scean(){};
    void initialize();
};
