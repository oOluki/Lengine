#pragma once
#include "Vertex Buffer.hpp"
#include "Vertex Buffer Layout.hpp"


namespace Le::OpenGL{


class VertexArray{
private:
unsigned int VertexArrayId;

public:
VertexArray(){}

void create(){
    glGenVertexArrays(1, &VertexArrayId);
    glBindVertexArray(VertexArrayId);
}


void deleteVertexArray(){
    glDeleteVertexArrays(1, &VertexArrayId);
}

void bind() const{
    glBindVertexArray(VertexArrayId);
}

void unbind() const{
    glBindVertexArray(0);
}

void addBuffer(const VertexBuffer buffer, const VertexBufferLayout& layout){
    buffer.bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;

    for(int i = 0; i < elements.size(); i++){
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized,
        layout.getStride(), reinterpret_cast<const void*>(offset));
        offset += element.count * VertexBufferLayoutElement::getSize(element.type);
    }

}


};




}



