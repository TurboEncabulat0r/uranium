#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/vec2.hpp"

namespace uranium {


    struct vec3 {
        float x, y, z;

        vec3(float x, float y, float z);

        vec3();
    };

    struct vec2 {
		float x, y;

		vec2(float x, float y);

		vec2();


	};

    vec2 operator+(const vec2& a, const vec2& b);

    vec2 operator-(const vec2& a, const vec2& b);

    vec2 operator*(const vec2& a, const vec2& b);

    vec2 operator*(const vec2& a, const float& b);



    vec3 operator+(const vec3& a, const vec3& b);

    vec3 operator-(const vec3& a, const vec3& b);

    vec3 operator*(const vec3& a, const vec3& b);

    vec3 operator*(const vec3& a, const float& b);


    struct vertex {
        vec3 position;
        vec3 color;
        vec2 uv;

        vertex();

        vertex(vec3 position, vec3 color);

        vertex(int x, int y, int z);
    };

    struct tri {
        vertex verts[3];

        tri();

        tri(vertex v1, vertex v2, vertex v3);
    };

    struct triBatch {
        std::vector<tri> tris;
        triBatch(std::vector<tri> tris) : tris(tris) {}
    };


    #define URANIUM_VERTEX_SIZE sizeof(float) * 6

    class renderable {

    public:
        vec3 position;
        vec3 scale;
        vec3 rotation;
        glm::mat4 translation = glm::mat4(1.0f);

        vec3 color;

        std::vector<tri> tris;
        bool renderByDefault = true;

        renderable();

        void addTri(tri t);

        virtual std::vector<tri> getTransformedTris();

        void setPosition(vec3 pos);

        void setScale(vec3 scale);

        void setRotation(vec3 rotation);

        void setColor(vec3 color);

        void computeTranslation();

        virtual void* getData();
        glm::mat4 getTranslation();
    };

    struct texture {
		unsigned int id;
		int width, height;

        texture(unsigned int id, int width, int height);

        texture(unsigned int id);
	};

    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::mat4 view;
        glm::mat4 projection;

        float fov = 90.0f;

        float _near = 0.1f;
        float _far = 1000.0f;

        Camera();

        void Update();
    };

    class primitive : public renderable {
    public:


        bool getAbsolute = false;


        primitive();


        void* getData();

        std::vector<tri> getTransformedTris();


    };

    class quad : public primitive {
    public:
        quad();
    };

    class cube : public primitive {
    public:
        quad* faces[6];
        cube();

    };

    class triangle : public primitive {
    public:
		triangle();
        triangle(vec3 pos);
        triangle(vec3 pos, vec3 color);

	};
    void addRenderable(renderable*);

    void removeRenderable(renderable*);

    void U_INTUpdateScreenSize(int16_t, int16_t);

    quad* Uranium_GetFullscreenQuad(int, int);

    std::vector<tri> Uranium_RenderTriangles();

    std::vector<renderable*> getRenderables();

    std::vector<tri> tessellate(const std::vector<vertex>);

    void* getData(tri* t);

    void U_ToggleWireframe();

    bool U_IsWireframe();

}