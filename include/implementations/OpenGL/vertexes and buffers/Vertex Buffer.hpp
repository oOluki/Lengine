#pragma once
#include "Vertex Buffer Layout.hpp"


namespace Le::OpenGL{

class VertexBuffer{
private:
unsigned int VertexId;


public:

void create(const void* data, unsigned int size){
    glGenBuffers(1, &VertexId);
    glBindBuffer(GL_ARRAY_BUFFER, VertexId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void deleteVertexBuffer(){
    glDeleteBuffers(1, &VertexId);
}

void bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, VertexId);
}

void unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

};

}


