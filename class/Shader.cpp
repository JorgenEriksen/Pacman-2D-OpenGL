/**
*   Main file for our Pacman Game. This project is mainly a OpenGL project.
*	There are 3 external libraries that help support this application.
*   Currently there are four ghosts and one pacman with one single map.
*	It is possible to add more levels as that code is dynamic in that regard.
*
*  @name Pacman.exe
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Shader.h"

/**
*   PlaceHolder constuctor for shader objects.
*
*/
Shader::Shader()
{

}
/**
*   Prepares a shaderfile to be read.
*
*  @param     filepath - The filepath to be read for vertex and fragment shader.
*  @see       parseShader.
*  @return    Sends back the ID number of a shader.
*/
GLuint Shader::shaderFile(const string& filepath)
/*: m_FilePath(filepath), rendererId(0)*/
{
	ShaderProgramSource source = parseShader(filepath);
	rendererId = createShader(source.VertexSource, source.FragmentSource);

	return rendererId;
}

/**
*   Parses (reads) the shaderfile.
*
*  @param     filepath - object for reading from file
*  @return    Two stringstream objects.
*/
ShaderProgramSource Shader::parseShader(const string& filepath)
{
	ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	string line;
	stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("shader") != string::npos)
		{
			if (line.find("vertex") != string::npos)
			{
				//set mode to vertex
				type = ShaderType::VERTEX;
			}

			else if (line.find("fragment") != string::npos)
			{
				//set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

/**
*   Recieves the shader and compiles the shaderprogram.
*
*  @param     type - decides the shader is fragment or vertex.
*  @param     source - the path for the shader file.
*  @return    The ID of the compiled shaderprogram.
*/
unsigned int Shader::compileShader(unsigned int type, const string& source)
{
	auto id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	
	auto result = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		auto length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << "\n";
		cout << message << "\n";

		glDeleteShader(id);

		return 0;
	}

	return id;
}


/**
*   Creates the final shader to be attached to the objects.
*
*  @param     vertexShader - recieved shader to be used.
*  @param     fragmentShader - recieved shader to be used.
*  @return    the finished shaderprogram.
*/
GLuint Shader::createShader(const string& vertexShader, const string& fragmentShader)
{
	unsigned int program = glCreateProgram();

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs); // memory managment, as soon as bound you can delete.
	glDeleteShader(fs);

	return program;
}
/**
*   Manages basic memory of the shader class.
*
*/
Shader::~Shader()
{
	glDeleteProgram(rendererId);
}

/**
*   Binds the program that is created with the ID from the class.
*
*/
void Shader::bind() const
{
	glUseProgram(rendererId);
}

/**
*   A seter for 4 float uniform location.
* 
* 
*  @param     name - location of the uniform.
*  @param     value - number of elements to be modified.
*  @param     model - the path for the .png file.
*/
void Shader::setUniform4v(const string& name, int value, glm::mat4 model)
{
	glUniformMatrix4fv(getUniformLocation(name), value, GL_FALSE, glm::value_ptr(model));
}

/**
*  A setter for 1 int vector.  
* 
*  @param     name - the name of the uniform in the hader file.
*  @param     value - setting the x value of the vector.
*/
void Shader::setUniform1i(const string& name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}
/**
*   A setter for the 1 float vector.
*
*  @param     name - the name of the uniform in the hader file.
*  @param     value - setting the x value of the vector.
*/
void Shader::setUniform1f(const string& name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}

/**
*   A setter for the 4 float vector.
*
*  @param     name - name of the uniform that is read.
*  @param     v0   - x place in vector4
*  @param     v1   - y place in vector4
*  @param     v2   - z place in vector4
*  @param     v3   - w place in vector4
*  @return    A struct that makes up the entire texture data.
*/
void Shader::setUniform4f(const string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

/**
*   retrieves the location of the uniform in the shader file.
*
*  @param     name - Taken in for reading the location in the shader.
*  @return    location that is defined in shaderfile (laylout location).
*/
int Shader::getUniformLocation(const string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
	{
		return uniformLocationCache[name];
	}

	int location = glGetUniformLocation(rendererId, name.c_str());

	if (location == -1) {
		cout << "Warning: uniform " << name << " doesn't exist!" << endl;
	}

	uniformLocationCache[name] = location;


	return location;
}

/**
*  check if compilation failed for shaderprogram.
*/
bool Shader::compileShader()
{
	return false;
}
