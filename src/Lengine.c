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
    printf("[HELP] Usage: ./Lengine <path_to_plugin>\n");
    printf("[HELP] To Display This Help Message: ./Lengine or ./Lengine --help\n");
    printf("[HELP] Lengine Takes A Path To A Valid Plugin, Loads It And Runs The Plugin.\n");
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
        "This function (if defined by the plugin) will be called for reopening the plugin, so it can retrieve"
        "its previos state from before closing when the Plugin object's flags is set to PLUGIN_HOT_RELOAD. The state that is passed to "
        "this function is the one in the Plugin object's state\n"
    );
    printf("\n[INFO] End Of Lengine Help Message\n");
}

int main(int argc, char** argv){

    if(argc < 2 || compare_str(argv[1], "--help")){
        print_help_message();
        return EXIT_SUCCESS;
    } else if(argc > 2){
        printf("[ERROR] Too Many Arguments, Expected 1 Got %u Instead\n", argc - 1);
        printf("[HELP] To Display A Help Message Do: ./Lengine or ./Lengine --help\n");
        return EXIT_FAILURE;
    }
    
    const char* path_to_plugin = argv[1];

    Channel channel = (Channel){
        .channel = NULL,
        .size = 0,
        .active = false
    };

    
    Env env = (Env){
        .channel = &channel,
        .init_subsystem = init_subsystem,
        .close_subsystem = close_subsystem,
        .get_active_subsystems = get_active_subsystems
    };

    Plugin plugin;

    if(!load_plugin(&plugin, path_to_plugin)){
        printf("[ERROR] Unable To Load Plugin \'%s\'\n", path_to_plugin);
        printf("[HELP] To Display A Help Message Do: ./Lengine or ./Lengine --help\n");
        return EXIT_FAILURE;
    }

    Plugin* const plugin_address = &plugin;

    printf("[INFO] Plugin At %p\n", plugin_address);

    ((void(*)(Env*))plugin.init)(&env);

    plugin.flags = PLUGIN_ACTIVE;

    while(plugin.flags == PLUGIN_ACTIVE){
        if(((bool(*)(Plugin*))plugin.update)(plugin_address) == false){
            switch (plugin.flags)
            {
            case PLUGIN_QUIT:
                unload_plugin(plugin_address);
                return EXIT_SUCCESS;
                break;
            case PLUGIN_HOT_RELOAD:
                printf("[INFO] Hot Reloading\n");
                void* plugin_state = plugin.state;
                unload_plugin(plugin_address);
                if(!load_plugin(plugin_address, path_to_plugin)){
                    printf("[ERROR] Unable To Load Plugin \'%s\'\n", path_to_plugin);
                    return EXIT_FAILURE;
                }
                if(plugin.retrieve_state){
                    plugin.retrieve_state(plugin_state);
                } else{
                    printf("[WARNING] Attempt To Hot Reload Plugin With No \'void retieve_state(void*)\' Definition, "
                    "Reinicialization Will Take Place Instead");
                    ((void(*)(Env*))plugin.init)(&env);
                }
                plugin.flags = PLUGIN_ACTIVE;
                break;
            
            default:
                printf("[ERROR] Invalid Plugin Flag \'%i\'\n", plugin.flags);
                return EXIT_FAILURE;
                break;
            }
        }
    }

    

    return EXIT_SUCCESS;    
}


