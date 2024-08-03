#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_SDL

#include "events numbering/KeyboardKeys.hpp"
#include "events numbering/MouseEvents.hpp"
#include "events numbering/EventTypes.hpp"


namespace Le::SDL{

typedef SDL_Event Event;


}
#endif