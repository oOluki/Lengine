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

typedef enum EnvFlags{
ENV_NONE =   0,
ENV_ACTIVE = 1,
ENV_SDL =    1 << 1,
ENV_IMAGE =  1 << 2,
ENV_TTF =    1 << 3,
ENV_MIXER =  1 << 4,
ENV_OPGL =   1 << 5,
ENV_VULKAN = 1 << 6
} EnvFlags;


typedef struct EnvSubSysFlags{
unsigned int Env_sdl_flags;
int          Env_img_flags;
int          Env_mixer_flags;
}EnvSubSysFlags;

typedef struct{
    Channel  channel;
    int      (*init_subsystem)(int, EnvSubSysFlags);
    void     (*close_subsystem)(int);
    int      (*get_active_subsystems)();
    int      (*load_plugin)(Plugin*, const char*);
    void     (*unload_plugin)(Plugin*);
    void     (*overwrite_plugin)(Plugin*, const char*);
} Env;



#endif // END OF CODE ===================================================================================================================================