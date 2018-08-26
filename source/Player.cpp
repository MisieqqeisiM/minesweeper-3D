#include "Player.h"
#include <cmath>
#include<iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Chunk.h"

Player::Player(float x, float z, float fov, float aspectRatio, float near, float far):camera(fov,aspectRatio, near, far){
    this->x = x;
    this->z = z;
    updateViewMatrix = true;
}
void Player::move(float clock, MapController& c, glm::vec2 movement){
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f),rotY, glm::vec3(0.0f,1.0f,0.0f));
    glm::vec4 requestedMovement = clock*movementSpeed*rotationMatrix*glm::vec4(movement.x, 0.0f, movement.y, 1.0f);
    glm::vec2 allowedMovement = c.allowedMovement(glm::vec2(x,z), glm::vec2(requestedMovement.x, requestedMovement.z));
    this->x += allowedMovement.x;
    this->z += allowedMovement.y;
    if(this->x>MapController::CHUNK_SIZE){
        this->x -= MapController::CHUNK_SIZE;
        c.shiftLeft();
    }
    if(this->x<0){
        this->x += MapController::CHUNK_SIZE;
        c.shiftRight();
    }
    if(this->z>MapController::CHUNK_SIZE){
        this->z -= MapController::CHUNK_SIZE;
        c.shiftDown();
    }
    if(this->z<0){
        this->z += MapController::CHUNK_SIZE;
        c.shiftUp();
    }

    updateViewMatrix = true;

}

void Player::moveForward(float clock, MapController& c){
    move(clock, c, glm::vec2(0.0f,1.0f));
}
void Player::moveRight(float clock, MapController& c){
    move(clock, c, glm::vec2(1.0f,0.0f));
}
void Player::rotateX(float rotation){
    this->rotX += rotation;
    if(rotX>3.141593/2)
        rotX = 3.141593/2;
    if(rotX<-3.141593/2)
        rotX = -3.141593/2;
    updateViewMatrix = true;
}
void Player::rotateY(float rotation){
    this->rotY += rotation;
    updateViewMatrix = true;
}
glm::mat4 Player::getProjectionMatrix(){
    return camera.getProjectionMatrix();
}
glm::mat4 Player::getViewMatrix(){
    if(updateViewMatrix){
        calculateViewMatrix();
    }
    return this->viewMatrix;
}
void Player::calculateViewMatrix(){
    glm::mat4 newMatrix(1.0f);
    newMatrix = glm::rotate(newMatrix, -rotX, glm::vec3(1.0f,0.0f,0.0f));
    newMatrix = glm::rotate(newMatrix, -rotY, glm::vec3(0.0f,1.0f,0.0f));
    newMatrix = glm::translate(newMatrix, glm::vec3(-x,-2.0f,-z));
    updateViewMatrix = false;
    this->viewMatrix = newMatrix;
}
glm::vec3 Player::getCenterRay(){
    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, rotY, glm::vec3(0.0f,1.0f,0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotX, glm::vec3(1.0f,0.0f,0.0f));
    return glm::vec3(rotationMatrix * glm::vec4(0.0f,0.0f,1.0f,0.0f));
};
glm::vec2 Player::getCenterRayContactPointWithPlane(float y){
    glm::vec3 centerRay = getCenterRay();
    float yDifference = (y-2.0f)/centerRay.y;
    if(yDifference>0)
        return glm::vec2(this->x+centerRay.x*yDifference, this->z+centerRay.z*yDifference);
    else
        return glm::vec2(0.0f,0.0f);
};

float Player::getX(){
    return x;
};
float Player::getZ(){
    return z;
};
