#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_SDL


namespace Le::SDL{

typedef SDL_Event Event;

typedef SDL_Texture* Texture;

typedef SDL_Surface* Surface;

typedef Mix_Chunk* Sound;

typedef Mix_Music* Music;

typedef TTF_Font* Font;

}

#endif
