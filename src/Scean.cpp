#include "Scean.h"
#include "GLM/ext.hpp"
#include "VoxelChunkData.h"

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
    int curx = -500 + 1000 * i;
    for (int j = 0; j < 1; ++j)
    {
      int curz = -500 + 1000 * j;
      std::string startXZ = std::to_string(curx) + " " + std::to_string(curz);
      VoxelChunkData& it = _chunkDatas[startXZ];
      it._pos = this->_objectPos;
      it._uv = this->_objectUV;
      it._transForm.reserve(it.amount);
      for (int x = 0; x < 1000; x++)
      {
        for (int z = 0; z < 1000; z++)
        {
          it._transForm.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(curx + x, 0, curz + z)));
        }
      }
      it.initialize();
    }
  }
}