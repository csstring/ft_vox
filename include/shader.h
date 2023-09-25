#pragma once
#include "Common.h"


class Shader : Noncopyable
{   
    public :
        explicit    Shader(const char* vertexRelativePath,const char* fragmentRelativePath, const char* geometryRelativePath);
        void        initialize(void);
        void        use(void);
        void        setMat4(const std::string &name, glm::mat4 mat4) const;
        void        setUint(const std::string &name, unsigned int index) const;
        void        setVec3(const std::string &name, glm::vec3 vec3) const;
        void        setFloat(const std::string &name, float index) const;
        void        textureUpdate();
    public :
        uint32      _programId;

    private:
        std::string _vertexFullPath;
        std::string _fragmentFullPath;
        std::string _geometryFullPath;
};
