#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <unordered_map>

using namespace std;

struct ShaderProgramSource
{
	string VertexSource; // easier management with struct.
	string FragmentSource;
};

class Shader
{
private:
	
	unordered_map<string, int> uniformLocationCache;
	string filePath;
	unsigned int rendererId;
	ShaderProgramSource parseShader(const string& filepath);
	GLuint compileShader(unsigned int type, const string& source);
	int getUniformLocation(const string& name);
	bool compileShader();
	//caching for uniform

public:

	GLuint shaderFile(const string& filepath);
	Shader();
	~Shader();
	void bind() const;
	//Set uniforms
	void setUniform1i(const string& name, int value);
	void setUniform1f(const string& name, float value);
	void setUniform4v(const string& name, int value, glm::mat4 model);
	void setUniform4f(const string& name, float v0, float v1, float v2, float v3);
	GLuint createShader(const string& vertexShader, const string& fragmentShader);

};