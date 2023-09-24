#pragma once
#include "Common.h"
#include "EnumHeader.h"
class Camera;
class VoxelChunkData
{
  public:
    uint32 _VAO, uvID, posID, chunkID ,boxID,normalID ,amount = 16 * 16 * 256;
    std::vector<glm::vec4> _pos;
    std::vector<glm::vec3> _normal;
    std::vector<glm::mat4> _transForm;
    
    std::vector<BoxTexture> _texture;
    std::vector<uint32> _textureID;
    std::vector<glm::vec2> _uv;
    std::vector<glm::mat4> buffer;
    std::vector<float> _textureIDBuffer;
  public:
    void initialize();
    void updata(const Camera& camera);
    ~VoxelChunkData();
};
