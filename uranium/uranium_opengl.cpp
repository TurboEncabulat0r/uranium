#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "imgui_impl.h"
#include "uranium.h"
#include "uranium_internal.h"
#include "helpers.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

int16_t URANIUM_RENDER_WIDTH = 1280;
int16_t URANIUM_RENDER_HEIGHT = 720;

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
    GLuint ibo;

    GLuint offscreenFramebuffer;
    quad* fullscreenquad;
    texture* offscreenTexture;

    texture* defaultTexture;

    triangle* testt;

    quad* q1;
    texture* q1Texture;

    std::vector<tri> triangles = {};

    std::vector<triBatch> batches = {};

    // screen space is the normalized 0-1 value of the screen
    // world space is 10 times the screen space
    float worldToScreenSpace(float world) {
        return world / 10;
	}

    void U_INTUpdateScreenSize(int16_t width, int16_t height) {
		URANIUM_RENDER_WIDTH = width;
		URANIUM_RENDER_HEIGHT = height;
	}

    vec3 worldToScreenSpace(vec3 world) {
        return world * 0.1f;
	}
   
    void addTri(tri t) {
        triangles.push_back(t);
    }

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    texture* loadTexture(const char* path) {
        texture* t = new texture(0);
        unsigned int id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        // Set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA; // Adjust format based on number of channels
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            t->id = id;
            t->height = height;
            t->width = width;
            log("Texture created successfully");
            stbi_image_free(data);
            return t;
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
            delete t; // Clean up allocated memory before returning nullptr
            return nullptr;
        }
    }

    texture* GenerateColorTexture() {
        // generates a opengl texture that is a solid color
        unsigned int id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
        // Set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        texture* t = new texture(id);
        t->height = 100;
        t->width = 100;
        return t;
	}


    int InitializeWindow(const char* name) {

        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return -1;

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(URANIUM_RENDER_WIDTH, URANIUM_RENDER_HEIGHT, name, nullptr, nullptr);
        if (window == nullptr)
            return 1;
        

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0); // Enable vsync
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

        glGenFramebuffers(1, &offscreenFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, offscreenFramebuffer);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        offscreenTexture = new texture(0);
        glGenTextures(1, &offscreenTexture->id);
        glBindTexture(GL_TEXTURE_2D, offscreenTexture->id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, URANIUM_RENDER_WIDTH, URANIUM_RENDER_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offscreenTexture->id, 0);

        // setup vertex buffer

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(vertexarray);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);

        defaultTexture = GenerateColorTexture();

        std::string vertex = loadShader(vertexShaderSource);
        std::string fragment = loadShader(fragmentShaderSource);
        const char* vxSource = vertex.c_str();
        const char* fgSource = fragment.c_str();

        if (vxSource == nullptr || fgSource == nullptr) {
            std::cout << "shader loading failed" << std::endl;
            return 0;
        }
       
        shaderProgram = createShaderProgram(vxSource, fgSource);
        q1Texture = loadTexture("textures/test.jpg");
        q1 = new quad();
        q1->renderByDefault = false;

        q1->position = vec3(0, 0.2, 0);

        fullscreenquad = Uranium_GetFullscreenQuad(720, 980);
        fullscreenquad->renderByDefault = false;

        testt = new triangle();
        testt->color = vec3(1, 0, 0);
        testt->setPosition(vec3(0, 0, 0));
        testt->scale = (vec3(0.4f, 0.4f, 0.4f));
    }

    bool isFinished() {
		return glfwWindowShouldClose(window);
    }

    void RenderTriangles() {
        triangles.clear();
        LoggerBegin("RenderTriangles");

        std::vector<tri> tris = Uranium_RenderTriangles();
        for (int i = 0; i < tris.size(); i++) {
            addTri(tris[i]);
            //log(tris[i]);
            //log(&tris[i]);
        }
        LoggerEnd();

    }

    void imgui() {
        ImGui::Begin("Uranium OpenGL impl");
        ImGui::Text("tris: %d", triangles.size());
        ImGui::Text("vertex buffer: %d", vertexBuffer);
        ImGui::Text("vertex array: %d", vertexarray);
        ImGui::End();
    }

    void drawBatch(const triBatch& batch) {
        glBindVertexArray(vertexarray);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        // Update the vertex buffer with the vertex data for the batch
        glBufferData(GL_ARRAY_BUFFER, batch.tris.size() * sizeof(tri), batch.tris.data(), GL_DYNAMIC_DRAW);

        // Bind the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        std::vector<GLuint> indices;
        for (size_t i = 0; i < batch.tris.size() * 3; ++i) {
            indices.push_back(static_cast<GLuint>(i));
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);
        // Texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3) * 2));
        glEnableVertexAttribArray(2);

        glBindTexture(GL_TEXTURE_2D, q1Texture->id);

        // Draw elements using the index buffer
        glDrawElements(GL_TRIANGLES, batch.tris.size() * 3, GL_UNSIGNED_INT, 0);
    }

    void DrawTriangles() {
        
        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindVertexArray(vertexarray);

		// Set the attribute pointers
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);
		// Texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3) * 2));
        glEnableVertexAttribArray(2);

		// Update buffer data once (assuming all triangles have the same vertices)

        glBufferData(GL_ARRAY_BUFFER, sizeof(tri) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

        for (int i = 0; i < triangles.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, defaultTexture->id);
			glDrawArrays(GL_TRIANGLES, i * 3, 3);
		}
    }

    std::vector<triBatch> cookBatches() {
        std::vector<triBatch> batches = {};
		std::vector<tri> currentBatch = {};
        for (int i = 0; i < triangles.size(); i++) {
            if (currentBatch.size() == 400) {
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



    void U_StartFrameInternal() {
        glfwPollEvents();

        LoggerBegin("Render");

        ImGUIStartFrame();

        RenderTriangles();
        batches.clear();
        batches = cookBatches();
        imgui();
    }

    void drawTriImmediate(tri t) {
		glBindVertexArray(vertexarray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(tri), &t, GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

    void drawTriImmediate(tri t, texture* tex) {
		glBindVertexArray(vertexarray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(tri), &t, GL_STATIC_DRAW);

        // sets pointers for the vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3) * 2));
        glEnableVertexAttribArray(2);

		glBindTexture(GL_TEXTURE_2D, tex->id);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

    void DrawQuadImmedate(quad* q, texture* t) {
        if (t == nullptr) {
            log("Texture is null");
            return;
        }

        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindVertexArray(vertexarray);

        // Set the attribute pointers
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(1);
        // Texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3) * 2));
        glEnableVertexAttribArray(2);

        auto qudTris = q->getTransformedTris();
        // Update buffer data once (assuming all triangles have the same vertices)
        glBufferData(GL_ARRAY_BUFFER, qudTris.size() * sizeof(tri), qudTris.data(), GL_STATIC_DRAW);

        for (int i = 0; i < qudTris.size(); i++) {
            // Bind the texture for each triangle
            glBindTexture(GL_TEXTURE_2D, t->id);

            // Draw the triangle with offset
            glDrawArrays(GL_TRIANGLES, i * 3, 3);
        }
    }

    void DrawQuadImmedate(quad* q) {
        glUseProgram(shaderProgram);
        auto qudTris = q->getTransformedTris();
        for (int i = 0; i < qudTris.size(); i++) {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBindVertexArray(vertexarray);



            glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0);
        }
    }

    void U_EndFrameInternal() {
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        ImGUIEndFrame();


        glBindFramebuffer(GL_FRAMEBUFFER, offscreenFramebuffer);
        glViewport(0, 0, URANIUM_RENDER_WIDTH, URANIUM_RENDER_HEIGHT);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        if (U_IsWireframe())
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //DrawTriangles(); 
        LoggerBegin("DrawBatches");
        for (int i = 0; i < batches.size(); i++) {
			drawBatch(batches[i]);
		}
        LoggerEnd();

        DrawQuadImmedate(q1, q1Texture);
        drawTriImmediate(testt->getTransformedTris()[0], q1Texture);

        ImGUIDraw();

        // bind to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
        DrawQuadImmedate(fullscreenquad, offscreenTexture);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        LoggerEnd();
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