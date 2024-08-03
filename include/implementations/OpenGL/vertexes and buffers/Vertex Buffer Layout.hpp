#pragma once
#include "../../../../dependencies/OpenGL_dependencies.hpp"


namespace Le::OpenGL{

struct VertexBufferLayoutElement
{
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    static unsigned int getSize(unsigned int type){
        switch(type){
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_BYTE:
                return 1;
            case GL_UNSIGNED_INT:
                return sizeof(unsigned int);
            case GL_INT:
                return sizeof(int);
            default:
                std::cerr << "Error: unsupported type" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
};


class VertexBufferLayout{
private:
std::vector<VertexBufferLayoutElement> Elements;
unsigned int Stride;


public:
VertexBufferLayout() : Stride(0){

}


void push(unsigned int type, unsigned int count, unsigned int normalized = GL_FALSE){
    Elements.push_back({type, count, normalized});
    Stride += count * VertexBufferLayoutElement::getSize(type);
}

inline const std::vector<VertexBufferLayoutElement>& getElements() const{return Elements;}

inline const unsigned int& getStride() const{return Stride;}


};



}


