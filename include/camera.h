#ifndef CAMERA_H
#define CAMERA_H    1
#include <glm/glm.hpp>

class Camera {
    public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    Camera(glm::vec3 startPos, glm::vec3 startFront) {
        front = startFront;   //  = glm::vec3(0.0f, 0.0f, -1.0f);
        pos = startPos;       // glm::vec3(0.0f, 0.0f, 3.0f);
    }
    Camera() {
        Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    }
};

#endif