#pragma once
#include "../../../dependencies/OpenGL_dependencies.hpp"


namespace Le::OpenGL{


class Texture{
public:

Texture(){}

Texture(unsigned int _texture) : Id(_texture){}

void create(const void* buffer, int width, int height,  GLenum type, GLint internalformat,
GLenum format, GLenum target = GL_TEXTURE_2D, GLint level = 0, GLint border = 0){
    Width = width;
    Height = height;
    Target = target;
    glGenTextures(1, &Id);
    glBindTexture(target, Id);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, buffer);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void deleteTexture(){
    glDeleteTextures(1, &Id);
}

void bind(unsigned int position) const{
    glActiveTexture(position + GL_TEXTURE0);
    glBindTexture(Target, Id);
}

void unbind() const{
    glBindTexture(Target, 0);
}

inline unsigned int id() const{return Id;}

inline int width() const{return Width;}

inline int height() const{return Height;}

private:
unsigned int Id;
int Width, Height;
GLenum Target;

};



}

