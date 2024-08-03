#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_SDL

namespace Le::SDL{


class Clock{
private:
Uint64 starting_tick;
unsigned int fps;

public:
Uint64 dt;

// The clock sets its starting tick and sets the fps to the default 60 fps value
Clock(){
    starting_tick = SDL_GetTicks64();
    fps = 60;
    dt = 1000 / 60;
}

// The clock sets its starting tick and sets the fps to the given value
Clock(unsigned int _fps){
    starting_tick = SDL_GetTicks64();
    fps = (_fps < 1)? 1 : _fps;
    dt = 1000 / fps;
}

// Sets the starting tick to this moment
void StartTicking(){
    starting_tick = SDL_GetTicks64();
}

// Sets the fps
void SetFps(unsigned int _fps){
    fps = (_fps < 1) ? 1 : _fps;
}

// If put inside a running loop, these will cap the fps to the passed value.
// It will also reset the starting tick and get the time it takes to go through the loop
// considering the sleep time to set the fps, this is slightly less precise than Tick2.
void Tick(){
    dt = SDL_GetTicks64() - starting_tick;
    if(1000/fps > dt){
        SDL_Delay(1000/fps - dt);
        dt = 1000 / fps;
    }
    starting_tick = SDL_GetTicks64();
}

// If put inside a running loop, these will cap the fps to the passed value.
// It will also reset the starting tick and get the time it takes to go through the loop
// considering the sleep time to set the fps, this is more precise than Tick but slightly heavier.
void Tick2(){
    dt = SDL_GetTicks64() - starting_tick;
    if(1000/fps > dt){
        Uint64 dummy = GetTicks64();
        SDL_Delay(1000/fps - dt);
        dt += GetTicks64() - dummy;
    }
    starting_tick = SDL_GetTicks64();
}

// If put inside a running loop, these will cap the fps to the passed value.
// It will also reset the starting tick and get the time it takes to go through the loop
// without considering the sleep time to set the fps.
void CapFps(){
    dt = SDL_GetTicks64() - starting_tick;
    if(1000/fps > dt){
        SDL_Delay(1000/fps - dt);
    }
    starting_tick = SDL_GetTicks64();
}

// Sleeps for a given number of miliseconds
void Sleep(Uint32 miliseconds){
    SDL_Delay(miliseconds);
}

Uint32 GetTicks32(){
    return SDL_GetTicks();
}

Uint64 GetTicks64(){
    return SDL_GetTicks64();
}

};


}

#endif