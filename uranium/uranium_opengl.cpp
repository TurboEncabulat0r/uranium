#include <iostream>
#include <windows.h>
#include "imgui_impl.h"
#include "uranium.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include "helpers.h"
#include <vector>
#include <glm/vec3.hpp>
#include "uranium_internal.h"

namespace uranium {
	static GLFWwindow* window;
    // Vertex shader source code
    const char* vertexShaderSource = "shaders/vertex.glsl";
    // Fragment shader source code
    const char* fragmentShaderSource = "shaders/fragment.glsl";
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLuint shaderProgram;
    GLuint vertexBuffer;
    GLuint vertexarray;

    // screen space is the normalized 0-1 value of the screen
    // world space is 10 times the screen space
    float worldToScreenSpace(float world) {
        return world / 10;
	}

    vec3 worldToScreenSpace(vec3 world) {
        return world * 0.1f;
	}
    
    std::vector<tri> triangles = {};

    void addTri(tri t) {
        triangles.push_back(t);
    }

    class Triangle;

    void addTriangle(Triangle* t);

    

    class Triangle {
    public:
        // center of the triangle
        vec3 position;
        vec3 scale;
        vec3 color;

        Triangle() {
            position = vec3(0, 0, 0);
			scale = vec3(0.3f, 0.3f, 0.3f);
			color = vec3(1, 1, 1);
			addTriangle(this);
        }

        Triangle(vec3 position, vec3 color) {
            this->position = position;
			this->color = color;
            scale = vec3(0.3f, 0.3f, 0.3f);
            addTriangle(this);
        }

  // gets the position of a vertex in screen space
  // i signifies witch vertex to solve for
        vec3 getVertexPosition(int i) {
            switch (i) {
            case 0:
                return worldToScreenSpace(this->position) + vec3(0, scale.x, 0);
                break;
            case 1:
                return worldToScreenSpace(this->position) + vec3(scale.x, -scale.y, 0);
				break;
            case 2:
                return worldToScreenSpace(this->position) + vec3(-scale.x, -scale.y, 0);
                break;
            }
        }

        std::vector<vertex> getVertices() {

            return std::vector<vertex> {
                vertex(worldToScreenSpace(this->position) + vec3(0, scale.x, 0), this->color),
                    vertex(worldToScreenSpace(this->position) + vec3(scale.x, -scale.y, 0), this->color),
                    vertex(worldToScreenSpace(this->position) + vec3(-scale.x, -scale.y, 0), this->color) };
		}

        tri getTriangle() {
			return tri(vertex(worldToScreenSpace(this->position) + vec3(0, scale.x, 0), this->color),
                						vertex(worldToScreenSpace(this->position) + vec3(scale.x, -scale.y, 0), this->color),
                						vertex(worldToScreenSpace(this->position) + vec3(-scale.x, -scale.y, 0), this->color));
		}
    };

    std::vector<Triangle*> tris = {};

    void addTriangle(Triangle* t) {
        tris.push_back(t);
	}

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

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    GLuint offscreenFramebuffer;
    GLuint offscreenTexture;
    int InitializeWindow(const char* name) {

        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return -1;

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(1280, 720, name, nullptr, nullptr);
        if (window == nullptr)
            return 1;
        

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync
        InitializeImGUI(window, glsl_version);
        GLenum err = glewInit();
        if (!err) {}
        else
        {
            if (GLEW_VERSION_3_3) {
				std::cout << "OpenGL 3.3 is supported" << std::endl;
			}
            else {
				std::cerr << "OpenGL 3.3 is not supported" << std::endl;
				return 0;
			}

            std::cout << glewGetErrorString(err) << std::endl;

            std::cerr << "Failed to initialize GLEW" << std::endl;
            return 0;
        }

        // setup vertex array
        glGenBuffers(1, &vertexBuffer);
        glGenVertexArrays(1, &vertexarray);

        glBindVertexArray(vertexarray);

        // setup vertex buffer
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);

        std::string vertex = loadShader(vertexShaderSource);
        std::string fragment = loadShader(fragmentShaderSource);
        const char* vxSource = vertex.c_str();
        const char* fgSource = fragment.c_str();

        if (vxSource == nullptr || fgSource == nullptr) {
            std::cout << "shader loading failed" << std::endl;
            return 0;
        }
        
        shaderProgram = createShaderProgram(vxSource, fgSource);

        Triangle* t1 = new Triangle(vec3(0, 0, 0), vec3(1, 0, 0));
        Triangle* t2 = new Triangle(vec3(3, 0, 0), vec3(1, 1, 0));
        Triangle* t3 = new Triangle(vec3(6, 0, 0), vec3(1, 1, 1));
        Triangle* t4 = new Triangle(vec3(-3, 0, 0), vec3(1, 0, 1));
        Triangle* t5 = new Triangle(vec3(-6, 0, 0), vec3(0, 1, 1));

        quad* q1 = new quad();
        triangle* t6 = new triangle();
        
    }

    bool isFinished() {
		return glfwWindowShouldClose(window);
    }

    void RenderTriangles() {
        triangles.clear();
        for (int i = 0; i < tris.size(); i++) {
            addTri(tris[i]->getTriangle());
		}

        std::vector<tri> tris = Uranium_RenderTriangles();
        for (int i = 0; i < tris.size(); i++) {
            addTri(tris[i]);
        }

    }

    void imgui() {
        ImGui::Begin("Uranium OpenGL impl");
        ImGui::Text("tris: %d", triangles.size());
        ImGui::Text("Triangles: %d", tris.size());
        ImGui::Text("vertex buffer: %d", vertexBuffer);
        ImGui::Text("vertex array: %d", vertexarray);
        ImGui::End();
    }

    void drawTriangle(int i) {
        glBindVertexArray(vertexarray);
        glUseProgram(shaderProgram);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);
        std::cout << "Drawing triangle " << i << std::endl;

	}

    struct triBatch {
        std::vector<tri> tris;
        triBatch(std::vector<tri> tris) : tris(tris) {}
    };

    void drawBatch(const triBatch& batch) {
        // Bind the vertex array object
        glBindVertexArray(vertexarray);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        // Update the vertex buffer with the vertex data for the batch
        glBufferData(GL_ARRAY_BUFFER, batch.tris.size() * (sizeof(tri)), batch.tris.data(), GL_DYNAMIC_DRAW);

        // Set the vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);

        // Draw the batch
        glDrawArrays(GL_TRIANGLES, 0, batch.tris.size() * 3);
    }

    void DrawTriangles() {
        
        glUseProgram(shaderProgram);

        for (int i = 0; i < tris.size(); i++) {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBindVertexArray(vertexarray);

			glBufferData(GL_ARRAY_BUFFER, sizeof(tri), getData(&triangles[i]), GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0);
		
		}
    }

    std::vector<triBatch> cookBatches() {
        std::vector<triBatch> batches = {};
		std::vector<tri> currentBatch = {};
        for (int i = 0; i < triangles.size(); i++) {
            if (currentBatch.size() == 1000) {
				batches.push_back(triBatch(currentBatch));
				currentBatch.clear();
			}
			currentBatch.push_back(triangles[i]);
		}
        if (currentBatch.size() > 0) {
			batches.push_back(triBatch(currentBatch));
		}
		return batches;
    }

    std::vector<triBatch> batches = {};

    void U_StartFrameInternal() {
        glfwPollEvents();
        ImGUIStartFrame();
        RenderTriangles();
        //batches.clear();
        //batches = cookBatches();
        imgui();
    }


    void U_EndFrameInternal() {
        ImGUIEndFrame();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);


        DrawTriangles(); 

        ImGUIDraw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    void U_ShutdownInternal() {
        ShutdownImGUI();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    /*
    uranium -

    turbo, this is a note to you in the future.
    your sitting here, at your computer, 11:05pm on a shitty ass friday
     - woke up at 5pm
     - missed out on doing shit with your girlfriend who really needed that
     - disipointed your parents

    things are bad right now, and this project is keeping me sane for the most part
    i hope that you see this in the future, and leave this somewhere in the src for uranium
    this project has helped a lot in the past few days
    and i hope that you are in the future, working a job you want, and achieving the thigns we wnated
    i hope to see that this project has come to a interesting conclusion
    and i hope that it is finally a project that can reach what we dreamed of in it
    
    anyways shit sucks right now, but programming is fun and i love cpp and the pain it brings
    your truly talented, even if you dont see it.
    and right now i really think i messed up in a lot of ways, and idk i hope to see that those problems have resolved
    i have so many doubts about the future and everything, and im just not sure. but i really hope this message gives
    you some courage in the future to push on.

    */
}