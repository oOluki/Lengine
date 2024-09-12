#ifdef LE_ENVIRONMENT_C
// This file should only be included once and in Lengine.c
#error [INTERNAL ERROR] Attempt To Include 'environment.c', aka 'environment implementation', more than once. This file should only be really included once in main.c
#endif

#define LE_ENVIRONMENT_C

#if defined(__linux__) || defined(__APPLE__)

#include <dlfcn.h>

#define LE_UNIX_LINUX_MACOS 1

#define LE_HANDLE void*
#define LE_OPENLIB(PATH) dlopen(PATH, RTLD_NOW)
#define LE_GETSYM(HANDLE, SYM) dlsym(HANDLE, SYM)
#define LE_CLOSELIB(HANDLE) dlclose(HANDLE)


void* load_object_file(const char* path){
    return LE_OPENLIB(path);
}

#elif defined(_WIN32)

#include <windows.h>

#define LE_WINDOWS 1

#define LE_HANDLE HINSTANCE
#define LE_OPENLIB(PATH) LoadLibrary(PATH)
#define LE_GETSYM(HANDLE, SYM) GetProcAddress(HANDLE, SYM)
#define LE_CLOSELIB(HANDLE) FreeLibrary(HANDLE)


#else

#error Unsupported OS, Lengine Has No Support For Your Operating System. \
Supported OS Should Be: Windows, Linux And Mac, \
If You Got This Error In One Of This OS It Means Lengine Has Internal Problems.

#endif


#include "environment.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    *plugin = (Plugin){};
}

void overwrite_plugin(Plugin* plugin, const char* path){
    new_path = (char*)path;
    reload_request = plugin;
}

