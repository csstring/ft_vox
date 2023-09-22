#pragma once
#include "Common.h"

class Plane
{
  public:
    glm::vec3 _pos;
    glm::vec3 _normal;
    float _distance = 0.f;
  
  public:
    Plane(){};
    ~Plane(){};
    Plane(glm::vec3 pos, glm::vec3 normal) : _pos(pos), _normal(glm::normalize(normal)), _distance(glm::dot(normal, pos)){};

    float getDistanceToPlane(const glm::vec3& point) const {
      return glm::dot(_normal, point) - _distance;
    }
};
