#pragma once
#include "Renderer.hpp"


namespace Le::GLFW{

void Renderer::clear() const{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::bind() const{glfwMakeContextCurrent(Window);}

void Renderer::unbind() const{glfwMakeContextCurrent(NULL);}

#ifdef LE_INTERNAL_INITMACRO_OPENGL

void Renderer::draw(const Le::OpenGL::VertexArray va, const Le::OpenGL::IndexBuffer ib) const{
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getSize(), GL_UNSIGNED_INT, nullptr);
}

#endif

void Renderer::render() const{
    glfwSwapBuffers(Window);
}

}


