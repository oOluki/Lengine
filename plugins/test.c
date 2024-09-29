#ifndef LE_TEST_PLUGIN
#define LE_TEST_PLUGIN
#include "../src/environment.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    Env* env;
    int reload_count;
} PluginState;

PluginState plugin;

void plugin_init(Env* env){
    printf("[INFO] Plugin: Hi From Test Plugin\n");
    printf("[INFO]- Plugin: Initiation Of Plugin Requested From %p\n", env);

    plugin.env = env;
    
    plugin.reload_count = 0;
}


bool plugin_update(Plugin* self){

    printf("plugin owned by %p with %i reloads\n", plugin.env, plugin.reload_count);


    if(plugin.reload_count > 3){
        return false;
    }

    self->state = malloc(sizeof(PluginState));

    *(PluginState*)self->state = plugin;

    plugin.env->overwrite_plugin(self, NULL);


    return true;
}

void plugin_retrieve_state(void* plugin_state){
    printf("[INFO] getting state from %p to %p\n", plugin_state, &plugin);
    plugin = *(PluginState*)plugin_state;
    free(plugin_state);
    plugin.reload_count += 1;
}







#endif // END OF CODE ========================================