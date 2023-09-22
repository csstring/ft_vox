#pragma once
#include "Common.h"
#include "Plane.h"
class Camera;

class Frustum : Noncopyable
{
  public:
    Plane _topFace;
    Plane _bottomFace;
    Plane _rightFace;
    Plane _leftFace;
    Plane _farFace;
    Plane _nearFace;

  public:
    Frustum(const Camera& camera, float aspect);
    Frustum() = delete;
    ~Frustum(){};
};