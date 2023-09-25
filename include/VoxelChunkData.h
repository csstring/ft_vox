#pragma once
#include "Common.h"
#include "EnumHeader.h"
class Camera;
class VoxelChunkData
{
  public:
    std::vector<glm::mat4> _transForm[256];
    std::vector<BoxTexture> _texture[256];
    glm::vec3 _startPos;
  public:
    void updata(const Camera& camera, std::vector<float>& textureBuffer, std::vector<glm::mat4>& transFormBuffer);
    ~VoxelChunkData(){};
};
