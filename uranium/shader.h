#pragma once
#include <string>
#include <GL/glew.h>

namespace uranium {


	class Shader {
	public:
		unsigned int id;

		Shader();

		Shader(const char* vertexPath, const char* fragmentPath);

		void use();

		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setVec3(const std::string &name, float x, float y, float z) const;
		void setMat4(const std::string &name, const float* value) const;

	private:
		void checkCompileErrors(unsigned int shader, std::string type);
	};


	GLuint compileShader(GLenum shaderType, const char* shaderSource);
	GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);


}