#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
typedef unsigned int uint;
class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader();
	~Shader();
	// activate the shader
	// ------------------------------------------------------------------------
	Shader use();
	Shader unUse();
	void compile(const char* vertexSource,const char* fragmentSource,const char* geometrySource);
	// utility uniform functions
	// -----------------------------------------------------
	void setf(float z) const;
	void setMat4(const string &name, glm::mat4 value) const;
	void setVec3(const string &name, glm::vec3 value) const;
	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value) const;
	void setFloat(const string &name, float value) const;
	void setVec2(const string &name, glm::vec2 value) const;
	void setVec4(const string &name, glm::vec4 value) const;
	void setInteger(const string& name, uint value, bool useShader);
private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, string type);
};
#endif