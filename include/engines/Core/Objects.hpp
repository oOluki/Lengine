#pragma once
#include "Report.hpp"
#include "__init__.hpp"




namespace Le{

template<typename T>
struct Tuple{
    T x, y;
};

#ifdef LE_INTERNAL_INITMACRO_SDL

typedef SDL_Color Color;

typedef SDL_Point Point;

typedef SDL_Rect Rect;

#else

struct{
int r, g, b, a;
} typedef Color;

struct{
int x, y, w, h;
} typedef Rect;

struct{
int x, y;
} typedef Point;

#endif

struct Line{
int x1, y1, x2, y2;
};

struct Triangle{
int x1, y1, x2, y2, x3, y3;
};

struct Circle{
int x, y, r;
};


}




