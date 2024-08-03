#pragma once
#include "../../../../dependencies/OpenGL_dependencies.hpp"

namespace Le::OpenGL{


class IndexBuffer{
private:
unsigned int IndexBufferId;
unsigned int Count;


public:

IndexBuffer(){}

template<typename T>
IndexBuffer(const T* data, unsigned int size, const size_t sizeofType = sizeof(T)) : Count(size / sizeofType){
    glGenBuffers(1, &IndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void deleteIndexBuffer(){
    glDeleteBuffers(1, &IndexBufferId);
}

void bind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
}

void unbind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline unsigned int getSize() const{return Count;}

};



}