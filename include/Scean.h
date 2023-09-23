#pragma once
#include "Common.h"
#include "VoxelChunkData.h"
#include "Texture.h"
#include <map>
#include <utility>
class Camera;
class Scean : Noncopyable
{
  private:
    std::vector<glm::vec4> _objectPos;
    std::vector<glm::vec2> _objectUV;
    Texture                _texture;
    
  public:
    std::map<std::string,VoxelChunkData> _chunkDatas; //string : "x y z"
    Scean() = delete;
    Scean(std::vector<glm::vec4>& pos, std::vector<glm::vec2>& uv);
    ~Scean(){};

    uint32 getVectexSize(){return _objectPos.size();};
    void initialize();
    void addChunkData();
    // void eraseChunkData();
    void initializeChunkData(VoxelChunkData& chunkData);
    void update(const Camera& camera);
};
