#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_SDL

#include "Objects.hpp"
#include "Events.hpp"
#include "Clock.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Mixer.hpp"




namespace Le::SDL{


class Engine {
private:
bool m_status;
int m_openlibs;

public:
Clock clock;
Event events;
Window window;
Renderer renderer;
Mixer mixer;

Engine(){}

void Init(int libs = Le::LibrariesEnum::ALL_LIB, Uint64 sdlFlags = SDL_INIT_EVERYTHING,
int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG, int mixFlags = MIX_INIT_MP3 | MIX_INIT_OGG){
    m_openlibs |= libs;
    if(libs & Le::LibrariesEnum::SDL_LIB){
        LE_initiate_sdl(sdlFlags);
    }
    if(libs & Le::LibrariesEnum::SDL_IMG_LIB){
        LE_initiate_IMG(imgFlags);
    }
    if(libs & Le::LibrariesEnum::SDL_MIX_LIB){
        LE_initiate_mixer(mixFlags);
    }
    if(libs & Le::LibrariesEnum::SDL_TTF_LIB){
        LE_initiate_ttf();
    }
}

void InitSdl(Uint64 flags = SDL_INIT_EVERYTHING){
    LE_initiate_sdl(flags);
    m_openlibs |= Le::LibrariesEnum::SDL_LIB;
}

void InitIMG(int flags = IMG_INIT_PNG | IMG_INIT_JPG){
    LE_initiate_IMG(flags);
    m_openlibs |= Le::LibrariesEnum::SDL_IMG_LIB;
}

void InitMIX(int flags = MIX_INIT_MP3 | MIX_INIT_OGG){
    LE_initiate_mixer(flags);
    m_openlibs |= Le::LibrariesEnum::SDL_MIX_LIB;
}

void InitTTF(){
    LE_initiate_ttf();
    m_openlibs |= Le::LibrariesEnum::SDL_TTF_LIB;
}


void Activate(){
    std::cout << "Lengine " << this << " Activated";
    if(m_openlibs & Le::LibrariesEnum::SDL_LIB) std:: cout << " : SDL";
    if(m_openlibs & Le::LibrariesEnum::SDL_IMG_LIB) std:: cout << " : SDL_IMG";
    if(m_openlibs & Le::LibrariesEnum::SDL_MIX_LIB) std:: cout << " : SDL_MIX";
    if(m_openlibs & Le::LibrariesEnum::SDL_TTF_LIB) std:: cout << " : SDL_TTF";
    std::cout << std::endl;

    m_status = true;
}

void Update()
{
renderer.Render();
renderer.Clear();
}


void GetDisplayInfo(int*&& w, int*&& h, int*&& refresh_rate = nullptr, int index = 0){
    int numOfDisplays = SDL_GetNumVideoDisplays();
    LE_DEBUG_CODE(
        if (numOfDisplays < 1)
            throw std::runtime_error("Hardware Information Error. Failed to get number of displays");
    )
    if (numOfDisplays <= index){
        index = numOfDisplays - 1;
    }
    SDL_DisplayMode dm;
    if(!SDL_GetDesktopDisplayMode(index, &dm)){
        *w = dm.w; *h = dm.h; if(refresh_rate) *refresh_rate = dm.refresh_rate;
        return;
    }
    throw std::runtime_error("Hardware Information Error. Failed to get display mode for display");
}

void GetMousePosition(int* out_x, int* out_y){
    SDL_GetMouseState(out_x, out_y);
}

void SetMousePosition(int x, int y){
    SDL_WarpMouseGlobal(x, y);
}

void Destroy(SDL_Texture* texture){
    SDL_DestroyTexture(texture);
}

void Destroy(SDL_Surface* surface){
    SDL_FreeSurface(surface);
}

void Destroy(TTF_Font* font){
    TTF_CloseFont(font);
}

void Destroy(Mix_Chunk* sound){
    Mix_FreeChunk(sound);
}

void Destroy(Mix_Music* music){
    Mix_FreeMusic(music);
}

void Shutdown(){
window.Close();
renderer.Close();
if(m_openlibs & Le::LibrariesEnum::SDL_IMG_LIB) IMG_Quit();
if(m_openlibs & Le::LibrariesEnum::SDL_MIX_LIB){
    mixer.Close();
    Mix_Quit();
}
if(m_openlibs & Le::LibrariesEnum::SDL_LIB) TTF_Quit();
if(m_openlibs & Le::LibrariesEnum::SDL_LIB) SDL_Quit();

m_openlibs = Le::LibrariesEnum::NONE_LIB;
m_status = false;
std::cout << "Lengine " << this << " Shutdown" << std::endl;
}

bool Status(){
    return m_status;
}

#ifdef LE_INTERNAL_INITMACRO_OPENGL

bool CheckOpenGLSupport(){
    SDL_Window* dummyWindow = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    640, 640, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    SDL_ClearError();
    if(dummyWindow == nullptr){
        std::cout << "[ERROR] Window Failed To Initialize:\n:: "
        << SDL_GetError() << std::endl;
        return false;
    }
    SDL_GLContext dummyContext = SDL_GL_CreateContext(dummyWindow);
    if(dummyContext == nullptr){
        std::cout << "[ERROR] Failed To Create OpenGL Context, " << "\n:: "
        << SDL_GetError() << std::endl;
        SDL_DestroyWindow(dummyWindow);
        SDL_GL_DeleteContext(dummyContext);
        return false;
    }
    SDL_GL_DeleteContext(dummyContext);
    SDL_DestroyWindow(dummyWindow);
    return true;
}

bool CheckOpenGLExtensionSupport(const char* extension){
    return SDL_GL_ExtensionSupported(extension);
}

#endif

TTF_Font* Font(const char* path, int size = 100)
{
TTF_Font* font = TTF_OpenFont(path, size); return font;
}

bool PollEvents(){
    if(events.type == SDL_QUIT){
        m_status = false;
    }
    return SDL_PollEvent(&events);
}


};

}



#endif

/*
safe zone ===========================================================================

*/



