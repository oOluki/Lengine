#pragma once
#include <iostream>

namespace Le{

#define LE_RAYCAST_SETUP()\
    float dx, dy, dr, r, x, y;\
    float cosine = std::cos(10e-3 + angle), sine = std::sin(10e-3 + angle);\
    size_t tile;\

#define LE_RAYCASTH_FUNCTION(condition)\
    if(sine > 0){\
        y = (int)(y0/ tileHeight) * tileHeight - 1;\
        dy = -tileHeight;\
    } else{\
        y = (int)(1 + y0 / tileHeight) * tileHeight;\
        dy = tileHeight;\
    }\
    r = abs((y - centery) / sine); x = centerx + r * cosine;\
    dr = - dy / sine; dx = dr * cosine;\
    for(; r < maxDistance; r += dr){\
        if(map[(int)(y / tileHeight) * sizex + (int)(x / tileWidth)] condition target){\
            tile = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);\
            break;\
        }\
        if(x < 0 or x > sizex * tileWidth or y < 0 or y > sizey * tileHeight){\
            tile = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);\
            break;\
        }\
        x += dx;\
        y += dy;\
    }

#define LE_RAYCASTV_FUNCTION(condition)\
    if(cosine > 0){\
        x = (int)(1 + x0 / tileWidth) * tileWidth;\
        dx = tileWidth;\
    } else{\
        x = (int)(x0 / tileWidth) * tileWidth - 1;\
        dx = -tileWidth;\
    }\
    r = (x - centerx) / cosine; y = centery - r * sine;\
    dr = dx / cosine; dy = - dr * sine;\
    for(; r < maxDistance; r += dr){\
        if(map[(int)(y / tileHeight) * sizex + (int)(x / tileWidth)] condition target){\
            tile = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);\
            break;\
        }\
        if(x < 0 or x > sizex * tileWidth or y < 0 or y > sizey * tileHeight){\
            tile = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);\
            break;\
        }\
        x += dx;\
        y += dy;\
    }



struct RayCastResult
{
    enum CollisionType{
        VERTICAL,
        HORIZONTAL
    };
    float r;
    size_t tile;
    CollisionType collisionType;
};



template<typename T>
RayCastResult castRayEV(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, float maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTV_FUNCTION(&);
    return {r, tile, VERTICAL};
}

template<typename T>
RayCastResult castRayEH(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, float maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTH_FUNCTION(&);
    return {r, tile, HORIZONTAL};
}

template<typename T>
RayCastResult castRayE(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, size_t maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTH_FUNCTION(&);
    float rh = r;
    size_t tileh = tile;
    LE_RAYCASTV_FUNCTION(&);
    return rh < r? {rh, tileh, HORIZONTAL} : {r, tile, VERTICAL};
}

template<typename T>
RayCastResult castRayV(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, float maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTV_FUNCTION(==);
    return {r, tile, VERTICAL};
}

template<typename T>
RayCastResult castRayH(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, float maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTH_FUNCTION(==);
    return {r, tile, HORIZONTAL};
}

template<typename T>
RayCastResult castRay(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, size_t maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTH_FUNCTION(==);
    float rh = r;
    size_t tileh = tile;
    LE_RAYCASTV_FUNCTION(==);
    return rh < r? {rh, tileh, HORIZONTAL} : {r, tile, VERTICAL};
}


template<typename T>
RayCastResult castRayNV(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, float maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTV_FUNCTION(!=);
    return {r, tile, VERTICAL};
}

template<typename T>
RayCastResult castRayNH(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, float maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTH_FUNCTION(!=);
    return {r, tile, HORIZONTAL};
}

template<typename T>
RayCastResult castRayN(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, size_t maxDistance, T target){
    LE_RAYCAST_SETUP();
    LE_RAYCASTH_FUNCTION(!=);
    float rh = r;
    size_t tileh = tile;
    LE_RAYCASTV_FUNCTION(!=);
    return rh < r? {rh, tileh, HORIZONTAL} : {r, tile, VERTICAL};
}


}




/*

template<typename T>
RayCastResult castRay(T* map, size_t sizex, size_t sizey, unsigned int tileWidth,
unsigned int tileHeight, float x0, float y0, double angle, size_t maxDistance, T target){
    RayCastResult result;
    float dx, dy, dx, dy, dr, rh, rv, x, y;
    float cosine = std::cos(10e-3 + angle), sine = std::sin(10e-3 + angle);
    size_t tileh, tilev;

    // Vertical interception
    if(cosine > 0){
        x = (int)(1 + x0 / tileWidth) * tileWidth;
        dx = tileWidth;
    } else{
        x = (int)(x0 / tileWidth) * tileWidth - 1;
        dx = -tileWidth;
    }
    rv = (xv - centerx) / cosine; y = centery - rv * sine;
    dr = dx / cosine; dy = - dr * sine;
    for(; rv < maxDistance; rv += drv){
        if(map[(int)(y / tileHeight) * sizex + (int)(x / tileWidth)] == target){
            tilev = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);
            break;
        }
        if(x < 0 or x > sizex * tileWidth or y < 0 or y > sizey * tileHeight){
            tilev = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);
            break;
        }
        x += dxv;
        y += dyv;
    }
    // Horizontal interception
    if(sine > 0){
        y = (int)(y0/ tileHeight) * tileHeight - 1;
        dy = -tileHeight;
    } else{
        y = (int)(1 + y0 / tileHeight) * tileHeight;
        dy = tileHeight;
    }
    rh = abs((yh - centery) / sine); x = centerx + rh * cosine;
    dr = - dy / sine; dx = dr * cosine;

    for(; rh < distance; rh += drh){
        if(map[(int)(y / tileHeight) * sizex + (int)(x / tileWidth)] == target){
            tileh = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);
            break;
        }
        if(x < 0 or x > sizex * tileWidth or y < 0 or y > sizey * tileHeight){
            tileh = (int)(y / tileHeight) * sizex + (int)(x / tileWidth);
            break;
        }
        x += dx;
        y += dy;
    }
    
    return rv > rh ? {rh, tileh, HORIZONTAL} : {rv, tilev, VERTICAL};
}


*/


