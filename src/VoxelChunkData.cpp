#include "VoxelChunkData.h"
#include "GL/glew.h"
#include "AABB.h"
#include "Camera.h"
#include "Frustum.h"

void VoxelChunkData::updata(const Camera& camera, std::vector<float>& textureBuffer, std::vector<glm::vec4>& transFormBuffer)
{
  Frustum frustum(camera, WINDOW_WITH / WINDOW_HEIGHT);
  for (int i = 0; i < _transForm.size(); ++i)
  {
    glm::vec3 min(-0.5f);
    glm::vec3 max(0.5f);
    AABB aabb(min, max);
    
      // std::cout << "frustum" << std::endl;
    if (aabb.isOnFrustum(frustum, _transForm[i]))
    {
      // textureBuffer.push_back(_texture[i]);
      transFormBuffer.push_back(_transForm[i]*glm::vec4(0,0,0,1));
    }
  }
}