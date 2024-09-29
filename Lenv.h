/*
MIT License

Copyright (c) 2024 oOluki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef LE_ENVIRONMENT
#define LE_ENVIRONMENT

/*
    this file contains type definitions regarding the
    core functionality of the Lengine Enviroment
*/

typedef struct Plugin{
    void* handle;
    void* environment;
    void* state;
    void* init;
    void* update;
    void  (*retrieve_state)(void*);
} Plugin;

typedef struct Channel{
    void* channel;
    unsigned int size;
    int active;
} Channel;


typedef struct{
    // shared plugin channel
    Channel  channel;

    // callback method to load dynamic object files (dlls/shared objects)
    void*    (*load_object_file)(const char*);

    // callback method to get methods/symbols from dynamic object file (dlls/shared objects)
    void*    (*get_symbol_from_object)(void*, const char*);

    // callback method to close the dynamic object file (dlls/shared object)
    int      (*close_object_file)(void*);

    int      (*load_plugin)(Plugin*, const char*);
    void     (*unload_plugin)(Plugin*);
    // overwrites the passed plugin with the object file at the passed path
    // pass NULL to the file path (const char*) argument to reload the plugin by
    // the object file at the main plugin (the first loaded plugin) path
    void     (*overwrite_plugin)(Plugin*, const char*);
} Env;


#ifndef LE_ENVIRONMENT_C // THE PLUGINS SETUP. THE LENGINE INTERNAL PROGRAM DOES NOT NEED TO INCLUDE THIS, INSTEAD, THIS SHOULD BE IN YOUR PLUGIN PROGRAM

#ifdef __linux__
    #define LE_PLUGIN_EXPORT __attribute__((visibility("default")))
    #define LE_PLUGIN_CALL
#elif defined(__APPLE__)
    #define LE_PLUGIN_EXPORT __attribute__((visibility("default")))
    #define LE_PLUGIN_CALL
#elif defined(_WIN32)
    #ifdef LE_BUILDING_DLL
        #define LE_PLUGIN_EXPORT __declspec(dllexport)
    #else
        #define LE_PLUGIN_EXPORT __declspec(dllimport)
    #endif
    #define LE_PLUGIN_CALL __cdecl
#else
    #define LE_PLUGIN_EXPORT
    #define LE_PLUGIN_CALL
#endif



#ifdef __cplusplus__
#ifndef _Bool
#define _Bool bool
#endif
extern 'C'{
#endif

/* Used to initiate the plugin once first loaded, and on all subsequential loads if hot realoading is not implemented
* see plugin_retireve_state() for hot reloading explanation.
* \param env the pointer to the environment that requested initiation
* \since 0.1.0
*/
extern LE_PLUGIN_EXPORT void LE_PLUGIN_CALL plugin_init(Env* env);


// Used for hot reloading, if a plugin doesn't want/need hot reloading simply don't define this method in it (in which case
// the main aplication will simply start the plugin using plugin_init()).
// This function (if defined by the plugin) will be called for reopening the plugin, so it can retrieve
// its previos state from before closing.
// \param plugin_state the state of the plugin before reloading (see plugin_close())
extern LE_PLUGIN_EXPORT void LE_PLUGIN_CALL plugin_retrive_state(void* plugin_state);

/*
* The method to be called every frame that the plugin is active.
* This method should also return 0 if the plugin is to be closed or 1 other wise.
* \return 0 if the plugin is to be closed or 1 otherwise.
*/
extern LE_PLUGIN_EXPORT _Bool LE_PLUGIN_CALL plugin_update(Plugin* self);


#ifdef __cplusplus__
}
#endif



#endif // END OF PLUGINS SETUP ====================================================================================



#endif // END OF CODE ===================================================================================================================================