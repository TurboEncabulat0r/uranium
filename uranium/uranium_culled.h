#pragma once
#include <vector>

namespace uranium {
	namespace rendering {
		struct vtex {
			glm::vec3 pos;
			glm::vec3 color;
			glm::vec2 uv;
		};

		struct tri {
			vtex verticies[3];
		};

		struct triangle {
			glm::mat4 modelTransform;
			vtex verticies[3] = {};
		};

		struct quad {
			glm::mat4 modelTransform;
			vtex verticies[4] = {};
		};

		struct cube {
			glm::mat4 modelTransform;
			float size;
		};


		void NewFrame();

		void AddTriangle(const triangle& t);

		void AddQuad(const quad& q);

		void AddCube(const cube& c);

		void Initialize();

	}
}