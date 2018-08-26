#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H
#include "ShaderProgram.h"
#include<glm/matrix.hpp>
class BasicShader: public ShaderProgram{
    public:
        BasicShader(const char* vertexPath, const char*fragmentPath);
        void loadProjectionMatrix(glm::mat4 projection_matrix);
        void loadViewMatrix(glm::mat4 camera_matrix);
        void loadModelMatrix(glm::mat4 model_matrix);
        void loadLight(glm::vec3 light_pos);
    private:
        int projection_matrix_uniform_location, view_matrix_uniform_location, model_matrix_uniform_location, light_pos_uniform_location;
};

#endif
