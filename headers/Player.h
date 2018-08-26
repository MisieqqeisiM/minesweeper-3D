#ifndef PLAYER_H
#define PLAYER_H
#include "Camera.h"
#include "Chunk.h"
#include<glm/glm.hpp>
#include "MapController.h"
class Player{
    public:
       Player(float x, float z, float fov, float aspectRatio, float near, float far);
       void moveForward(float clock, MapController& c);
       void moveRight(float clock, MapController& c);
       void move(float clock, MapController& c, glm::vec2 movement);
       void rotateX(float rotation);
       void rotateY(float rotation);
       glm::mat4 getProjectionMatrix();
       glm::mat4 getViewMatrix();
       float getX();
       float getZ();
       glm::vec3 getCenterRay();
       glm::vec2 getCenterRayContactPointWithPlane(float y);
    private:
       Camera camera;
       float x, z, rotX, rotY;
       glm::mat4 viewMatrix;
       void calculateViewMatrix();
       bool updateViewMatrix;
       float movementSpeed = 5.0f;
};
#endif
