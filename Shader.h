#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace glm;
using namespace std;

class ShaderProgram
{
public:
	static void printProgramLog(GLuint program)
	{
		//Make sure name is shader
		if (glIsProgram(program))
		{
			//Program log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			//Allocate string
			char* infoLog = new char[maxLength];

			//Get info log
			glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
			if (infoLogLength > 0)
			{
				//Print Log
				printf("%s\n", infoLog);
			}

			//Deallocate string
			delete[] infoLog;
		}
		else
		{
			printf("Name %d is not a program\n", program);
		}
	}

	static void printShaderLog(GLuint shader)
	{
		//Make sure name is shader
		if (glIsShader(shader))
		{
			//Shader log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			//Allocate string
			char* infoLog = new char[maxLength];

			//Get info log
			glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
			if (infoLogLength > 0)
			{
				//Print Log
				printf("%s\n", infoLog);
			}

			//Deallocate string
			delete[] infoLog;
		}
		else
		{
			printf("Name %d is not a shader\n", shader);
		}
	}

	static GLuint Shader(GLenum shaderType, string shaderPath)
	{
		//Get vertex source
		string shaderSource;
		//open shader file
		ifstream shaderStream(shaderPath);
		//if that worked ok, load file line by line
		if (shaderStream.is_open())
		{
			string Line = "";
			while (getline(shaderStream, Line))
			{
				shaderSource += "\n" + Line;
			}
			shaderStream.close();
		}
		//convert to cstring
		char const *source = shaderSource.c_str();

		// Create and compile the vertex shader
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		//Check vertex shader for errors
		GLint vShaderCompiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile shader %d!\n", shader);
			printShaderLog(shader);
			return 0;
		}
		else
		{
			printf("shader compile success\n");
			return shader;
		}

		return 0;
	}

};