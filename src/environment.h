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
    void*    (*get_object_from_symbol)(void*, const char*);

    // callback method to close the dynamic object file (dlls/shared object)
    int      (*close_object_file)(void*);

    int      (*load_plugin)(Plugin*, const char*);
    void     (*unload_plugin)(Plugin*);
    void     (*overwrite_plugin)(Plugin*, const char*);
} Env;



#endif // END OF CODE ===================================================================================================================================