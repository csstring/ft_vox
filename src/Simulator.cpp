#include "Simulator.h"
#include "Parser.h"
#include "GL/glew.h"
#include <random>
#include "DDSLoder.h"
#include <algorithm>
#include "shader.h"
#include <unistd.h>
#include "Scean.h"
#include "GLM/ext.hpp"
#include "GLM/gtx/string_cast.hpp"
#include "Scean.h"
#include "Camera.h"

Simulator::~Simulator()
{
  if (_scean != nullptr)
    delete _scean;
}

void Simulator::initialize(const char* objFilePath)
{
  std::filesystem::path objPath(objFilePath);
  std::filesystem::path TexturePath("./resources/uvmap.DDS");

  if (std::filesystem::exists(objPath) == false)
  {
    std::cerr << "can't find obj file " << std::endl;
    std::cout << objFilePath << std::endl;
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
  _textureID = textureLoder.loadDDS();
  
  if (_textureID == 0)
  {
    std::cerr << "texture file error\n";
    exit(1);
  }
  _scean = new Scean(parser._facePos, parser._faceUV);
  _scean->initialize();
}

void Simulator::draw(void)
{
  uint32 totalData = 0;
  uint32 curData = 0;
  glBindTexture(GL_TEXTURE_2D, _textureID);
  for (const auto& it : _scean->_chunkDatas)
  {
    glBindVertexArray(it.second._VAO);
    glBindBuffer(GL_ARRAY_BUFFER, it.second.chunkID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * it.second.buffer.size(), it.second.buffer.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    totalData += it.second._transForm.size();
    curData += it.second.buffer.size();
    glDrawArraysInstanced(GL_TRIANGLES, 0, _scean->getVectexSize(), it.second.buffer.size());
    glBindVertexArray(0);
  }
  std::cout << "total : " << totalData << "\n";
  std::cout << "cur : " << curData << "\n";
}

void Simulator::update(float delta, const Shader& shader,const Camera& camera)
{
  _scean->update(camera);
}