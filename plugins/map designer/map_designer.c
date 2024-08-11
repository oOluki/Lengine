#ifndef LE_MAPDESIGNER_C
#define LE_MAPDESIGNER_C


#include "map_editor.h"
#include "home.h"

#define MAP_CANVAS 20, 20, map_designer.dm.w - 20, map_designer.dm.h - 20

void compile(){
    const char path[] = "'"__FILE__"'";
    const char prefix_prompt[] = "gcc -fPIC -shared " "'" __FILE__ "' -o ";
    char output[(sizeof(prefix_prompt) / sizeof(char)) + (sizeof(path) / sizeof(char)) + 2];
    
    char* ofile = (char*)alloca(sizeof(path) + 2 * sizeof(char));;
    int ofile_size = (sizeof(path) + 2 * sizeof(char)) / sizeof(char);
    memcpy(ofile, path, sizeof(path) - sizeof('\0'));

    for(int i = -1 + sizeof(path) / sizeof(char); i > -1; i -= 1){
        if(path[i] == '.'){
            memcpy(ofile + i, ".so'", sizeof(".so'"));
            ofile_size = i + sizeof(".so'") / sizeof(char);
            break;
        }
    }

    memcpy(output, prefix_prompt, sizeof(prefix_prompt));
    memcpy(output + ((sizeof(prefix_prompt)) / sizeof(char)) - 1, ofile, ofile_size * sizeof(char));

    printf("[INFO] Requested Compilation:\n%s\n", output);
    if(system(output)){
        printf("[WARNING] Compilation Failed ^^^\n");
        SDL_SetWindowTitle(map_designer.window, "Le: Map Designer -compilation failed");
    } else{
        printf("[INFO] Compilation Finnished With No Errors, Warnings Could Still Have Happened\n");
        SDL_SetWindowTitle(map_designer.window, "Le: Map Designer +compilation success");
    }
}

void get_mother_dir(char* output, int mother_dir_size){
    const char* file_path = __FILE__;
    for(int i = 0; i < mother_dir_size; i+=1){
        output[i] = file_path[i];
    }
    output[mother_dir_size] = '\0';
}

void plugin_init(Env* env){
    printf("Hello From Map Designer!\n");
    printf("[INFO] Initiation Requested From %p\n", env);
    memset(&map_designer, 0, sizeof(map_designer));

    map_designer.env = env;

    EnvSubSysFlags env_init_flags;
    env_init_flags.Env_sdl_flags = SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER;
    env_init_flags.Env_img_flags = IMG_INIT_PNG;

    if(!env->init_subsystem(ENV_SDL | ENV_TTF | ENV_IMAGE, env_init_flags)) exit(EXIT_FAILURE);

    if(SDL_GetDisplayMode(0, 0, &map_designer.dm)){
        printf("[ERROR] Unable To GetDisplay Information\n");
        exit(EXIT_FAILURE);
    }
    
    map_designer.window = SDL_CreateWindow(
        "Le: Map Designer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        map_designer.dm.w, map_designer.dm.h, SDL_WINDOW_RESIZABLE
    );
    if(!map_designer.window){
        printf("[ERROR] %s\n[ERROR] Unable To Create Window\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    map_designer.renderer = SDL_CreateRenderer(map_designer.window, -1, 0);
    if(!map_designer.renderer){
        printf("[ERROR] %s\n[ERROR] Unable To Create Renderer\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_SetRenderTarget(map_designer.renderer, NULL);
    SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);
    SDL_RenderClear(map_designer.renderer);

    map_designer.tile_dim_w = 32;
    map_designer.tile_dim_h = 32;

    map_designer.characters = SDL_CreateTexture(map_designer.renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET, N_SUP_CHAR * CHAR_SIZEW, CHAR_SIZEH);
    map_designer.char_sheet = (SDL_Rect){0, 0, N_SUP_CHAR * CHAR_SIZEW, CHAR_SIZEH};

    const int mdir_s = get_mother_dir_size(__FILE__);

    char* mother_dir = (char*)alloca(mdir_s * sizeof(char));

    get_mother_dir(mother_dir, mdir_s);

    #define FONT_PATH PATH(PATH("..", ".."), PATH(PATH("assets", "fonts"), "Abel_400Regular.ttf"))

    char* font_path = (char*)alloca(mdir_s * sizeof(char) + sizeof(FONT_PATH));

    concatonate(
        font_path, mother_dir,
        FONT_PATH
    );

    #undef FONT_PATH

    map_designer.font = TTF_OpenFont(font_path, 25);
    if(!map_designer.font){
        printf("[ERROR] %s\n[ERROR] Unable To Load Font '%s'\n", TTF_GetError(), font_path);
        exit(EXIT_FAILURE);
    }

    init_chars_sheet();
    
    map_designer.draw_texture = SDL_CreateTexture(map_designer.renderer,
    SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, map_designer.dm.w, map_designer.dm.h);

    int ww, wh;
    SDL_GetWindowSize(map_designer.window, &ww, &wh);
    map_designer.wdtfx = (float)map_designer.map_canvas.w / (float)ww;
    map_designer.wdtfy = (float)map_designer.map_canvas.h / (float)wh;

    map_designer.held_tile = 1;
    map_designer.number_of_tiles = 1;
    

    map_designer.map_canvas = (SDL_Rect){
        MAP_CANVAS
    }; map_designer.zoom = 1;
    map_designer.map_sizex = 2 * map_designer.map_canvas.w / TILESIZEW;
    map_designer.map_sizey = 2 * map_designer.map_canvas.h / TILESIZEH;
    map_designer.n_map_sizex = map_designer.map_sizex;
    map_designer.n_map_sizey = map_designer.map_sizey;
    map_designer.map = (int*)malloc(map_designer.map_sizex * map_designer.map_sizey * sizeof(int));
    memset(map_designer.map, 0, map_designer.map_sizex * map_designer.map_sizey * sizeof(int));

    map_designer.sliderx_wrapper = (SDL_Rect){
        0, 0, map_designer.dm.w, map_designer.dm.h - map_designer.map_canvas.h
    };

    map_designer.slidery_wrapper = (SDL_Rect){
        0, map_designer.sliderx_wrapper.h, map_designer.dm.w - map_designer.map_canvas.w,
        map_designer.dm.h - map_designer.sliderx_wrapper.h
    };

    set_buttons();

    map_designer.flags = MDF_HOME;

    t = SDL_GetTicks64();
}

bool plugin_update(Plugin* plugin){

    {
        Uint64 _dt = SDL_GetTicks64() - t;
        if(_dt < 1000 / map_designer.dm.refresh_rate){
            SDL_Delay(1000 / map_designer.dm.refresh_rate);
            _dt = 1000 / map_designer.dm.refresh_rate;
        }
        t = SDL_GetTicks64();
        dt = _dt;
    }
    
    if(map_designer.flags & MDF_EDIT_MAP){
        update_map();
        return handle_map_events(plugin);
    } else{
        update_home();
        return handle_home_events(plugin);
    }
    return true;
}


void plugin_retrieve_state(void* plugin_state){
    printf("[INFO] Hot Reload. Retrieved State From %p To %p\n", plugin_state, &map_designer);
    map_designer = *(MapDesigner*)plugin_state;
    free(plugin_state);
    set_buttons();
    SDL_SetWindowTitle(map_designer.window, "Le: Map Designer");
    t = SDL_GetTicks64();
}

#endif // END OF FILE ============================================