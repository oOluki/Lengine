/*
MIT License

Copyright (c) 2024 oOluki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef LE_CONGIG
#define LE_CONGIG

#define LE_BUILDING_DLL 1

#include "../src/environment.h"

#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

LE_PLUGIN_EXPORT int init(){
    int err = SDL_Init(SDL_INIT_EVERYTHING);
    if(err){
        return err;
    }
    err = IMG_Init(IMG_Init(IMG_INIT_PNG));
    if(!err)
        return 1;

    err = TTF_Init();
    return err;
}

LE_PLUGIN_EXPORT void close(){
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


#endif // END OF FILE