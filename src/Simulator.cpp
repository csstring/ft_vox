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
  glBindTexture(GL_TEXTURE_2D, _textureID);
  for (const auto& it : _scean->_chunkDatas)
  {
    glBindVertexArray(it.second._VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, _scean->getVectexSize(), it.second.amount);
    glBindVertexArray(0);
  }
}

void Simulator::update(float delta, const Shader& shader)
{
}