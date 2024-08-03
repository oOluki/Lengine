#pragma once
#include "../../../dependencies/STD_dependencies.hpp"
#include "../../../dependencies/OpenGL_dependencies.hpp"


namespace Le::OpenGL{


enum ShaderType{
    VERTEX_SHADER = GL_VERTEX_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER
};


class Shader{
private:
GLuint ShaderId;

public:

Shader(){}

Shader(unsigned int type, const char* shader){
    ShaderId = glCreateShader(type);
    glShaderSource(ShaderId, 1, &shader, nullptr);
    glCompileShader(ShaderId);
    int compileResult;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &compileResult);

    if(compileResult == GL_FALSE){
        int lenght;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &lenght);
        char* errMsg = (char*)(alloca(lenght * sizeof(char)));
        glGetShaderInfoLog(ShaderId, lenght, &lenght, errMsg);
        std::cout << "Failed to compile Shader of type " << type << std::endl;
        std::cout << shader << std::endl;
        std::cerr << errMsg << std::endl;
    }
    std::cout << shader << std::endl;
}

void load(unsigned int type, const char* path){
    std::ifstream file;

    file.open(path);
    if(file.fail()){
        std::cerr << "Error while loadin shader: Not able to open file" << std::endl;
    }
    std::string fileData, line;

    while(std::getline(file, line)){
        fileData += line;
        fileData += "\n";
    }
    const char* shader = fileData.data();
    ShaderId = glCreateShader(type);
    glShaderSource(ShaderId, 1, &shader, nullptr);
    glCompileShader(ShaderId);
    int compileResult;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &compileResult);

    if(compileResult == GL_FALSE){
        int lenght;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &lenght);
        char* errMsg = (char*)(alloca(lenght * sizeof(char)));
        glGetShaderInfoLog(ShaderId, lenght, &lenght, errMsg);
        std::cout << "Failed to compile Shader of type " << type << std::endl;
        std::cout << fileData << std::endl;
        std::cerr << errMsg << std::endl;
    }

}

void deleteShader() const{
    glDeleteShader(ShaderId);
}

GLuint get() const {
    return ShaderId;
}

};

}


