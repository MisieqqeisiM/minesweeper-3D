#include "ShaderProgram.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<GL/glew.h>
#include<vector>

void ShaderProgram::loadFromFiles(const char*vertexPath, const char*fragmentPath){
    std::ifstream vertexInputStream(vertexPath, std::ios::in|std::ios::binary);
    if(vertexInputStream){
        std::string vertexString;
        vertexInputStream.seekg(0, std::ios::end);
        vertexString.resize(vertexInputStream.tellg());
        vertexInputStream.seekg(0, std::ios::beg);
        vertexInputStream.read(&vertexString[0], vertexString.size());
        vertexInputStream.close();

        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        const GLchar * vertexSource = (const GLchar *)vertexString.c_str();
        glShaderSource(vertexShaderID, 1, &vertexSource, 0);
        glCompileShader(vertexShaderID);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE){
            functional = false;
            GLint maxLength =0;
            glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(vertexShaderID);
            std::cout<<std::string(infoLog.data()); 
            return;
        }  
    } else{
        functional = false;
        return;
    }
    std::ifstream fragmentInputStream(fragmentPath, std::ios::in|std::ios::binary);
    if(fragmentInputStream){
        std::string fragmentString;
        fragmentInputStream.seekg(0, std::ios::end);
        fragmentString.resize(fragmentInputStream.tellg());
        fragmentInputStream.seekg(0, std::ios::beg);
        fragmentInputStream.read(&fragmentString[0], fragmentString.size());
        fragmentInputStream.close();

        fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar * fragmentSource = (const GLchar *)fragmentString.c_str();
        glShaderSource(fragmentShaderID, 1, &fragmentSource, 0);
        glCompileShader(fragmentShaderID);

        GLint isCompiled = 0;
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE){
            functional = false;
            GLint maxLength =0;
            glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShaderID, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(fragmentShaderID);
            glDeleteShader(vertexShaderID);
            std::cout<<std::string(infoLog.data()); 
            functional=false;
            return;
        }  
    } else{
        glDeleteShader(vertexShaderID);
        functional = false;
        return;
    }


    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
};

ShaderProgram::~ShaderProgram(){
    if(functional){
        glDeleteProgram(programID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }
};
void ShaderProgram::link(){
    glLinkProgram(programID);
    GLint isLinked = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE){
        GLint maxLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(programID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        std::cout<<std::string(infoLog.data());
        functional = false;
        return;
    }
    glDetachShader(programID, fragmentShaderID);
    glDetachShader(programID, vertexShaderID);
}
void ShaderProgram::use(){
    glUseProgram(programID);
};
