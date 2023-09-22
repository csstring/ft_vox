#pragma once
#include "Common.h"
#include "EnumHeader.h"
#include "MtlStruct.h"
class Parser;
class Shader;
class Scean;
class Camera;
class Simulator : Noncopyable
{
    private:
        uint32 _textureID;
        uint32 _vertexSize;
        
    private:
        Scean* _scean = nullptr;

    public:
        glm::mat4 _worldTranslate;
        MtlStruct  _mtlStruct;
        Simulator(){};
        ~Simulator();
        
        void initialize(const char* objFilePath);
        void update(float delta, const Shader& shader,const Camera& camera);
        void draw(void);
};

// std::ostream& operator<<(std::ostream& os, const std::pair<Animation*, TimeNode>& ref);