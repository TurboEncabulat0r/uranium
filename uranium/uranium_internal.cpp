#include "uranium_internal.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace uranium {

    vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3::vec3() {
        x = 0;
        y = 0;
        z = 0;
    }

    vec2::vec2(float x, float y) : x(x), y(y) {}

    vec2::vec2() {
		x = 0;
		y = 0;
	}
    

    vec2 operator+(const vec2& a, const vec2& b) {
		vec2 result;
		result.x = a.x + b.x;
		result.y = a.y + b.y;
		return result;
	}

    vec2 operator-(const vec2& a, const vec2& b) {
        vec2 result;
        result.x = a.x - b.x;
        result.y = a.y - b.y;
        return result;

    }

    vec2 operator*(const vec2& a, const vec2& b) {
		vec2 result;
		result.x = a.x * b.x;
		result.y = a.y * b.y;
		return result;
	}

    vec2 operator*(const vec2& a, const float& b) {
        vec2 result;
        result.x = a.x * b;
        result.y = a.y * b;
        return result;


    }


    
   
    vertex::vertex() {
        position = vec3(0, 0, 0);
        color = vec3(0, 0, 0);
    }

    vertex::vertex(vec3 p, vec3 c) {
        position = p;
        color = c;
    }

    vertex::vertex(int x, int y, int z) {
        position = vec3(x, y, z);
        color = vec3(0, 0, 0);
    }

    vec3 operator+(const vec3& a, const vec3& b) {
        vec3 result;
        result.x = a.x + b.x;
        result.y = a.y + b.y;
        result.z = a.z + b.z;
        return result;
    }

    vec3 operator-(const vec3& a, const vec3& b) {
        vec3 result;
        result.x = a.x - b.x;
        result.y = a.y - b.y;
        result.z = a.z - b.z;
        return result;
    }

    vec3 operator*(const vec3& a, const vec3& b) {
        vec3 result;
        result.x = a.x * b.x;
        result.y = a.y * b.y;
        result.z = a.z * b.z;
        return result;
    }

    vec3 operator*(const vec3& a, const float& b) {
        vec3 result;
        result.x = a.x * b;
        result.y = a.y * b;
        result.z = a.z * b;
        return result;
    }

    tri::tri() {
        vertacies.push_back(vertex(0, 1, 0));
        vertacies.push_back(vertex(1, -1, 0));
        vertacies.push_back(vertex(-1, -1, 0));
    }

    tri::tri(vertex v1, vertex v2, vertex v3) {
        vertacies.push_back(v1);
        vertacies.push_back(v2);
        vertacies.push_back(v3);
    }




    renderable::renderable() {
        tris = {};
	}

    void renderable::addTri(tri t) {
        tris.push_back(t);
    }

    void* renderable::getData() {
        return tris.data();
    }

    //bool isConvex(const vertex& prev, const vertex& current, const vertex& next) {
    //    glm::vec2 edge1 = current.position - prev.position;
    //    glm::vec2 edge2 = next.position - current.position;
    //    float crossProduct = edge1.x * edge2.y - edge1.y * edge2.x;
    //    return crossProduct >= 0; // Convex if cross product is non-negative
    //}



    std::vector<tri> tessellate(const std::vector<vertex> vertacies) {
		std::vector<tri> tris = {};
        for (int i = 0; i < vertacies.size(); i += 3) {
			tris.push_back(tri(vertacies[i], vertacies[i + 1], vertacies[i + 2]));
		}
		return tris;
	}
    
    

    

}