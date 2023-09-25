#include "VoxelChunkData.h"
#include "GL/glew.h"
#include "AABB.h"
#include "Camera.h"
#include "Frustum.h"

void VoxelChunkData::updata(const Camera& camera, std::vector<float>& textureBuffer, std::vector<glm::mat4>& transFormBuffer)
{
  Frustum frustum(camera, WINDOW_WITH / WINDOW_HEIGHT);
  glm::vec3 min(0.0f);
  glm::vec3 max(1.0f);
  AABB aabb(min, max);
  
  for (int y = 0; y < 256; ++y)
  {
    if (_transForm[y].empty()) return;
    glm::mat4 trans0 = glm::translate(glm::mat4(1.0f), glm::vec3(_startPos.x, y, _startPos.z));
    glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(_startPos.x + 16, y, _startPos.z));
    glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(_startPos.x, y, _startPos.z+16));
    glm::mat4 trans3 = glm::translate(glm::mat4(1.0f), glm::vec3(_startPos.x + 16, y, _startPos.z+16));
    if (aabb.isOnFrustum(frustum, trans0) || aabb.isOnFrustum(frustum, trans1) ||
        aabb.isOnFrustum(frustum, trans2) || aabb.isOnFrustum(frustum, trans3) )
    {
      for (int i = 0; i < _transForm[y].size(); ++i)
      {
        if (aabb.isOnFrustum(frustum, _transForm[y][i]))
        {
          textureBuffer.push_back(_texture[y][i]);
          transFormBuffer.push_back(_transForm[y][i]);
        }
      }
    } 
    else
    {
      // std::cout << "cell check faile" << std::endl;
    }
  }
}