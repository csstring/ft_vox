#include "Scean.h"
#include "GLM/ext.hpp"
#include "VoxelChunkData.h"
#include "Camera.h"
Scean::Scean(std::vector<glm::vec4>& pos, std::vector<glm::vec2>& uv)
{
  _objectPos = pos;
  _objectUV = uv;
}
//0 1000
void Scean::initialize()
{
  for (int i = 0; i < 1; ++i)
  {
    int curx = -80 + 160 * i;
    for (int j = 0; j < 1; ++j)
    {
      int curz = -80 + 160 * j;
      std::string startXZ = std::to_string(curx) + " " + std::to_string(curz);
      VoxelChunkData& it = _chunkDatas[startXZ];
      it._pos = this->_objectPos;
      it._uv = this->_objectUV;
      it._transForm.reserve(it.amount);
      for (int x = 0; x < 160; x++)
      {
        for (int z = 0; z < 160; z++)
        {
          it._transForm.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(curx + x, 0, curz + z)));
        }
      }
      it.initialize();
    }
  }
  _texture.initialize();
}

void Scean::update(const Camera& camera)
{
  for (auto& it : _chunkDatas)
  {
    it.second.updata(camera);
  }
}