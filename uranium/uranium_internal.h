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

        vertex();

        vertex(vec3 position, vec3 color);

        vertex(int x, int y, int z);
    };

    struct tri {
        vertex verts[3];

        tri();

        tri(vertex v1, vertex v2, vertex v3);
    };

    #define URANIUM_VERTEX_SIZE sizeof(float) * 6

    class renderable {
    public:

        std::vector<tri> tris;

        renderable();

        void addTri(tri t);

        virtual std::vector<tri> getTransformedTris();

        virtual void* getData();
    };

    class primitive : public renderable {
    public:
        vec3 position;
        vec3 scale;
        vec3 rotation;
        vec3 color;

        primitive();

        void setPosition(vec3 pos);

        void setScale(vec3 scale);

        void setRotation(vec3 rotation);

        void* getData();

        std::vector<tri> getTransformedTris();

        glm::mat4 getTransform();
    };

    class quad : public primitive {
    public:
        quad();
    };

    class triangle : public primitive {
    public:
		triangle();
	};

    std::vector<tri> Uranium_RenderTriangles();

    std::vector<renderable*> getRenderables();

    std::vector<tri> tessellate(const std::vector<vertex>);

    void* getData(tri* t);
}