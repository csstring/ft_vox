#include "Window.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Simulator.h"
#include "Camera.h"
#include "EnumHeader.h"

void Window::initialize(void)
{
    if (glfwInit() == false) 
        ft_assert("glfwInit false");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    _window = glfwCreateWindow(WINDOW_WITH, WINDOW_HEIGHT, "model", NULL, NULL);  
    if (_window == NULL) 
    {
        glfwTerminate();
        ft_assert("glfw create Window failed");
    }
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwMakeContextCurrent(_window);
    if (glewInit() != GLEW_OK) 
        ft_assert("glew init failed");

    clearColorSetUp();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDepthFunc(GL_LESS);
    glfwSwapInterval(1);

}

void Window::processInput(Simulator& simulator, Camera& camera)
{
    static int currentBlendingState,previousBlendingState;
    currentBlendingState = glfwGetKey(_window, GLFW_KEY_1);

    if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        camera._cameraPos += cameraSpeed * camera._cameraFront;
    if (glfwGetKey(_window, GLFW_KEY_S ) == GLFW_PRESS)
        camera._cameraPos -= cameraSpeed * camera._cameraFront;
    if (glfwGetKey(_window, GLFW_KEY_A ) == GLFW_PRESS)
        camera._cameraPos -= camera._cameraRight * cameraSpeed;
    if (glfwGetKey(_window, GLFW_KEY_D ) == GLFW_PRESS)
        camera._cameraPos += camera._cameraRight * cameraSpeed;
    
    if (currentBlendingState == GLFW_PRESS && previousBlendingState == GLFW_RELEASE)
    {
        previousBlendingState = currentBlendingState;
    } else if (currentBlendingState == GLFW_RELEASE && previousBlendingState == GLFW_PRESS) {
        previousBlendingState = currentBlendingState;
        simulator.colorBlendingTriger(NUMINPUT::NUM1);
    }

    if (glfwGetKey(_window, GLFW_KEY_U ) == GLFW_PRESS)
    {
        simulator.moveObjectThreeAxis(glm::vec3(0,0,-0.1));
    }
    if (glfwGetKey(_window, GLFW_KEY_J ) == GLFW_PRESS)
    {
        simulator.moveObjectThreeAxis(glm::vec3(0,0,0.1));
    }
    if (glfwGetKey(_window, GLFW_KEY_H ) == GLFW_PRESS)
    {
        simulator.moveObjectThreeAxis(glm::vec3(-0.1,0,0));
    }
    if (glfwGetKey(_window, GLFW_KEY_K ) == GLFW_PRESS)
    {
        simulator.moveObjectThreeAxis(glm::vec3(0.1,0,0.0));
    }
    if (glfwGetKey(_window, GLFW_KEY_Y ) == GLFW_PRESS)
    {
        simulator.moveObjectThreeAxis(glm::vec3(0,0.1,0.00));
    }
    if (glfwGetKey(_window, GLFW_KEY_I ) == GLFW_PRESS)
    {
        simulator.moveObjectThreeAxis(glm::vec3(0,-0.1,0));
    }
}

void Window::clearColorSetUp(float r, float g, float b, float a)
{
    glClearColor(a,g,b,a);
}

bool Window::isWindowClose(void)
{
    if (glfwWindowShouldClose(_window) == 0)
        return false;
    return true;
}

void Window::bufferSwap(void)
{
    glfwSwapBuffers(_window);
}