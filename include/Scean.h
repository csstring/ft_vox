#pragma once
#include "Common.h"
#include "VoxelChunkData.h"
#include "Texture.h"
#include <map>
#include <utility>
#include "ChunkManager.h"

class Camera;
class Scean : Noncopyable
{
  private:
    Texture                _texture;
    ChunkManager           _ChunkManager;
    
  public:
    std::map<std::string,VoxelChunkData*> _chunkDatas; //string : "x y z"
    Scean() = delete;
    Scean(std::vector<glm::vec4>& pos, std::vector<glm::vec2>& uv, std::vector<glm::vec3>& normal)
    :_ChunkManager(&_chunkDatas, pos, uv, normal){
    };
    ~Scean(){};

    void initialize();
    void addChunkData();
    // void eraseChunkData();
    void initializeChunkData(VoxelChunkData& chunkData);
    void update(const Camera& camera);
};
