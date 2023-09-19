#pragma once
#include "Common.h"
enum TextureID {
  normal
};

struct VoxelChunkData
{
  uint32 _VAO;
  std::vector<glm::vec3> _pos;
  std::vector<glm::mat4> _transForm;
  std::vector<TextureID> _textureID;
};
