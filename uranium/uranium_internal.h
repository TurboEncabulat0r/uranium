#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

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
        std::vector<vertex> vertacies;

        tri();

        tri(vertex v1, vertex v2, vertex v3);
    };


    class renderable {
    public:
        std::vector<tri> tris;

        renderable();

        void addTri(tri t);

        void* getData();
    };


    std::vector<tri> tessellate(const std::vector<vertex>);
}