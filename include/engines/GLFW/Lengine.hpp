#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_GLFW

#include "Window.hpp"
#include "Renderer.cpp"


namespace Le::GLFW{


class Engine{

public:
GLFWwindow* window;
Renderer renderer;

Engine(){}

void initialize(int width = 800, int height = 600, const char* name = "Lengine",
    GLFWmonitor* monitor = nullptr, GLFWwindow* shared = nullptr){
    LE_initiate_glfw();
    window = glfwCreateWindow(width, height, name, monitor, shared);
    glfwMakeContextCurrent(window);
    LE_initiate_glew();
    renderer = window;
}

void close(){
    glfwDestroyWindow(window);
    LE_close_glfw();
}

void setSwapInterval(int interval) const{
    glfwSwapInterval(interval);
}

inline void update() const{glfwPollEvents();}

inline bool isAtive() const{return !glfwWindowShouldClose(window);}

private:

    

};


}

#endif