#ifndef LE_CONGIG
#define LE_CONGIG


#ifdef LE_SDL2_FOUND
#include <SDL2/SDL.h>

#ifdef LE_IMG_TTF_FOUND

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int init(){
    int err = SDL_Init(SDL_INIT_EVERYTHING);
    if(err){
        return err;
    }
    err = IMG_Init(IMG_Init(IMG_INIT_PNG));
    if(err)
        return err;

    err = TTF_Init();
    return err;
}

void close(){
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

#else

int init(){
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
}

void close(){
    SDL_Quit();
}

#endif


#endif



#endif // END OF FILE