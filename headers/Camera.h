#ifndef CAMERA_H
#define CAMERA_H
#include<glm/glm.hpp>
class Camera{
    public:
        Camera(float fov, float aspectRatio, float near, float far);
        glm::mat4 getProjectionMatrix();
    private:
        float fov, aspectRatio, near, far;
        glm::mat4 perspectiveMatrix;
};
#endif
