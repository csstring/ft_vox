#pragma once
#include "Common.h"
#include "PerlinNoise.h"
#include <map>
#include "VoxelChunkData.h"
#include <utility>
//thread
class Camera;
class ChunkManager : Noncopyable
{
  private:
    uint32 _VAO, uvID, posID, chunkID ,boxID,normalID ,amount = 16 * 16 * 256;
    std::vector<glm::vec4> _objectPos;
    std::vector<glm::vec2> _objectUV;
    std::vector<glm::vec3> _objectNormal;

    PerlinNoise _perlinNoise;
    std::map<std::string,VoxelChunkData*> _chunkDatas;
    std::vector<std::pair<std::string, VoxelChunkData*>> _attachBuffer;
    std::vector<glm::vec3> _attachChunkPos;
    std::vector<std::string> _detachName;

    std::vector<glm::vec4> _drawTransFromBuffer;
    std::vector<float>     _drawTextureID;
  private:  
    void generateChunk(glm::vec3 startPos);
    void surroundCheck();
    // void writeMapFile();
    // void readMapFile();
  public:
    glm::vec3 _camPos;

  public:
    ChunkManager(
    std::vector<glm::vec4>& pos, 
    std::vector<glm::vec2>& uv,
    std::vector<glm::vec3>& normal
    ) : _perlinNoise("default"),_objectPos(pos), _objectUV(uv), _objectNormal(normal){};
    ~ChunkManager();//delete data

    void initialize();
    void update(const Camera& camera);
    void draw();
    uint32 getVectexSize(){return _objectPos.size();};
    void attachChunk();
    void detachChunk();//delete data
};