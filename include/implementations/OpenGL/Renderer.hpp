#pragma once
#include "vertexes and buffers/Index Buffer.hpp"
#include "vertexes and buffers/Vertex Array.hpp"
#include "vertexes and buffers/Vertex Buffer.hpp"
#include "Shaders.hpp"
#include "Program.hpp"
#include "Textures.hpp"

namespace Le::OpenGL::Renderer{

void draw(const Le::OpenGL::VertexArray va, const Le::OpenGL::IndexBuffer ib){
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getSize(), GL_UNSIGNED_INT, nullptr);
}



}

