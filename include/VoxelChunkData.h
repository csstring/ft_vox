#pragma once
#include "Common.h"
enum TextureID {
  normal
};

class Camera;
class VoxelChunkData
{
  public:
    uint32 _VAO, uvID, posID, chunkID ,amount = 160*160;
    std::vector<glm::vec4> _pos;
    std::vector<glm::mat4> _transForm;
    std::vector<TextureID> _textureID;
    std::vector<glm::vec2> _uv;
    std::vector<glm::mat4> buffer;

  public:
    void initialize();
    void updata(const Camera& camera);
    ~VoxelChunkData();
};
