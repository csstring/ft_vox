#pragma once

#include "Common.h"

struct GLFWwindow;
class Mygui
{
  private:
    /* data */
  public:
    void initialize(GLFWwindow* window);
    void update(void);
    void render(void);
    Mygui(){};
    ~Mygui();
};

