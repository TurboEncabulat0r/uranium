#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "uranium_internal.h"
#include "shader.h"
#include <vector>
#include "uranium_culled.h"
#include "camera.h"
#include "uranium_opengl.h"

namespace uranium {
	namespace rendering {
		Camera* c;

		std::vector<triangle> renderQueue = {};

		void NewFrame() {
			//renderQueue.clear();
		}

		void addToRenderStack(const triangle& t) {
			renderQueue.push_back(t);
		}

		void AddTriangle(const triangle& t) {
			addToRenderStack(t);
		}

		void AddQuad(const quad& q) {
			triangle t1;
			t1.verticies[0] = q.verticies[0];
			t1.verticies[1] = q.verticies[1];
			t1.verticies[2] = q.verticies[2];
			t1.modelTransform = q.modelTransform;

			triangle t2;
			t2.verticies[0] = q.verticies[0];
			t2.verticies[1] = q.verticies[2];
			t2.verticies[2] = q.verticies[3];
			t2.modelTransform = q.modelTransform;

			addToRenderStack(t1);
			addToRenderStack(t2);
		}

		void AddCube(const cube& c) {
			quad q1;
			q1.verticies[0] = { { -c.size, -c.size, -c.size }, { 1, 0, 0 }, { 0, 0 } };
			q1.verticies[1] = { { c.size, -c.size, -c.size }, { 0, 1, 0 }, { 1, 0 } };
			q1.verticies[2] = { { c.size, c.size, -c.size }, { 0, 0, 1 }, { 1, 1 } };
			q1.verticies[3] = { { -c.size, c.size, -c.size }, { 1, 1, 1 }, { 0, 1 } };
			q1.modelTransform = c.modelTransform;

			quad q2;
			q2.verticies[0] = { { -c.size, -c.size, c.size }, { 1, 0, 0 }, { 0, 0 } };
			q2.verticies[1] = { { c.size, -c.size, c.size }, { 0, 1, 0 }, { 1, 0 } };
			q2.verticies[2] = { { c.size, c.size, c.size }, { 0, 0, 1 }, { 1, 1 } };
			q2.verticies[3] = { { -c.size, c.size, c.size }, { 1, 1, 1 }, { 0, 1 } };
			q2.modelTransform = c.modelTransform;

			quad q3;
			q3.verticies[0] = { { -c.size, -c.size, -c.size }, { 1, 0, 0 }, { 0, 0 } };
			q3.verticies[1] = { { -c.size, c.size, -c.size }, { 0, 1, 0 }, { 1, 0 } };
			q3.verticies[2] = { { -c.size, c.size, c.size }, { 0, 0, 1 }, { 1, 1 } };
			q3.verticies[3] = { { -c.size, -c.size, c.size }, { 1 , 1, 1 }, { 0, 1 } };
			q3.modelTransform = c.modelTransform;

			quad q4;
			q4.verticies[0] = { { c.size, -c.size, -c.size }, { 1, 0, 0 }, { 0, 0 } };
			q4.verticies[1] = { { c.size, c.size, -c.size }, { 0, 1, 0 }, { 1, 0 } };
			q4.verticies[2] = { { c.size, c.size, c.size }, { 0, 0, 1 }, { 1, 1 } };
			q4.verticies[3] = { { c.size, -c.size, c.size }, { 1, 1, 1 }, { 0, 1 } };
			q4.modelTransform = c.modelTransform;

			quad q5;
			q5.verticies[0] = { { -c.size, -c.size, -c.size }, { 1, 0, 0 }, { 0, 0 } };
			q5.verticies[1] = { { c.size, -c.size, -c.size }, { 0, 1, 0 }, { 1, 0 } };
			q5.verticies[2] = { { c.size, -c.size, c.size }, { 0, 0, 1 }, { 1, 1 } };
			q5.verticies[3] = { { -c.size, -c.size, c.size }, { 1, 1, 1 }, { 0, 1 } };
			q5.modelTransform = c.modelTransform;

			quad q6;
			q6.verticies[0] = { { -c.size, c.size, -c.size }, { 1, 0, 0 }, { 0, 0 } };
			q6.verticies[1] = { { c.size, c.size, -c.size }, { 0, 1, 0 }, { 1, 0 } };
			q6.verticies[2] = { { c.size, c.size, c.size }, { 0, 0, 1 }, { 1, 1 } };
			q6.verticies[3] = { { -c.size, c.size, c.size }, { 1, 1, 1 }, { 0, 1 } };
			q6.modelTransform = c.modelTransform;

			AddQuad(q1);
			AddQuad(q2);
			AddQuad(q3);
			AddQuad(q4);
			AddQuad(q5);
			AddQuad(q6);
		}


		void RenderScene() {
			while (renderQueue.size() > 0) {

			}
		}



		void Initialize() {
			c = new Camera();

		}
	}
}