#ifdef LE_ENVIRONMENT_C
// This file should only be included once and in main.c
#error [INTERNAL ERROR] Attempt To Include 'environment.c', aka 'environment implementation', more than once. This file should only be really included once in main.c
#endif

#define LE_ENVIRONMENT_C

#include "environment.h"

#include <stdbool.h>
#include "stdio.h"
#include "stdlib.h"
#include "dlfcn.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

int active_subsystems = ENV_NONE;

int init_subsystem(int subsystems, EnvSubSysFlags flags){
    subsystems = (subsystems & (~active_subsystems));
    int out = 1;
    if(subsystems & ENV_SDL){
        if(SDL_Init(flags.Env_sdl_flags)){
            printf("[ERROR] Could Not Initiate SDL Subsytem");
            out = 0;
        } else active_subsystems |= ENV_SDL;
    }
    if(subsystems & ENV_IMAGE){
        if(!IMG_Init(flags.Env_img_flags)){
            printf("[ERROR] Could Not Initiate SDL_image Subsystem");
            out = 0;
        } else active_subsystems |= ENV_IMAGE;
    }
    if(subsystems & ENV_TTF){
        if(TTF_Init()){
            printf("[ERROR] Could Not Initiate SDL_ttf Subsytem");
            out = 0;
        } else active_subsystems |= ENV_TTF;
    }
    if(subsystems & ENV_MIXER){
        if(!Mix_Init(flags.Env_mixer_flags)){
            printf("[ERROR] Could Not Initiate SDL_mixer Subsystem");
            out = 0;
        } else active_subsystems |= ENV_MIXER;
    }
    return out;
}

void close_subsystem(int subsystems){
    subsystems = subsystems & active_subsystems;
    active_subsystems = active_subsystems & (~subsystems);
    if(subsystems & ENV_SDL){
        SDL_Quit();
    }
    if(subsystems & ENV_IMAGE){
        IMG_Quit();
    }
    if(subsystems & ENV_TTF){
        TTF_Quit();
    }
    if(subsystems & ENV_MIXER){
        Mix_CloseAudio();
        Mix_Quit();
    }

}

int get_active_subsystems(){
    return active_subsystems;
}

int load_plugin(Plugin* plugin, const char* path_to_plugin){

    void* handle = dlopen(path_to_plugin, RTLD_NOW);
    if(!handle){
        printf("[ERROR] %s\n", dlerror());
        return 0;
    }
    plugin->init = dlsym(handle, "plugin_init");
    if(!plugin->init){
        printf("[ERROR] Missing Symbol \'plugin_init\' In Plugin \'%s\'.\n"
        "All Plugins Should Define A Method \'void plugin_init(Env*)\'.\n", path_to_plugin);
        dlclose(handle);
        return 0;
    }
    plugin->update = dlsym(handle, "plugin_update");
    if(!plugin->update){
        printf("[ERROR] Missing Symbol \'plugin_update\' In Plugin \'%s\'.\n"
        "All Plugins Should Define A Method \'bool plugin_update()\'.\n", path_to_plugin);
        plugin->init = NULL;
        dlclose(handle);
        return 0;
    }
    plugin->retrieve_state = (void(*)(void*))dlsym(handle, "plugin_retrieve_state");
    if(!plugin->retrieve_state){
        printf("[WARNING] No Symbol \'plugin_retrieve_state\' In Plugin \'%s\'.\n"
        "Plugins Can't Hot Reload Withod A Method \'void plugin_retrieve_state(void*)\'.\n", path_to_plugin);
    }
    plugin->handle = handle;
    return 1;
}

void unload_plugin(Plugin* plugin){
    dlclose(plugin->handle);
    *plugin = (Plugin){};
}

void quit_plugin(Plugin* plugin){
    unload_plugin(plugin);
    plugin->flags = PLUGIN_NONE;
}

