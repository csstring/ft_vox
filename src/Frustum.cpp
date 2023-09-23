#include "Frustum.h"
#include "Camera.h"

Frustum::Frustum(const Camera& camera, float aspect)
{
  const float halfVSide = camera._zFar * tanf(glm::radians(camera._fov + 10) * 0.5f);
  const float halfHSide = halfVSide * aspect;
  const glm::vec3 frontMultFar = camera._zFar * camera._cameraFront;

  _nearFace = Plane(camera._cameraPos + camera._zNear * camera._cameraFront, camera._cameraFront);
  _farFace = Plane(camera._cameraPos + frontMultFar, -camera._cameraFront);
  _rightFace = Plane(camera._cameraPos, glm::cross(glm::normalize(frontMultFar - camera._cameraRight * halfHSide), camera._cameraUp));//left
  _leftFace = Plane(camera._cameraPos, glm::cross(camera._cameraUp, glm::normalize(frontMultFar + camera._cameraRight * halfHSide)));//right
  _topFace = Plane(camera._cameraPos, glm::cross(camera._cameraRight, glm::normalize(frontMultFar - camera._cameraUp * halfVSide)));//bottm
  _bottomFace = Plane(camera._cameraPos, glm::cross(glm::normalize(frontMultFar + camera._cameraUp * halfVSide), camera._cameraRight));//up???
}