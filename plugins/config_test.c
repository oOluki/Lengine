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
#ifndef CONFIG_TEST_HEADER
#define CONFIG_TEST_HEADER


#define LE_BUILDING_DLL 1

#include "../src/environment.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct{
    Env* env;
    void* subsystem;
    int reload_count;
} PluginState;

PluginState plugin;

LE_PLUGIN_EXPORT void plugin_init(Env* env){
    printf("[INFO] Plugin: Hi From Test Plugin\n");
    printf("[INFO]- Plugin: Initiation Of Plugin Requested From %p\n", env);

    plugin.env = env;
    
    plugin.reload_count = 0;

    void* subsystem = env->channel.channel;

    int(*init_subsystem)(void) = (int(*)())env->get_symbol_from_object(subsystem, "init");

    init_subsystem();

    plugin.subsystem = subsystem;

}


LE_PLUGIN_EXPORT bool plugin_update(Plugin* self){

    printf("\nplugin owned by %p with %i reloads\n", plugin.env, plugin.reload_count);

    void(*use_subsystem)(void)  = (void(*)())plugin.env->get_symbol_from_object(plugin.subsystem, "use");

    use_subsystem();

    if(plugin.reload_count > 2){
        void(*close_subsystem)(void) = (void(*)())plugin.env->get_symbol_from_object(plugin.subsystem, "close");
        close_subsystem();
        return false;
    }

    self->state = malloc(sizeof(PluginState));

    *(PluginState*)self->state = plugin;

    plugin.env->overwrite_plugin(self, NULL);


    return true;
}

LE_PLUGIN_EXPORT void plugin_retrieve_state(void* plugin_state){
    printf("[INFO] getting state from %p to %p\n", plugin_state, &plugin);
    plugin = *(PluginState*)plugin_state;
    free(plugin_state);
    plugin.reload_count += 1;
}


#ifdef __cplusplus
}
#endif

#endif // =====================  END OF FILE CONFIG_TEST_HEADER ===========================