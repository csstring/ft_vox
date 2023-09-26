#pragma once
#include "Common.h"

const float cameraSpeed = 0.5f;
class Camera
{

    public:
        Camera() : _yaw(-90.0f), _pitch(0.0f), _movementSpeed(2.5f), _mouseSensitivity(0.1f), _fov(80.0f), _isFirst(true)
        {};
        ~Camera(){};
        void update(void);
        void initialize(void);
        void ProcessMouseScroll(float yoffset);
        void updateCameraVectors(void);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);

    private:
        float _yaw;
        float _pitch;
        float _movementSpeed;
        float _mouseSensitivity;
    
    public:
        float _fov;
        bool _isFirst;
        float _lastX;
        float _lastY;
        float _zNear = 0.1;
        float _zFar = 100;
        glm::vec3   _worldUp;
        glm::vec3   _cameraUp;
        glm::vec3   _cameraPos;
        glm::vec3   _cameraFront;
        glm::vec3   _cameraRight;
        glm::mat4   _view;
};  