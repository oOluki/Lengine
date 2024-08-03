#pragma once
#include "Shaders.hpp"



namespace Le::OpenGL{


class Program{
private:
GLuint ProgramId;

public:
Program(){
}

Program(const Program& program) : ProgramId(program.id()){}

void create(Shader vertexShader, Shader fragmentShader){
    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, vertexShader.get());
    glAttachShader(ProgramId, fragmentShader.get());
    link();
    glValidateProgram(ProgramId);
}

void create(const std::initializer_list<GLuint>& shaders){
    ProgramId = glCreateProgram();

    for(const GLuint shader: shaders){
        glAttachShader(ProgramId, shader);
    }

    link();

    glValidateProgram(ProgramId);
}

void create(const std::initializer_list<Shader>& shaders){
    ProgramId = glCreateProgram();

    for(const Shader& shader: shaders){
        glAttachShader(ProgramId, shader.get());
    }
    link();

    glValidateProgram(ProgramId);
}

void start(){
    ProgramId = glCreateProgram();
}

void end() const{
    glDeleteProgram(ProgramId);
}

void addShader(Shader shader) const{
    glAttachShader(ProgramId, shader.get());
}

void removeShader(Shader shader) const{
    glDetachShader(ProgramId, shader.get());
}

void link() const{
    glLinkProgram(ProgramId);
    int linkStatus;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        std::cerr << "Shader program failed to link" << std::endl;
        // Optionally, you can retrieve and print the error log
        int logLength;
        glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            char* log = (char*)(alloca(logLength * sizeof(char)));
            glGetProgramInfoLog(ProgramId, logLength, &logLength, log);
            std::cerr << "Linker error log:\n" << log << std::endl;
        }
    }
}

void validate() const{
    glValidateProgram(ProgramId);
}

void bind() const{
    glUseProgram(ProgramId);
}

void unbind() const{
    glUseProgram(0);
}

void setUniform4f(int uniform, float v0, float v1, float v2, float v3){
    glUniform4f(uniform, v0, v1, v2, v3);
}

void setUniform3f(int uniform, float v0, float v1, float v2){
    glUniform3f(uniform, v0, v1, v2);
}

void setUniform2f(int uniform, float v0, float v1){
    glUniform2f(uniform, v0, v1);
}

void setUniform1f(int uniform, float v){
    glUniform1f(uniform, v);
}

void setUniform4i(int uniform, int v0, int v1, int v2, int v3){
    glUniform4i(uniform, v0, v1, v2, v3);
}

void setUniform3i(int uniform, int v0, int v1, int v2){
    glUniform3i(uniform, v0, v1, v2);
}

void setUniform2i(int uniform, int v0, int v1){
    glUniform2i(uniform, v0, v1);
}

void setUniform1i(int uniform, int v){
    glUniform1i(uniform, v);
}

inline GLuint id() const{return ProgramId;}

int getUniform(const char* name) const{
    return glGetUniformLocation(ProgramId, name);
}


};



}




