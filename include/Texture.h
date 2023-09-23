#pragma once
#include "Common.h"
#include "EnumHeader.h"

class Texture : Noncopyable
{
  private:
    uint32 _textureID[10];
    std::vector<std::string> _textureFilePath;
  public:
    Texture();
    ~Texture(){};
    void initialize();
    void activeTexture();
    uint32 getBoxTextrue(BoxTexture id){
      return _textureID[id];
    }
};

