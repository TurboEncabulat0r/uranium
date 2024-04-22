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
        verts[0] = vertex(0, 1, 0);
        verts[1] = vertex(1, -1, 0);
        verts[2] = vertex(-1, -1, 0);
    }

    tri::tri(vertex v1, vertex v2, vertex v3) {
        verts[0] = v1;
		verts[1] = v2;
		verts[2] = v3;
    }

    void* getData(tri* t) {
        return t->verts;
    }

    std::vector<renderable*> renderables = {};


    renderable::renderable() {
        tris = {};
        renderables.push_back(this);
	}

    void renderable::addTri(tri t) {
		tris.push_back(t);
	}

    void* renderable::getData() {
        return tris.data();
    }

    std::vector<tri> renderable::getTransformedTris() {
		return tris;
	}

    primitive::primitive() {
        position = vec3(0, 0, 0);
        rotation = vec3(0, 0, 0);
        scale = vec3(1, 1, 1);
	}

    void primitive::setPosition(vec3 pos) {
		position = pos;
    }

    void primitive::setRotation(vec3 rot) {
        rotation = rot;
    }

    void primitive::setScale(vec3 s) {
		scale = s;
	}

    glm::mat4 primitive::getTransform() {
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(position.x, position.y, position.z));
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(scale.x, scale.y, scale.z));
		return transform;
	}


    void* primitive::getData() {
        return getTransformedTris().data();
	}

    std::vector<tri> primitive::getTransformedTris() {
        std::vector<tri> transformedTris = {};
        for (tri t : tris) {
            vertex v1 = t.verts[0];
            vertex v2 = t.verts[1];
            vertex v3 = t.verts[2];
            v1.position = (v1.position * scale) + position;
            v2.position = (v2.position * scale) + position;
            v3.position = (v3.position * scale) + position;
            transformedTris.push_back(tri(v1, v2, v3));
        }
        return transformedTris;
    }

    quad::quad() {
        tri t1 = tri(vertex(-1, 1, 0), vertex(1, 1, 0), vertex(-1, -1, 0));
        tri t2 = tri(vertex(1, 1, 0), vertex(1, -1, 0), vertex(-1, -1, 0));
        addTri(t1);
        addTri(t2);
	}

    triangle::triangle() {
		tri t = tri(vertex(0, 1, 0), vertex(1, -1, 0), vertex(-1, -1, 0));
		addTri(t);
    }

    bool isConvex(const vertex& prev, const vertex& current, const vertex& next) {
		glm::vec2 edge1 = glm::vec2(current.position.x - prev.position.x, current.position.y - prev.position.y);
		glm::vec2 edge2 = glm::vec2(next.position.x - current.position.x, next.position.y - current.position.y);
		float crossProduct = edge1.x * edge2.y - edge1.y * edge2.x;
		return crossProduct >= 0; // Convex if cross product is non-negative
	}
    
    std::vector<renderable*> getRenderables() {
		return renderables;
	}

    std::vector<tri> Uranium_RenderTriangles() {
        std::vector<tri> tris = {};
        for (renderable* r : renderables) {
            auto t = r->getTransformedTris();
            tris.insert(tris.end(), t.begin(), t.end());
		}
        return tris;
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