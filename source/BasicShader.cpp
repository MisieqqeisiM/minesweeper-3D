#include"BasicShader.h"
#include<iostream>
#include<GL/glew.h>
#include<glm/glm.hpp>
BasicShader::BasicShader(const char*vertexPath, const char* fragmentPath){
    loadFromFiles(vertexPath, fragmentPath);
    glBindAttribLocation(programID, 0, "in_Position");
    glBindAttribLocation(programID, 1, "in_Normal");
    glBindAttribLocation(programID, 2, "in_TextureCoords");
    link();
    projection_matrix_uniform_location = glGetUniformLocation(programID, "projection_matrix");
    view_matrix_uniform_location = glGetUniformLocation(programID, "view_matrix");
    model_matrix_uniform_location = glGetUniformLocation(programID, "model_matrix");
    light_pos_uniform_location = glGetUniformLocation(programID, "light_pos");
};
void BasicShader::loadProjectionMatrix(glm::mat4 projection_matrix){
    glUniformMatrix4fv(projection_matrix_uniform_location, 1, GL_FALSE, &projection_matrix[0][0]); 
};
void BasicShader::loadViewMatrix(glm::mat4 camera_matrix){
    glUniformMatrix4fv(view_matrix_uniform_location,1,GL_FALSE,&camera_matrix[0][0]);
};
void BasicShader::loadModelMatrix(glm::mat4 model_matrix){
    glUniformMatrix4fv(model_matrix_uniform_location, 1, GL_FALSE, &model_matrix[0][0]);
};
void BasicShader::loadLight(glm::vec3 light_pos){
    glUniform3f(light_pos_uniform_location, light_pos.x, light_pos.y, light_pos.z);
};
