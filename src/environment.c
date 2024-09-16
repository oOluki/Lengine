#ifdef LE_ENVIRONMENT_C
// This file should only be included once and in Lengine.c
#error [INTERNAL ERROR] Attempt To Include 'environment.c', aka 'environment implementation', more than once. This file should only be really included once in main.c
#endif

#define LE_ENVIRONMENT_C

#define LE_EMPTY_PLUGIN (Plugin) {.environment = NULL, .handle = NULL, .init = NULL, .update = NULL, .retrieve_state = NULL, .state = NULL}

#if defined(__linux__) || defined(__APPLE__)

#include <dlfcn.h>

#define LE_UNIX_LINUX_MACOS 1

#define LE_HANDLE void*
#define LE_OPENLIB(PATH) dlopen(PATH, RTLD_NOW)
#define LE_GETSYM(HANDLE, SYM) dlsym(HANDLE, SYM)
#define LE_CLOSELIB(HANDLE) dlclose(HANDLE)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* load_object_file(const char* path){
    return LE_OPENLIB(path);
}

#elif defined(_WIN32)

#include <windows.h>

#define LE_WINDOWS 1

#define LE_HANDLE_BASE_CAP 100

#define LE_HANDLE HINSTANCE
#define LE_OPENLIB(PATH) LoadLibrary(PATH)
#define LE_GETSYM(HANDLE, SYM) GetProcAddress(HANDLE, SYM)
#define LE_CLOSELIB(HANDLE) FreeLibrary(HANDLE)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* load_object_file(const char* path){
    static LE_HANDLE* loaded_handles = NULL;
    static size_t handle_cap = 0;
    static size_t loaded_handles_count = 0;

    if(loaded_handles_count == handle_cap){
        LE_HANDLE* dummy = loaded_handles;
        loaded_handles = (LE_HANDLE*)malloc((handle_cap + LE_HANDLE_BASE_CAP) * sizeof(LE_HANDLE));
        handle_cap += LE_HANDLE_BASE_CAP;
        for(size_t i = 0; i < loaded_handles_count; i += 1){
            loaded_handles[i] = dummy[i];
        }
        free(dummy);
    }
    loaded_handles[loaded_handles_count] = LoadLibrary(path);
    return (void*)(loaded_handles + loaded_handles_count++);
}

void* get_sym(void* handle, const char* symbol_name){
    static LE_HANDLE* loaded_handles = NULL;
    static size_t loaded_handles_count = 0;

    const size_t d = (size_t)((LE_HANDLE*)handle - loaded_handles);
    if(d >= loaded_handles_count){
        fprintf(stderr, "[ERROR] Attempt To Use Unloaded or Invalid Handle '%p'\n", handle);
        exit(1);
    }
    void* const symptr = GetProcAddress(loaded_handles[d], symbol_name);
    return symptr;
}

int close_object_file(void* handle){
    static LE_HANDLE* loaded_handles = NULL;
    static size_t handle_cap = 0;
    static size_t loaded_handles_count = 0;

    const size_t d = (size_t)((LE_HANDLE*)handle - loaded_handles);
    if(d >= loaded_handles_count){
        fprintf(stderr, "[WARNING] Attempt To Free Unloaded or Invalid Handle '%p'\n", handle);
        return 1;
    }

    const int output = (int)FreeLibrary(loaded_handles[d]);

    loaded_handles_count -= 1;

    for(size_t i = d; i < loaded_handles_count; i+=1){
        loaded_handles[i] = loaded_handles[i + 1];
    }

    return output;
}

#else

#error Unsupported OS, Lengine Has No Support For Your Operating System. \
Supported OS Should Be: Windows, Linux And Mac, \
If You Got This Error In One Of This OS It Means Lengine Has Internal Problems.

#endif


#include "environment.h"


char* new_path = NULL;
Plugin* reload_request = NULL;

int load_plugin(Plugin* plugin, const char* path){

    printf("[INFO] Loading '%s' To Plugin At %p\n", path, plugin);
    LE_HANDLE handle = LE_OPENLIB(path);

    #if defined(__linux__) || defined(__APPLE__)

    if(!handle){
        printf("[ERROR] %s\n", dlerror());
        return 0;
    }

    #endif

    plugin->init = LE_GETSYM(handle, "plugin_init");
    if(!plugin->init){
        printf("[ERROR] Missing Symbol \'plugin_init\' In Plugin \'%s\'.\n"
        "All Plugins Should Define A Method \'void plugin_init(Env*)\'.\n", path);
        LE_CLOSELIB(handle);
        return 0;
    }
    plugin->update = LE_GETSYM(handle, "plugin_update");
    if(!plugin->update){
        printf("[ERROR] Missing Symbol \'plugin_update\' In Plugin \'%s\'.\n"
        "All Plugins Should Define A Method \'bool plugin_update()\'.\n", path);
        plugin->init = NULL;
        LE_CLOSELIB(handle);
        return 0;
    }
    plugin->retrieve_state = (void(*)(void*))LE_GETSYM(handle, "plugin_retrieve_state");
    if(!plugin->retrieve_state){
        printf("[WARNING] No Symbol \'plugin_retrieve_state\' In Plugin \'%s\'.\n"
        "Plugins Can't Hot Reload Withod A Method \'void plugin_retrieve_state(void*)\'.\n", path);
    }
    plugin->handle = handle;
    return 1;
}

void unload_plugin(Plugin* plugin){
    LE_CLOSELIB(plugin->handle);
    *plugin = LE_EMPTY_PLUGIN;
}

void overwrite_plugin(Plugin* plugin, const char* path){
    reload_request = plugin;

    if(path == NULL){
        if(new_path) new_path[0] = '\0';
        return;
    }

    size_t str_size = 0;
    for(; path[str_size++]; );
    new_path = (char*)malloc(str_size * sizeof(char));
    for(size_t i = 0; i < str_size; i += 1){
        new_path[i] = path[i];
    }
}

