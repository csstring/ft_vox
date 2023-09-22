#pragma once
#include "Common.h"

class Plane;
class Frustum;

class AABB
{
  private:
    glm::vec3 _center; //world
    glm::vec3 _extents;

  public:
    AABB(const glm::vec3& center, float x, float y, float z) 
    : _center(center), _extents(glm::vec3(x,y,z)){};
    AABB(const glm::vec3& min, const glm::vec3& max) 
    : _center((min + max) * 0.5f), _extents(glm::vec3(max.x - _center.x, max.y - _center.y, max.z - _center.z)){};
    AABB() = delete;
    ~AABB(){};

    bool isOnFrustum(const Frustum& frustumCam, const glm::mat4& transfrom) const;
    bool isOnOrForwardPlane(const Plane& plane) const;
};
