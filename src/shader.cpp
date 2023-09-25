
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "GL/glew.h"
#include "shader.h"

Shader::Shader(const char* vertexRelativePath,const char* fragmentRelativePath, const char* geometryRelativePath)
{
	std::filesystem::path vertexPath(vertexRelativePath);
  std::filesystem::path fragmentPath(fragmentRelativePath);
	std::filesystem::path geometryPath(geometryRelativePath);

	_vertexFullPath.assign(std::filesystem::canonical(vertexPath));
	_fragmentFullPath.assign(std::filesystem::canonical(fragmentPath));
	_geometryFullPath.assign(std::filesystem::canonical(geometryPath));
}

void Shader::initialize() 
{

	// Create the shaders
	uint32 VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	uint32 FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	uint32 GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(_vertexFullPath, std::ios::in);

	if (VertexShaderStream.is_open()) 
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else 
		ft_assert("Impossible to open vertexShader");
	std::string GeometryShaderCode;
	std::ifstream GeometryShaderStream(_geometryFullPath, std::ios::in);
	if (GeometryShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << GeometryShaderStream.rdbuf();
		GeometryShaderCode = sstr.str();
		GeometryShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(_fragmentFullPath, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}


	int32 Result = GL_FALSE;
	int32 InfoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling shader : " << _vertexFullPath << std::endl;
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) 
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::string ErrorMessage(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}

	std::cout << "Compiling shader : " << _geometryFullPath << std::endl;
	char const* GeometrySourcePointer = GeometryShaderCode.c_str();
	glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
	glCompileShader(GeometryShaderID);

	// Check Geometry Shader
	glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
		std::string ErrorMessage(GeometryShaderErrorMessage.begin(), GeometryShaderErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}

	// Compile Fragment Shader
	std::cout << "Compiling shader : " << _fragmentFullPath << std::endl;
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::string ErrorMessage(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}


	// Link the program
	std::cout << "Linking program" << std::endl;
	_programId = glCreateProgram();
	glAttachShader(_programId, VertexShaderID);
	glAttachShader(_programId, GeometryShaderID);
	glAttachShader(_programId, FragmentShaderID);
	glLinkProgram(_programId);

	// Check the program
	glGetProgramiv(_programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(_programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::string ErrorMessage(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}


	glDetachShader(_programId, VertexShaderID);
	glDetachShader(_programId, FragmentShaderID);
	glDetachShader(_programId, GeometryShaderID);
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	glDeleteShader(GeometryShaderID);
}

void Shader::use(void)
{
	glUseProgram(_programId);
}

void Shader::setMat4(const std::string &name, glm::mat4 mat4) const
{
    glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()),1, false, &mat4[0].x);
}

void Shader::setVec3(const std::string &name, glm::vec3 vec3) const
{
    glUniform3fv(glGetUniformLocation(_programId, name.c_str()), 1, &vec3.x);
}

void Shader::setUint(const std::string &name, unsigned int index) const
{
    glUniform1ui(glGetUniformLocation(_programId, name.c_str()), index);
}

void Shader::setFloat(const std::string &name, float index) const
{
    glUniform1f(glGetUniformLocation(_programId, name.c_str()), index);
}

void Shader::textureUpdate()
{
	GLint dirtLocation = glGetUniformLocation(_programId, "dirt");
	GLint grassSideLocation = glGetUniformLocation(_programId, "grass_side");
	GLint grassTopLocation = glGetUniformLocation(_programId, "grass_top");
	GLint snowBlockLocation = glGetUniformLocation(_programId, "SNOW_SNOW_BLOCK");
	GLint snowStoneLocation = glGetUniformLocation(_programId, "SNOW_STONE");

	glUniform1i(dirtLocation, 0); // Bind to texture unit 0
	glUniform1i(grassSideLocation, 1); // Bind to texture unit 1
	glUniform1i(grassTopLocation, 2); // Bind to texture unit 2
	glUniform1i(snowBlockLocation, 3); // Bind to texture unit 3
	glUniform1i(snowStoneLocation, 4); // Bind to texture unit 4
}