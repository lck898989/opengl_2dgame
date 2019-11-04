#include <glad/glad.h>
#include <climits>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shader.h"
using namespace std;
Shader::Shader(const char* vertexPath, const char* fragmentPath) {

}
Shader::Shader() {

}
Shader::~Shader() {

}
Shader Shader::use() {
	try {
		glUseProgram(ID);
		return *this;
	}
	catch (runtime_error e) {
		cout << e.what() << endl;
	}
}
Shader Shader::unUse() {
	glDeleteProgram(ID);
	return *this;
}
void Shader::setf(float z) const {
	float m = z;
}
void Shader::setBool(const string &name, bool value) const
{
	//glUniform
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec3(const string &name, const glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setMat4(const string &name, const glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setInteger(const string& name, uint value, bool useShader) {
	if (useShader) {
		this->use();
	}
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setVec2(const string& name, const glm::vec2 value) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}void Shader::setVec4(const string& name, const glm::vec4 value) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
// 编译连接shader
void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {   
	uint sVertex, sFragment, gShader;
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex,"VERTEX");
	// 片段
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment,"FRAGMENT");
	// 几何着色器
	if (geometrySource != nullptr) {
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader,1,&geometrySource,NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader,"GEOMETRY");
	}
	this->ID = glCreateProgram();
	glAttachShader(this->ID,sVertex);
	glAttachShader(this->ID,sFragment);
	if (geometrySource != nullptr) {
		glAttachShader(this->ID, gShader);
	}
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID,"PROGRAM");
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr) {
		glDeleteShader(gShader);
	}
}
void Shader::checkCompileErrors(unsigned int shader, string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}