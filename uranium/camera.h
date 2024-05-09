#pragma once
#include <glm/glm.hpp>

namespace uranium {

    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::mat4 view;
        glm::mat4 projection;

        inline static Camera* main;

        float fov = 90.0f;

        float _near = 0.1f;
        float _far = 1000.0f;

        Camera();

        void Update();
    };
}