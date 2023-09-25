#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h"
#include "Window.h"
#include "shader.h"
#include "Simulator.h"
#include "Camera.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtx/transform.hpp"
#include "Mygui.h"

Camera      _camera;
Mygui       mygui;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (_camera._isFirst)
    {
        _camera._lastX = xpos;
        _camera._lastY = ypos;
        _camera._isFirst = false;
    }

    float xoffset = xpos - _camera._lastX;
    float yoffset = _camera._lastY - ypos;

    _camera._lastX = xpos;
    _camera._lastY = ypos;

    _camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


int main(int ac, char** av) 
{

    if (ac < 2){
        std::cerr << "input error : obj file path missing\n";
        return 1;
    } else if (ac > 2) {
        std::cerr << "input error : to many argument\n";
        return 1;
    }

    Window window;
    Shader shader("./shaderSource/VertexShader.glsl","./shaderSource/FragmentShader.glsl");
    
    window.initialize();
    _camera.initialize();
    shader.initialize();
    mygui.initialize(window._window);

    Simulator simulator;
    simulator.initialize(av[1]);
    // camera mouse call
    glfwSetFramebufferSizeCallback(window._window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window._window, mouse_callback);
    glfwSetScrollCallback(window._window, scroll_callback);
    glfwSetInputMode(window._window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::chrono::steady_clock::time_point curTime = getCurTimePoint();
    std::chrono::steady_clock::time_point beforeTime = getCurTimePoint();
    while (window.isWindowClose() == false)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mygui.update();
        shader.use();
        double delta = getMilisecondTime(curTime, beforeTime) * 0.05;
        window.processInput(delta , _camera);
        _camera.update();
        glm::mat4 projection = glm::perspective(glm::radians(_camera._fov), (float)WINDOW_WITH / (float)WINDOW_HEIGHT, _camera._zNear, _camera._zFar);
 
        shader.setMat4("projection", projection);
        shader.setMat4("view", _camera._view);
        shader.setVec3("LightPosition_worldspace", glm::vec3(4,4,4));
        simulator.update(delta, shader, _camera);
        shader.textureUpdate();
        simulator.draw();
        beforeTime = curTime;
        curTime = getCurTimePoint();
        mygui.render();
        window.bufferSwap();
        glfwPollEvents();
    }
    glfwTerminate();
}
