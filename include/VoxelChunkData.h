#pragma once
#include "Common.h"
#include "EnumHeader.h"
class Camera;
class VoxelChunkData
{
  public:
    std::vector<glm::mat4> _transForm;
    std::vector<BoxTexture> _texture;

  public:
    void updata(const Camera& camera, std::vector<float>& textureBuffer, std::vector<glm::mat4>& transFormBuffer);
    ~VoxelChunkData(){};
};
