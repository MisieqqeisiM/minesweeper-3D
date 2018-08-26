#include"Camera.h"
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera(float fov, float aspectRatio, float near, float far){
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->near = near;
    this->far = far;
    this->perspectiveMatrix = glm::perspectiveLH(fov, aspectRatio, near, far);
};
glm::mat4 Camera::getProjectionMatrix(){
    return this->perspectiveMatrix;
};
