#include "AABB.h"
#include "Plane.h"
#include "Frustum.h"

bool AABB::isOnFrustum(const Frustum& frustumCam, const glm::mat4& transform) const
{
  const glm::vec3 globalCenter{ transform * glm::vec4(_center, 1.f) };

    // Scaled orientation
    const glm::vec3 right = transform[0] * _extents.x;
    const glm::vec3 up = transform[1] * _extents.y;
    const glm::vec3 forward = -transform[2] * _extents.z;

    const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

    const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

    const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

    //We not need to divise scale because it's based on the half extention of the AABB
    const AABB globalAABB(globalCenter, newIi, newIj, newIk);
  return globalAABB.isOnOrForwardPlane(frustumCam._bottomFace) &&
    globalAABB.isOnOrForwardPlane(frustumCam._farFace) &&
    globalAABB.isOnOrForwardPlane(frustumCam._leftFace) &&
    globalAABB.isOnOrForwardPlane(frustumCam._nearFace) &&
    globalAABB.isOnOrForwardPlane(frustumCam._rightFace) &&
    globalAABB.isOnOrForwardPlane(frustumCam._topFace);
}


bool AABB::isOnOrForwardPlane(const Plane& plane) const
{
  const float r = _extents.x * std::abs(plane._normal.x) +
    _extents.y * std::abs(plane._normal.y) + _extents.z * std::abs(plane._normal.z);
  
  return -r <= plane.getDistanceToPlane(_center);
}