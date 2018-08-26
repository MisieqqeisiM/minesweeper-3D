#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include<GL/glew.h>
class ShaderProgram{
    public:
        ~ShaderProgram();
        void use();
    protected:
        void loadFromFiles(const char* vertexSource, const char * fragmentSource);
        GLuint programID, vertexShaderID, fragmentShaderID; 
        bool functional = true;
        void link();
};
#endif
