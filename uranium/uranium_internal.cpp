#include "uranium.h"
#include "uranium_internal.h"

#include "helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace uranium {

    // global variables
    extern bool isWireframe = false;
    extern bool isFullscreen = false;
    extern bool isVsync = false;

    static glm::mat4 modelTransform = glm::mat4(1.0f);
    static glm::mat4 viewTransform = glm::mat4(1.0f);
    static glm::mat4 globalProjection = glm::mat4(1.0f);

    static Camera* mainCamera;

#pragma region uranium_math

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

#pragma endregion


#pragma region uranium_rendering
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

    void addRenderable(renderable* r) {
		renderables.push_back(r);
	}

    void removeRenderable(renderable* r) {
        for (int i = 0; i < renderables.size(); i++) {
            if (renderables[i] == r) {
				renderables.erase(renderables.begin() + i);
				return;
			}
		}

    }


    texture::texture(unsigned int id) {
        this->id = id;
	}

    texture::texture(unsigned int id, int w, int h) {
		this->id = id;
		this->width = w;
		this->height = h;
    }

    renderable::renderable() {
        tris = {};
        this->computeTranslation();
        //renderables.push_back(this);
	}

    void setTriColor(tri& t, vec3 color) {
		t.verts[0].color = color;
		t.verts[1].color = color;
		t.verts[2].color = color;
	}

    void setTriUV(tri& t, vec2 uv1, vec2 uv2, vec2 uv3) {
        t.verts[0].uv = uv1;
        t.verts[1].uv = uv2;
        t.verts[2].uv = uv3;
    }

    Camera::Camera() {
        position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
        view = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, _near, _far);
		fov = 90;
        mainCamera = this;
    }

    void setGlobalProjection(glm::mat4 projection) {
		globalProjection = projection;
	}

    void Camera::Update() {
        view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        view = glm::translate(view, -position);

        projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, _near, _far);
        setGlobalProjection(projection);
	}

    void renderable::addTri(tri t) {
        setTriColor(t, color);

		tris.push_back(t);
	}

    void* renderable::getData() {
        return tris.data();
    }

    std::vector<tri> renderable::getTransformedTris() {
		return tris;
	}

    primitive::primitive() {
        color = vec3(1, 1, 1);
	}

    void mat4SetPosition(glm::mat4& t, vec3 pos) {
		t = glm::translate(t, glm::vec3(pos.x, pos.y, pos.z));
	}

    void mat4SetScale(glm::mat4& t, vec3 scale) {
        t = glm::scale(t, glm::vec3(scale.x, scale.y, scale.z));
    }

    void mat4SetRotation(glm::mat4& t, vec3 rotation) {
		t = glm::rotate(t, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		t = glm::rotate(t, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		t = glm::rotate(t, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	}

    void renderable::setPosition(vec3 pos) {
        position = pos;
        computeTranslation();
    }

    void renderable::setRotation(vec3 rot) {
        rotation = rot;
        computeTranslation();
    }

    void renderable::setScale(vec3 s) {
        scale = s;
        computeTranslation();
	}

    glm::mat4 renderable::getTranslation() {
        //computeTranslation();
		return this->translation;
	}

    void renderable::computeTranslation() {
        glm::mat4* t;
        t = &this->translation;
        *t = glm::mat4(1.0f);
        // apply scale
        *t = glm::scale(*t, glm::vec3(scale.x, scale.y, scale.z));
        // apply rotation
        *t = glm::rotate(*t, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        *t = glm::rotate(*t, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        *t = glm::rotate(*t, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        // apply position
        *t = glm::translate(*t, glm::vec3(position.x, position.y, position.z));
	}

    void ApplyTranslation(const glm::mat4& t, tri& tri) {
        // Apply the translation matrix to each vertex of the triangle

        for (int i = 0; i < 3; ++i) {
            glm::vec4 pos(tri.verts[i].position.x, tri.verts[i].position.y, tri.verts[i].position.z, 1);
            pos = t * pos;
            tri.verts[i].position = vec3(pos.x, pos.y, pos.z);
        }
    }


    void applyTransform(glm::mat4 t, tri& tri) {
        for (int i = 0; i < 3; i++) {
			glm::vec4 pos = glm::vec4(tri.verts[i].position.x, tri.verts[i].position.y, tri.verts[i].position.z, 1);
			pos = t * pos;
			tri.verts[i].position = vec3(pos.x, pos.y, pos.z);
		}
	}


    void* primitive::getData() {
        return getTransformedTris().data();
	}

    std::vector<tri> primitive::getTransformedTris() {

        std::vector<tri> transformedTris = {};

        // applies translation to each triangle
        for (tri t : tris) {
			tri tCopy = t;
			ApplyTranslation(this->getTranslation(), tCopy);
			transformedTris.push_back(tCopy);
		}
        return transformedTris;
    }

    quad::quad() {
        tri t1 = tri(vertex(-1, 1, 0), vertex(1, 1, 0), vertex(-1, -1, 0));
        tri t2 = tri(vertex(1, 1, 0), vertex(1, -1, 0), vertex(-1, -1, 0));

        // sets uv coordinates
        t1.verts[0].uv = vec2(0, 0);
        t1.verts[1].uv = vec2(1, 0);
        t1.verts[2].uv = vec2(0, 1);

        t2.verts[0].uv = vec2(1, 0);
        t2.verts[1].uv = vec2(1, 1);
        t2.verts[2].uv = vec2(0, 1);

        addTri(t1);
        addTri(t2);
	}

    vec2 triUvs[3] = {vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1)};
    vec2* getTriangleUVs() {

		return triUvs;
	}

    triangle::triangle() {
		tri t = tri(vertex(0, 1, 0), vertex(1, -1, 0), vertex(-1, -1, 0));
        vec2* uvs = getTriangleUVs();
        setTriUV(t, uvs[0], uvs[1], uvs[2]);
		addTri(t);
    }

    triangle::triangle(vec3 pos) {
        pos = pos;
        tri t = tri(vertex(0, 1, 0), vertex(1, -1, 0), vertex(-1, -1, 0));
        addTri(t);
    }

    triangle::triangle(vec3 pos, vec3 color) {
        this->setPosition(pos);
        this->color = color;
        tri t = tri(vertex(0, 1, 0), vertex(1, -1, 0), vertex(-1, -1, 0));
        addTri(t);
    }

    bool isConvex(const vertex& prev, const vertex& current, const vertex& next) {
		glm::vec2 edge1 = glm::vec2(current.position.x - prev.position.x, current.position.y - prev.position.y);
		glm::vec2 edge2 = glm::vec2(next.position.x - current.position.x, next.position.y - current.position.y);
		float crossProduct = edge1.x * edge2.y - edge1.y * edge2.x;
		return crossProduct >= 0; // Convex if cross product is non-negative
	}


    
    bool shouldBeCulled(const tri& t, Camera* c) {
        // checks if the triangle is in the view frustum
        for (int i = 0; i < 3; i++) {

		}
        return true;
	}
    
    std::vector<renderable*> getRenderables() {
		return renderables;
	}

    std::vector<tri> Uranium_RenderTriangles() {
        std::vector<tri> tris;

        for (renderable* r : renderables) {
            if (!r->renderByDefault)
                continue;

            //const glm::mat4& translationMatrix = r->getTranslation(); // Cache translation

            for (tri& t : r->tris) {
                
                tris.push_back(t);
                //ApplyTranslation(translationMatrix, tris.back());
            }
        }

        //for (int i = 0; i < tris.size(); i++) {
            //ApplyTranslation(globalProjection, tris[i]);
		//}


        return tris;
    }

    quad* Uranium_GetFullscreenQuad(int w, int h) {
		quad* q = new quad();
		q->getAbsolute = true;
        q->setScale(vec3(1, 1, 1));
		return q;
	} 

    cube::cube(){
        // creates the quads for the cube
        quad* front = new quad();
        front->setPosition(vec3(0, 0, 1));
        front->setRotation(vec3(0, 0, 0));
        front->setScale(vec3(1, 1, 1));

        this->faces[0] = front;

        quad* back = new quad();
        back->setPosition(vec3(0, 0, -1));
        back->setRotation(vec3(0, 180, 0));
        back->setScale(vec3(1, 1, 1));

        this->faces[1] = back;

        quad* left = new quad();
        left->setPosition(vec3(-1, 0, 0));
        left->setRotation(vec3(0, 90, 0));
        left->setScale(vec3(1, 1, 1));

        this->faces[2] = left;

        quad* right = new quad();
        right->setPosition(vec3(1, 0, 0));
        right->setRotation(vec3(0, -90, 0));
        right->setScale(vec3(1, 1, 1));

        this->faces[3] = right;

        quad* top = new quad();
        top->setPosition(vec3(0, 1, 0));
        top->setRotation(vec3(90, 0, 0));
        top->setScale(vec3(1, 1, 1));

        this->faces[4] = top;

        quad* bottom = new quad();
        bottom->setPosition(vec3(0, -1, 0));
        bottom->setRotation(vec3(-90, 0, 0));
        bottom->setScale(vec3(1, 1, 1));

        this->faces[5] = bottom;

        for (int i = 0; i < 6; i++) {
            addRenderable(faces[i]);
            addTri(faces[i]->tris[0]);
        }
    }


    void U_ToggleWireframe() {
        isWireframe = !isWireframe;
    }

    bool U_IsWireframe() {
		return isWireframe;
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

#pragma endregion
    
    

    

}