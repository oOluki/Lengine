#pragma once
#include "Initialization macros.hpp"

#ifdef LE_INTERNAL_INITMACRO_SDL
#include "../../../dependencies/SDL_dependencies.hpp"
#endif

#ifdef LE_INTERNAL_INITMACRO_OPENGL
#include "../../../dependencies/OpenGL_dependencies.hpp"
#endif

#ifdef LE_INTERNAL_INITMACRO_GLFW
#include "../../../dependencies/GLFW_dependencies.hpp"
#endif

#include "../../../dependencies/STD_dependencies.hpp"
#include "Report.hpp"

#define LE_REPORT_IMPLEMENTATION_SUPPORT_FAILURE(identifier, msg)\
LE_REPORT_ERROR(2, "Unsupported Implementation/API \"" #identifier "\":: ", msg);

#define LE_REPORT_LIBRARY_INITIALIZATION_FAILURE(identifier, msg)\
LE_REPORT_ERROR(2, "Library \"" #identifier "\"" " Failed To Initiate, With The Following Message:\n", msg);

#define LE_REPORT_LIBRARYFLAG_INITIALIZATION_FAILURE(identifier, failedflags, msg)\
LE_REPORT_WARNING(4, "Library \"" #identifier "\""\
" Failed To Initiate The Following Flags ", std::to_string(failedflags).data(), ", With The Following Message:\n", msg);

#define LE_REPORT_LIBRARY_CLOSE_FAILURE(identifier, msg)\
LE_REPORT_ERROR(2, "Library \"" #identifier "\"" " Failed To Close, With The Following Message:\n", msg);
namespace Le{

enum LibrariesEnum{
NONE_LIB = 0,
STD_LIB = 1 << 0,
SDL_LIB = 1 << 1,
SDL_IMG_LIB = 1 << 2,
SDL_MIX_LIB = 1 << 3,
SDL_TTF_LIB = 1 << 4,
GLEW_LIB = 1 << 5,
GLFW_LIB = 1 << 6,
ALL_LIB = (1 << 7) - 1
};

}

#ifdef LE_INTERNAL_INITMACRO_SDL

#define LE_initiate_sdl(flags){\
    int status = SDL_Init(flags);\
    if(status){\
        LE_REPORT_LIBRARY_INITIALIZATION_FAILURE(SDL, SDL_GetError());\
    }\
}

#define LE_close_sdl(){\
    SDL_ClearError();\
    SDL_Quit();\
    if(SDL_GetError()){\
        LE_REPORT_LIBRARY_CLOSE_FAILURE(SDL, SDL_GetError());\
    };\
}

#define LE_initiate_IMG(flags){\
SDL_ClearError();\
   int status = IMG_Init(flags);\
    if(status == 0){\
        LE_REPORT_LIBRARY_INITIALIZATION_FAILURE(SDL_IMG, SDL_GetError());\
        return;\
    }\
    if(status != flags){\
        LE_REPORT_LIBRARYFLAG_INITIALIZATION_FAILURE(SDL_IMG, (flags & (~status)), SDL_GetError())\
    }\
}

#define LE_close_IMG(){\
    SDL_ClearError();\
    IMG_Quit();\
    if(SDL_GetError()){\
        LE_REPORT_LIBRARY_CLOSE_FAILURE(SDL_IMG, SDL_GetError());\
    };\
}
#define LE_initiate_mixer(flags){\
    SDL_ClearError();\
    int status = Mix_Init(flags);\
    if(status == 0){\
        LE_REPORT_LIBRARY_INITIALIZATION_FAILURE(SDL_MIX, SDL_GetError());\
        return;\
    }\
    if(status != flags){\
        LE_REPORT_LIBRARYFLAG_INITIALIZATION_FAILURE(SDL_MIX, (flags & (~status)), SDL_GetError());\
    }\
}

#define LE_close_mixer(){\
    SDL_ClearError();\
    Mix_Quit();\
    if(SDL_GetError()){\
        LE_REPORT_LIBRARY_CLOSE_FAILURE(SDL_MIX, SDL_GetError());\
    };\
}
#define LE_initiate_ttf(){\
    SDL_ClearError();\
    if(TTF_Init() == -1){\
        LE_REPORT_LIBRARY_INITIALIZATION_FAILURE(SDL_TTF, SDL_GetError());\
    }\
}\

#define LE_close_ttf(){\
    SDL_ClearError();\
    TTF_Quit();\
    if(SDL_GetError()){\
        LE_REPORT_LIBRARY_CLOSE_FAILURE(SDL_TTF, SDL_GetError());\
    };\
}
#endif


#ifdef LE_INTERNAL_INITMACRO_OPENGL

#define LE_initiate_glew(){\
    if(glewInit() != GLEW_OK){\
        LE_REPORT_IMPLEMENTATION_SUPPORT_FAILURE(GLEW, glewGetErrorString(glGetError()));\
    }\
}

#endif

#ifdef LE_INTERNAL_INITMACRO_GLFW

#define LE_initiate_glfw(){\
    if(glfwInit() == GLFW_FALSE){\
        const char* description = (char*)alloca(500);\
        glfwGetError(&description);\
        LE_REPORT_LIBRARY_INITIALIZATION_FAILURE(GLFW, description);\
    }\
}

#define LE_close_glfw(){\
    glfwTerminate();\
}

#endif

#if defined(LE_INTERNAL_INITMACRO_LIBINCLUSIVE) && defined(LE_INTERNAL_INITMACRO_IMPLINCLUSIVE)
#define LE_initiate_libraries(libraries, sdlFlags, sdlImgFlags, sdlMixerFlags,\
glfwHint, glfwValue){\
    if(libraries & SDL_LIB) LE_initiate_sdl(sdlFlags);\
    if(libraries & SDL_IMG_LIB) LE_initiate_img(sdlImgFlags);\
    if(libraries & SDL_MIX_LIB) LE_initiate_mixer(sdlMixerFlags);\
    if(libraries & SDL_TTF_LIB) LE_initiate_ttf();\
    if(libraries & GLEW_LIB) LE_initiate_glew();\
    if(libraries & GLFW_LIB) LE_initiate_glfw(glfwHint, glfwValue);\
}

#define LE_close_libraries(libraries){\
    if(libraries & SDL_LIB) LE_close_sdl();\
    if(libraries & SDL_IMG_LIB) LE_close_img();\
    if(libraries & SDL_MIX_LIB) LE_close_mixer();\
    if(libraries & SDL_TTF_LIB) LE_close_ttf();\
    if(libraries & GLFW_LIB) LE_close_glfw();\
}

#endif





