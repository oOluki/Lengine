#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_GLFW
#include "../../../dependencies/GLFW_dependencies.hpp"

namespace Le::GLFW{

class Window{
private:
GLFWwindow* _Window;


public:

Window(GLFWwindow* window) : _Window(window){}

Window(int width, int height){
    _Window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
}

GLFWwindow* get() const {
    return _Window;
}

};




}

#endif

