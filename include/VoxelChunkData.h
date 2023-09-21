#pragma once
#include "Common.h"
enum TextureID {
  normal
};

class VoxelChunkData
{
  public:
    uint32 _VAO, uvID, posID, chunkID ,amount = 1000000;
    std::vector<glm::vec4> _pos;
    std::vector<glm::mat4> _transForm;
    std::vector<TextureID> _textureID;
    std::vector<glm::vec2> _uv;

  public:
    void initialize();
    ~VoxelChunkData();
};
