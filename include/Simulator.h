#pragma once
#include "Common.h"
#include "EnumHeader.h"
#include "MtlStruct.h"
class Parser;
class Shader;
class Simulator : Noncopyable
{
    private:
        void sendDataToGpuBuffer(const Parser& parser);
        void moveToCenter(Parser& parser);
        void blendingRatioUpdate(float delta);
        uint32 _VAO, _VBO, _VCO, _textureID, _uvID, _normalID;
        uint32 _vertexSize;
        glm::vec3 _curColor;
        float      _blendingRatio = 0;

    private:
        bool _blendingTriger = false;
        float _ns;
        glm::vec3 _ka;
        glm::vec3 _kd;
        glm::vec3 _ks;
        glm::vec3 _ni;
        float _d;
        uint8 _illum;

    public:
        glm::mat4 _worldTranslate;
        MtlStruct  _mtlStruct;
        NUMINPUT _numInput;
        Simulator(){}
        ~Simulator(){}
        
        void initialize(const char* objFilePath);
        void update(float delta, const Shader& shader);
        void draw(void);
        void colorBlendingTriger(NUMINPUT input);
        void moveObjectThreeAxis(glm::vec3 move);
};

// std::ostream& operator<<(std::ostream& os, const std::pair<Animation*, TimeNode>& ref);