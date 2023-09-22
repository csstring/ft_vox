#include "Frustum.h"
#include "Camera.h"

Frustum::Frustum(const Camera& camera, float aspect)
{
  const float halfVSide = camera._zFar * tanf(glm::radians(camera._fov) * 0.5f);
  const float halfHSide = halfVSide * aspect;
  const glm::vec3 frontMultFar = camera._zFar * camera._cameraFront;

  _nearFace = Plane(camera._cameraPos + camera._zNear * camera._cameraFront, camera._cameraFront);
  _farFace = Plane(camera._cameraPos + frontMultFar, -camera._cameraFront);
  _rightFace = Plane(camera._cameraPos, glm::cross(frontMultFar - camera._cameraRight * halfHSide, camera._cameraUp));
  _leftFace = Plane(camera._cameraPos, glm::cross(camera._cameraUp, frontMultFar + camera._cameraRight * halfHSide));
  _topFace = Plane(camera._cameraPos, glm::cross(camera._cameraRight, frontMultFar - camera._cameraUp * halfVSide));
  _bottomFace = Plane(camera._cameraPos, glm::cross(frontMultFar + camera._cameraUp * halfVSide, camera._cameraRight));
}