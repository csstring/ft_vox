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
Camera      _camera;

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

    Simulator simulator;
    simulator.initialize(av[1]);
    // camera mouse call
    glfwSetFramebufferSizeCallback(window._window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window._window, mouse_callback);
    glfwSetScrollCallback(window._window, scroll_callback);
    glfwSetInputMode(window._window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float radias = 0;
    std::chrono::steady_clock::time_point curTime = getCurTimePoint();
    std::chrono::steady_clock::time_point beforeTime = getCurTimePoint();
    while (window.isWindowClose() == false)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        radias += 0.01;
        shader.use();
        window.processInput(simulator, _camera);
        glm::mat4 projection = glm::perspective(glm::radians(_camera._fov), (float)WINDOW_WITH / (float)WINDOW_HEIGHT, _camera._zNear, _camera._zFar);
        glm::mat4 rotation = glm::rotate(radias, glm::vec3(0,1,0));
        _camera.update();
        shader.setMat4("projection", projection);
        shader.setMat4("view", _camera._view);
        // shader.setMat4("model", simulator._worldTranslate * rotation);
        shader.setVec3("LightPosition_worldspace", glm::vec3(4,4,4));
        curTime = getCurTimePoint();
        simulator.update(getMilisecondTime(curTime, beforeTime) / float(2000), shader, _camera);
        simulator.draw();
        beforeTime = curTime;
        window.bufferSwap();
        glfwPollEvents();
    }
    glfwTerminate();
}
