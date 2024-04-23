#pragma once
#include <string>
#include <GL/glew.h>

namespace uranium {


	class Shader {
	public:
		unsigned int id;

		Shader(const char* vertexPath, const char* fragmentPath);

		void use();

		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;

	private:
		void checkCompileErrors(unsigned int shader, std::string type);
	};


	GLuint compileShader(GLenum shaderType, const char* shaderSource);
	GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);


}