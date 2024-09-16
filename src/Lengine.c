#include "environment.c"

bool compare_str(const char* str1, const char* str2){
    int i = 0;
    for(; str1[i] && str2[i]; i+=1){
        if(str1[i] != str2[i]) return false;
    }
    return (!str1[i] && !str2[i]);
}

void print_help_message(){
    printf("[INFO] Lengine Help:\n");
    printf("[HELP] Usage: ./Lengine <path_to_plugin> <optional: path_to_subsystem>\n");
    printf("[HELP] To Display This Help Message: ./Lengine or ./Lengine --help\n");
    printf("[HELP] Lengine Takes A Path To A Valid Plugin, Loads It And Runs The Plugin.\n");
    printf("[HELP] If Passed, The Object File (aka dll) At <path_to_plugin> Will Be Loaded Into The Shared Channel\n");
    printf("\n[HELP] The Plugin Must Contain The Following Methods:\n");
    printf(
        "\n[HELP] void plugin_init(Env*):\n"
        "\tUsed to initiate the plugin once first loaded, and on all subsequential loads if hot realoading is not implemented "
        "(see void plugin_retireve_state(void*)). It takes a pointer to the Lengine environment in case the plugin wants to "
        "internally manipulate it or use its channel , discretion is adviced when doing so as it can lead to unpredictable behavior.\n"
    );
    printf(
        "\n[HELP] bool plugin_update(Plugin*):\n"
        "\tThe method to be called every frame that the plugin is active, it takes a pointer to its own Plugin object (see Plugin). "
        "This method should also return 0 if the plugin is to be closed or 1 other wise. "
        "If you wish to hot reload write the plugin's state to the Plugin object state "
        "that way it will be passed to the void plugin_retrieve_state(void*) (see void plugin_retireve_state(void*)) method after the plugin gets reloaded.\n"
        ", set the Plugin object's flags (see PluginFlags) to PLUGIN_HOT_RELOAD and return 0 to signal the Lengine environment to close the current version of the plugin. "
        "If, on the other hand, you want to terminally close the plugin, set the Plugin object's flags to PLUGIN_QUIT and "
        "return 0 to signal the Lengine environment to close the plugin. "
        "Be wary that Lengine does not free any memory dynamically allocated by the plugin, not even if its on its Plugin object's state.\n"
    );
    printf("\n[HELP] The Plugin May Contain The Follwing Method (For Hot Reloading):\n");
    printf(
        "\n[HELP] void plugin_retrieve_state(void*):\n"
        "\tUsed for hot reloading, if a plugin doesn't want/need hot reloading simply don't define this method in it "
        "(in which case the Lengine aplication will simply start the plugin using plugin_init())."
        "This function (if defined by the plugin) will be called for reopening the plugin, so it can retrieve "
        "its previos state from before closing when the environment's int overwrite_plugin(Plugin*, const char*) method is called. The state that is passed to "
        "this function is the one in the Plugin object's state\n"
    );
    printf("\n[INFO] End Of Lengine Help Message\n");
}


int main(int argc, char** argv){

    if(argc < 2 || compare_str(argv[1], "--help")){
        print_help_message();
        return EXIT_SUCCESS;
    } else if(argc > 3){
        printf("[ERROR] Too Many Arguments, Expected 1 Or 2 Got %u Instead\n", argc - 1);
        printf("[HELP] To Display A Help Message Do: ./Lengine or ./Lengine --help\n");
        return EXIT_FAILURE;
    }
    
    const char* path_to_plugin =  argv[1];
    
    Env env = (Env){
        .channel = (Channel){.channel = NULL, .size = 0, .active = false},
        #if defined(LE_UNIX_LINUX_MACOS)
            .load_object_file = load_object_file,
            .get_object_from_symbol = dlsym,
            .close_object_file = dlclose,
        #elif defined(LE_WINDOWS)
            .load_object_file = load_object_file,
            .get_object_from_symbol = get_sym,
            .close_object_file = close_object_file,
        #endif
        .load_plugin = load_plugin,
        .unload_plugin = unload_plugin,
        .overwrite_plugin = overwrite_plugin  
    };

    // load subsystem object (if requested) into shared channel
    if(argc == 3){
        void* subsystem = LE_OPENLIB(argv[2]);
        if(!subsystem){
            printf(
                "[WARNING] Unable To Load Subsystem"
                #ifdef LE_UNIX_LINUX_MACOS
                ", %s\n", dlerror()
                #else
                "\n"
                #endif
            );
        } else{
            env.channel = (Channel){.channel = subsystem, .size = 1, .active = true};
        }
    }

    Plugin plugin;
    plugin.environment = &env;

    if(!load_plugin(&plugin, path_to_plugin)){
        printf("[ERROR] Unable To Load Plugin \'%s\'\n", path_to_plugin);
        printf("[HELP] To Display A Help Message Do: ./Lengine or ./Lengine --help\n");
        return EXIT_FAILURE;
    }

    printf("[INFO] Plugin At %p\n", &plugin);

    ((void(*)(Env*))plugin.init)(&env);

    while(((bool(*)(Plugin*))plugin.update)(&plugin)){
        if(reload_request){
            printf("[INFO] Overwritting Plugin At %p With '%s'\n", reload_request, new_path? new_path : path_to_plugin);

            LE_CLOSELIB(reload_request->handle);

            const char* const output_path = (new_path && new_path[0])? new_path : path_to_plugin;

            if(!load_plugin(reload_request, output_path)){
                *reload_request = LE_EMPTY_PLUGIN;
                printf("[WARNING] Unable To Overwrite Plugin With '%s'\n", output_path);
                printf("[WARNING] Plugin At %p Holds Invalid Methods\n", reload_request);
                if(reload_request = &plugin){
                    printf("[ERROR] Fatal Error Main Plugin Compromised\n");
                    return EXIT_FAILURE;
                }
                reload_request = NULL;
                if(new_path){
                    free(new_path);
                    new_path = NULL;
                }
                continue;
            }
            if(reload_request->retrieve_state){
                reload_request->retrieve_state(reload_request->state);
            } else{
                printf("[WARNING] Attempt To Reload Plugin, '%s', With No void plugin_retrieve_state(void*) Definition, "
                "Reinitialization Will Take Place Instead\n", output_path);
                ((void(*)(Env*))reload_request->init)((Env*)reload_request->environment);
            }

            printf("[INFO] Plugin Overwritten By '%s'\n", output_path);
            reload_request = NULL;
            if(new_path){
                free(new_path);
                new_path = NULL;
            }
        }
    }

    printf("[INFO] Lengine Shutdown\n"); 

    return EXIT_SUCCESS;    
}


