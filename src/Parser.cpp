#include "Parser.h"
#include <unistd.h>
#include "MtlStruct.h"

void Parser::saveVertex(const std::vector<std::string>& v)
{
  if (v.size() != 4)
  {
    std::cerr << "vertex size error "<< std::endl;
    exit(1);
  }
  try
  {
    glm::vec4 vertex(std::stof(v[1]), std::stof(v[2]), std::stof(v[3]), 1);
    _vertices.push_back(vertex);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
}

void Parser::saveUV(const std::vector<std::string>& v)
{
  if (v.size() != 3)
  {
    std::cerr << "UV size error "<< std::endl;
    exit(1);
  }
  try
  {
    glm::vec2 vertex(std::stof(v[1]), std::stof(v[2]));
    _uv.push_back(vertex);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
}

void Parser::saveNormal(const std::vector<std::string>& v)
{
  if (v.size() != 4)
  {
    std::cerr << "normal size error "<< std::endl;
    exit(1);
  }
  try
  {
    glm::vec3 vertex(std::stof(v[1]), std::stof(v[2]), std::stof(v[3]));
    _normal.push_back(vertex);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
}
/*
1
1/1/1
1//1
1/1
*/
void Parser::saveCase(void)
{
  if (_uv.empty() == true && _normal.empty() == true)
    _Case = 0;
  else if (_uv.empty() == false && _normal.empty() == false)
    _Case = 1;
  else if (_uv.empty() == true && _normal.empty() == false)
    _Case = 2;
  else if (_uv.empty() == false && _normal.empty() == true)
    _Case = 3;
  else
  {
    std::cerr << "face format error\n";
    exit(1);
  }
}

void Parser::saveIndex(const std::vector<std::string>& v, uint32 count)
{
  _uvIndex.clear();
  _normalIndex.clear();
  _posIndex.clear();

  for (int i =1; i < count; ++i)
  {
    uint32 posi, uvi, normali, matches;

    switch (_Case)
    {
    case 0:
      matches = sscanf(v[i].c_str(), "%d", &posi);
      if (matches != 1){
        std::cerr << "face format error\n";
        exit(1);
      }
      _posIndex.push_back(posi - 1);
      _uvIndex.push_back(posi - 1);
      _normalIndex.push_back(posi - 1);
      break;
    case 1:
      matches = sscanf(v[i].c_str(), "%d/%d/%d", &posi, &uvi, &normali);
      if (matches != 3){
        std::cerr << "face format error\n";
        exit(1);
      }
      _posIndex.push_back(posi - 1);
      _uvIndex.push_back(uvi - 1);
      _normalIndex.push_back(normali - 1);
      break;
    case 2:
      matches = sscanf(v[i].c_str(), "%d//%d", &posi, &normali);
      if (matches != 2){
        std::cerr << "face format error\n";
        exit(1);
      }
      _posIndex.push_back(posi - 1);
      _uvIndex.push_back(posi - 1);
      _normalIndex.push_back(normali - 1);
      break;
    case 3:
      matches = sscanf(v[i].c_str(), "%d/%d", &posi, &uvi);
      if (matches != 2){
        std::cerr << "face format error\n";
        exit(1);
      }
      _posIndex.push_back(posi - 1);
      _uvIndex.push_back(uvi - 1);
      _normalIndex.push_back(posi - 1);
      break;
    }
  }
}

void Parser::saveTriangleVertex(const std::vector<std::string>& v)
{
  if (_vertices.empty() == false)
  {
    _facePos.push_back(_vertices[_posIndex[0]]);
    _facePos.push_back(_vertices[_posIndex[1]]);
    _facePos.push_back(_vertices[_posIndex[2]]);
  }
  if (_uv.empty() == false)
  {
    _faceUV.push_back(_uv[_uvIndex[0]]);
    _faceUV.push_back(_uv[_uvIndex[1]]);
    _faceUV.push_back(_uv[_uvIndex[2]]);
  }
  if (_normal.empty() == false)
  {
    _faceNormal.push_back(_normal[_normalIndex[0]]);
    _faceNormal.push_back(_normal[_normalIndex[1]]);
    _faceNormal.push_back(_normal[_normalIndex[2]]);
  } else {
    _faceIndex.push_back(_normalIndex[0]);
    _faceIndex.push_back(_normalIndex[1]);
    _faceIndex.push_back(_normalIndex[2]);
  }
}

void Parser::saveQuadVertex(const std::vector<std::string>& v)
{
  saveTriangleVertex(v);
  if (_vertices.empty() == false)
  {
    _facePos.push_back(_vertices[_posIndex[2]]);
    _facePos.push_back(_vertices[_posIndex[3]]);
    _facePos.push_back(_vertices[_posIndex[0]]);
  }
  if (_uv.empty() == false)
  {
    _faceUV.push_back(_uv[_uvIndex[2]]);
    _faceUV.push_back(_uv[_uvIndex[3]]);
    _faceUV.push_back(_uv[_uvIndex[0]]);
  }
  if (_normal.empty() == false)
  {
    _faceNormal.push_back(_normal[_normalIndex[2]]);
    _faceNormal.push_back(_normal[_normalIndex[3]]);
    _faceNormal.push_back(_normal[_normalIndex[0]]);
  } else {
    _faceIndex.push_back(_normalIndex[2]);
    _faceIndex.push_back(_normalIndex[3]);
    _faceIndex.push_back(_normalIndex[0]);
  }
}

void Parser::saveFace(const std::vector<std::string>& v)
{
  if (v.size() <= 3)
  {
    std::cerr << "face size error "<< std::endl;
    exit(1);
  }
  saveIndex(v, v.size());
  if (v.size() == 4)
    saveTriangleVertex(v);
  else if (v.size() == 5)
    saveQuadVertex(v);
}

void Parser::generateUV(void) 
{
    float minZ = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();

    for (const auto& vertex : _vertices) {
        minZ = std::min(minZ, vertex.z);
        minY = std::min(minY, vertex.y);
        maxZ = std::max(maxZ, vertex.z);
        maxY = std::max(maxY, vertex.y);
    }

    for (const auto& vertex : _vertices) {
        glm::vec2 uv((vertex.z - minZ) / (maxZ - minZ), (vertex.y - minY) / (maxY - minY));
        _uv.push_back(uv);
    }
}

void Parser::generateNormal(void) //fix me
{
  std::vector<glm::vec3> normals;

  normals.resize(_vertices.size(), glm::vec3(0.0f));
  for (int i =0; i < _faceIndex.size(); i += 3)
  {
    glm::vec3 v1 = glm::vec3(_vertices[_faceIndex[i]]);
    glm::vec3 v2 = glm::vec3(_vertices[_faceIndex[i+1]]);
    glm::vec3 v3 = glm::vec3(_vertices[_faceIndex[i+2]]);

    glm::vec3 normal = glm::normalize(glm::cross(v2-v1, v3-v1));
    normals[_faceIndex[i]] += normal;
    normals[_faceIndex[i+1]] += normal;
    normals[_faceIndex[i+2]] += normal;
  }

  for (auto& it : normals)
    it = glm::normalize(it);
  
  for (int i = 0; i < _faceIndex.size(); ++i)
  {
    _faceNormal.push_back(normals[_faceIndex[i]]);
  }
}

void Parser::parseObj(std::ifstream& ifs)
{
  std::string buffer;
  std::vector<std::string> v;
  std::filesystem::path mtlPath("./resources/");

  while (ifs.good())
  {
    std::getline(ifs, buffer);
    v = ft_split(buffer);

    if (v.empty() == true)
      continue;
    
    if (v[0] == "#")
      continue;
    else if (v[0] == "mtllib")
    {
      mtlPath.append(v[1]);
      if (std::filesystem::exists(mtlPath) == false)
      {
        std::cerr << "can't find mtl file " << std::endl;
        std::cerr << "render defualt mtl env!\n";
      }
      else
        _mtlFilePath.assign(std::filesystem::canonical(mtlPath));
    }
    else if (v[0] == "v")
      saveVertex(v);
    else if (v[0] == "vt")
      saveUV(v);
    else if (v[0] == "vn")
      saveNormal(v);
    else if (v[0] == "f")
    {
      if (_posIndex.empty() == true)
        saveCase();
      if (_uv.empty() == true)
        generateUV();
      saveFace(v);
    }
  }
  if (_normal.empty() == true)
    generateNormal();

  ifs.close();
  if (_mtlFilePath.empty() == true)
  {
    mtlPath.append("teapot2.mtl");
    if (std::filesystem::exists(mtlPath) == false)
    {
      std::cerr << "defualt mtl file can't find, render fail" << std::endl;
      exit(1);
    }
    _mtlFilePath.assign(std::filesystem::canonical(mtlPath));
  }
}

void Parser::parseMtl(std::ifstream& ifs, MtlStruct& mtlStruct)
{
  std::string buffer;
  std::vector<std::string> v;

  while (ifs.good())
  {
    std::getline(ifs, buffer);
    v = ft_split(buffer);

    if (v.empty() == true)
      continue;

    try
    {
      if (v[0] == "#")
        continue;
      else if (v[0] == "Ns")
        mtlStruct._Ns = std::stof(v[1]);
      else if (v[0] == "Ka")
        mtlStruct._Ka = glm::vec3(std::stof(v[1]),std::stof(v[2]),std::stof(v[3]));
      else if (v[0] == "Kd")
        mtlStruct._Kd = glm::vec3(std::stof(v[1]),std::stof(v[2]),std::stof(v[3]));
      else if (v[0] == "Ks")
        mtlStruct._Ks = glm::vec3(std::stof(v[1]),std::stof(v[2]),std::stof(v[3]));
      else if (v[0] == "Ni")
        mtlStruct._Ni = std::stof(v[1]);
      else if (v[0] == "d")
        mtlStruct._d = std::stof(v[1]);
      else if (v[0] == "illum")
        mtlStruct._illum = std::stof(v[1]);
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      exit(1);
    }
  }
  ifs.close();
}

void Parser::initialize(MtlStruct& mtlStruct)
{
  std::ifstream ifs(_objFilePath);

  if (ifs.is_open() == false)
  {
    std::cerr << "Obj file path fail : "<< _objFilePath << std::endl;
    exit(1);
  }

  parseObj(ifs);
  if (_facePos.empty() || _faceNormal.empty() || _faceUV.empty())
  {
    std::cerr << "obj file format error\n";
    exit(1);
  }

  std::ifstream mtlIfs(_mtlFilePath);
  if (mtlIfs.is_open() == false)
  {
    std::cerr << "defualt mtl file can't open, render fail" << std::endl;
    exit(1);
  }
  parseMtl(mtlIfs, mtlStruct);
}