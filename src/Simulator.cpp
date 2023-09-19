#include "Simulator.h"
#include "Parser.h"
#include "GL/glew.h"
#include <random>
#include "DDSLoder.h"
#include <algorithm>
#include "shader.h"
#include <unistd.h>
#include "GLM/ext.hpp"
void Simulator::colorBlendingTriger(NUMINPUT input)
{
  if (_blendingTriger == false && _blendingRatio == 0.0f)
  {
    _blendingTriger = true;
  } 
  else if (_blendingTriger == true && _blendingRatio == 1.0f)
  {
    _blendingTriger = false;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    float random1 = dis(gen);
    float random2 = dis(gen);
    float random3 = dis(gen);
    _curColor = glm::vec3(random1, random2, random3);
  }
}

void Simulator::moveObjectThreeAxis(glm::vec3 move)
{
  _worldTranslate = glm::translate(_worldTranslate, move);
}

void Simulator::moveToCenter(Parser& parser)
{
  float minZ = std::numeric_limits<float>::max();
  float minY = std::numeric_limits<float>::max();
  float minX = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::min();
  float maxY = std::numeric_limits<float>::min();
  float maxX = std::numeric_limits<float>::min();

  for (const auto& vertex : parser._facePos) 
  {
    minZ = std::min(minZ, vertex.z);
    minY = std::min(minY, vertex.y);
    minX = std::min(minX, vertex.x);
    maxZ = std::max(maxZ, vertex.z);
    maxY = std::max(maxY, vertex.y);
    maxX = std::min(maxX, vertex.x);
  }
  glm::vec4 center(0,0,glm::mix(minZ, maxZ, 0.5),0);

  for (auto& it : parser._facePos)
    it -= center;
}

void Simulator::sendDataToGpuBuffer(const Parser& parser)
{
  _vertexSize = parser._facePos.size();
  
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glEnableVertexAttribArray(0);	
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * parser._facePos.size(), parser._facePos.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  std::vector<glm::vec3> colors;
  colors.resize(_vertexSize, glm::vec3(1,0,1));
  _curColor = glm::vec3(1,0,1);
  glGenBuffers(1, &_VCO);
  glBindBuffer(GL_ARRAY_BUFFER, _VCO);
  glEnableVertexAttribArray(1);	
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &_uvID);
  glBindBuffer(GL_ARRAY_BUFFER, _uvID);
  glEnableVertexAttribArray(2);	
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * parser._faceUV.size(), parser._faceUV.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &_normalID);
  glBindBuffer(GL_ARRAY_BUFFER, _normalID);
  glEnableVertexAttribArray(3);	
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);//size 열의 개수
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * parser._faceNormal.size(), parser._faceNormal.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

}

void Simulator::blendingRatioUpdate(float delta)
{
  if (_blendingTriger == true)
    _blendingRatio += delta;
  else
    _blendingRatio -= delta;

  if (_blendingRatio >= 1.0f)
    _blendingRatio = 1;
  else if (_blendingRatio <= 0.0f)
    _blendingRatio = 0;
}
#include <cstdlib>
void Simulator::initialize(const char* objFilePath)
{
  std::filesystem::path objPath(objFilePath);
  std::filesystem::path TexturePath("./resources/uvmap.DDS");

  if (std::filesystem::exists(objPath) == false)
  {
    std::cerr << "can't find obj file " << std::endl;
    std::cout << objFilePath << std::endl;
    std::cout << getcwd(NULL, 0) << std::endl;
    exit(1);
  }
  if (std::filesystem::exists(TexturePath) == false)
  {
    std::cerr << "can't find dds file " << std::endl;
    exit(1);
  }

  Parser parser(std::filesystem::canonical(objPath));
  DDSLoder textureLoder(std::filesystem::canonical(TexturePath));
  
  _worldTranslate = glm::mat4(1.0f);
  parser.initialize(_mtlStruct);
  moveToCenter(parser);
  _textureID = textureLoder.loadDDS();
  
  if (_textureID == 0)
  {
    std::cerr << "texture file error\n";
    exit(1);
  }
  sendDataToGpuBuffer(parser);
}

void Simulator::draw(void)
{
  glBindTexture(GL_TEXTURE_2D, _textureID);
  glBindVertexArray(_VAO);
  glDrawArrays(GL_TRIANGLES, 0, _vertexSize);
  glBindVertexArray(0);
}

void Simulator::update(float delta, const Shader& shader)
{
  std::vector<glm::vec3> colors;

  blendingRatioUpdate(delta);
  colors.resize(_vertexSize, _curColor);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VCO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);;
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  shader.setFloat("blendingRatio", _blendingRatio);
  shader.setVec3("Ka", _mtlStruct._Ka);
  shader.setVec3("Kd", _mtlStruct._Kd);
  shader.setVec3("Ks", _mtlStruct._Ks);
  shader.setFloat("Ns", _mtlStruct._Ns);
}