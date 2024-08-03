#pragma once
#include "../../../dependencies/STD_dependencies.hpp"
#include "../../../dependencies/OpenGL_dependencies.hpp"
#include "../../../dependencies/GLFW_dependencies.hpp"

#define ASSERT(x) if(!(x)) __debugbreak()

#define GL_DEBUG_CALL(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define GL_ENABLE_DEBUG() glEnable(GL_DEBUG_OUTPUT); glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); glDebugMessageCallback(debugCallback, nullptr)

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar *message, const void *userParam){
    std::cout << id << "- Lengine Got An Error Through The OpenGL Implementation [debugCallback]"
    << " Of Severity " << severity << ", With The Following Message:\n"
    "Error " << type << ", " << message << std::endl;
}

namespace Le{


void initiateGLFW(){
    if (!glfwInit()) {
        std::cerr << "glfw did not initialize correctly" << std::endl;
        exit(EXIT_FAILURE);
    }
}

GLFWwindow* createWindow(int width, int height, const char* name){
    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if (!window) {
        std::cerr << "window did not initialize properly" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    return window;
}

void destroyWindow(GLFWwindow* window){
    glfwDestroyWindow(window);
}

void setCurrentWindow(GLFWwindow* window){
    glfwMakeContextCurrent(window);
}

void initiateGLEW(){
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW did not initialize properly" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void quitGLFW(){
    glfwTerminate();
}

GLFWwindow* initiateEverything(int width, int height, const char* name){
    initiateGLFW();
    GLFWwindow* window = createWindow(width, height, name);
    setCurrentWindow(window);
    initiateGLEW();
    return window;
}

void GLClearErrors(){
    while (glGetError() != GL_NO_ERROR);    
}

bool GLLogCall(const char* function, const char* file, int line){
    bool dummyErrorCheckingVariable = false;
    while (GLenum error = glGetError())
    {
        std::cout << "Lengine Raised An OpenGL Related Error: "
        << error << "; Raised In Function " << function
        << " On File " << file << " Line" << line << std::endl;
        dummyErrorCheckingVariable = true;
    }
    return dummyErrorCheckingVariable;    
}

unsigned int createShader(unsigned int type, const char* shader){

    unsigned int shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &shader, nullptr);
    glCompileShader(shaderId);
    int compileResult;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);

    if(compileResult == GL_FALSE){
        int lenght;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &lenght);
        char* errMsg = (char*)(alloca(lenght * sizeof(char)));
        glGetShaderInfoLog(shaderId, lenght, &lenght, errMsg);
        std::cout << "Failed to compile Shader of type " << type << std::endl;
        std::cerr << errMsg << std::endl;
    }
    return shaderId;
}

unsigned int loadShader(unsigned int type, const char* path){
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

    return createShader(type, fileData.data());
}

unsigned int createProgram(){
    return glCreateProgram();
}

void addShaderToProgram(unsigned int program, unsigned int shader){
    glAttachShader(program, shader);
}

void validateProgram(unsigned int program){
    glLinkProgram(program);
    // Check the link status
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        std::cout << "Shader program failed to link" << std::endl;
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            char* log = (char*)(alloca(logLength * sizeof(char)));
            glGetProgramInfoLog(program, logLength, &logLength, log);
            std::cerr << "Linker error log:\n" << log << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    glValidateProgram(program);
}


}
