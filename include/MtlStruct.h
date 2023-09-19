#pragma once
#include "Common.h"

struct MtlStruct
{
  float _Ns = 1;
  glm::vec3 _Ka = glm::vec3(0.0f);
  glm::vec3 _Kd = glm::vec3(0.0f);
  glm::vec3 _Ks = glm::vec3(0.0f);
  float _Ni = 1;
  float _d = 1;
  uint8 _illum = 2;
};
