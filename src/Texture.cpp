#include "Texture.h"
#include "GL/glew.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
  _textureFilePath = {
    "./minecraft_asset/001-dirt.png",
    "./minecraft_asset/002-grass_side.png",
    "./minecraft_asset/003-grass_top.png",
    "./minecraft_asset/007-snow.png",
    "./minecraft_asset/008-stone.png",
  };
}

void Texture::initialize()
{
  uint32 i = 0;
  for (const std::string& str : _textureFilePath)
  {
    int width, height, nrChannels;
    GLint		intFormat;
	  GLenum		format;
    unsigned char *data = stbi_load(str.c_str(), &width, &height, &nrChannels,0);
    if (nrChannels == 1) {
			intFormat = GL_RED;
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			intFormat = true ? GL_SRGB : GL_RGB;
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			intFormat = true ? GL_SRGB_ALPHA : GL_RGBA;
			format = GL_RGBA;
		}
		else {
			exit(1);
		}
    glGenTextures(1, &_textureID[i]);
    glBindTexture(GL_TEXTURE_2D, _textureID[i++]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      std::cout << "texture file load fail" << std::endl;
    }

    stbi_image_free(data);
  }
}

void Texture::activeTexture()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureID[BoxTexture::DIRT]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _textureID[BoxTexture::GRASS_SIDE]);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _textureID[BoxTexture::GRASS_TOP]);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, _textureID[BoxTexture::SNOW_SNOW_BLOCK]);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, _textureID[BoxTexture::SNOW_STONE]);
}