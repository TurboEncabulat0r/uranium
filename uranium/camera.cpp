#include "camera.h"
#include "glm/matrix.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace uranium {
    // Camera* Camera::main = nullptr;

    Camera::Camera() {
        position = glm::vec3(0, 0, 0);
        rotation = glm::vec3(0, 0, 0);
        view = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, _near, _far);
        fov = 90;
    }

    void Camera::Update() {
        view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        view = glm::translate(view, -position);

        projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, _near, _far);
        if (main == nullptr)
            main = this;
    }

    
}