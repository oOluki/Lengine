#pragma once
#include "../Core/__init__.hpp"

#ifdef LE_INTERNAL_INITMACRO_GLFW
#include "../../../dependencies/GLFW_dependencies.hpp"

#ifdef LE_INTERNAL_INITMACRO_OPENGL
#include "../../implementations/OpenGL/__init__.hpp"
#endif

namespace Le::GLFW{

class Renderer
{
private:
GLFWwindow* Window;


public:

Renderer(){}

Renderer(GLFWwindow* window) : Window(window){
    glfwMakeContextCurrent(Window);
}

void init(GLFWwindow* window){
    Window = window;
}

void close(){
    glfwDestroyWindow(Window);
}

inline void bind() const;

inline void unbind() const;

void clear() const;

#ifdef LE_INTERNAL_INITMACRO_OPENGL

void draw(const Le::OpenGL::VertexArray va, const Le::OpenGL::IndexBuffer ib) const;

#endif

void render() const;

};


}

#endif

