#include "shader.h"
#include "uranium_internal.h"


#include "helpers.h"

namespace uranium 
{



    // Compile shader function
    GLuint compileShader(GLenum shaderType, const char* shaderSource) {
        GLuint shader = glCreateShader(shaderType);
        int k = 0;
        glShaderSource(shader, 1, &shaderSource, NULL);
        std::cout << "Shader source: " << shaderSource << std::endl;
        glCompileShader(shader);

        // Check for compilation errors
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        }

        return shader;
    }

    // Create shader program function
    GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
        // Compile vertex and fragment shaders

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);


        // Link shaders into a program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Check for linking errors
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "Shader linking failed: " << infoLog << std::endl;
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        std::cout << "Shader program created successfully" << std::endl;

        return shaderProgram;
    }

    Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode = loadShader(vertexPath);
		std::string fragmentCode = loadShader(fragmentPath);

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		id = createShaderProgram(vShaderCode, fShaderCode);
	}

    void Shader::use() {
		glUseProgram(id);
	}

    void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}

    void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

    void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}

    void Shader::checkCompileErrors(unsigned int shader, std::string type) {
		GLint success;
		GLchar infoLog[1024];
        if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "Shader compilation error of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
        else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "Shader linking error of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}


	
}