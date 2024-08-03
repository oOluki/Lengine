#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_SDL


namespace Le::SDL{


class Window{

public:
Window(){}

void Init(const char* title = "Lengine", int width = 0, int height = 0,
int xpos = SDL_WINDOWPOS_CENTERED, int ypos = SDL_WINDOWPOS_CENTERED,
Uint32 flags = SDL_WINDOW_RESIZABLE){
    if(!width || !height){
        SDL_DisplayMode dm;
        SDL_GetDisplayMode(0, 0, &dm);
        if(!width) width = dm.w;
        if(!height) height = dm.h;
    }
    _window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
}

void Close(){
    SDL_DestroyWindow(_window);
}

void SetIcon(SDL_Surface* icon){
    SDL_SetWindowIcon(_window, icon);
}

void SetSize(int w, int h){
    SDL_SetWindowSize(_window, w, h);
}

void Resizeable(bool yn = true){
    SDL_SetWindowResizable(_window, yn? SDL_TRUE : SDL_FALSE);
}

void Fullscreen(Uint32 flags = SDL_WINDOW_FULLSCREEN){
    SDL_SetWindowFullscreen(_window, flags);
}

void SetBrightness(float brightness){
    SDL_SetWindowBrightness(_window, brightness);
}

void SetOpacity(float opacity){
    SDL_SetWindowOpacity(_window, opacity);
}

void GetWindowPosition(int*&& x, int*&& y){
    SDL_GetWindowPosition(_window, x, y);
}

void GetWindowSize(int*&& w, int*&& h){
    SDL_GetWindowSize(_window, w, h);
}

void MouseVisibility(bool visibility = true){
    SDL_ShowCursor(visibility);
}

void SetMousePosition(int x, int y){
    SDL_WarpMouseInWindow(_window, x, y);
}

SDL_Window* Get(){
    return _window;
}


private:
SDL_Window* _window;

};


}




#endif
