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

#ifndef LE_MAPDESIGNER_C
#define LE_MAPDESIGNER_C


#include "map_editor.h"
#include "home.h"

#define MAP_CANVAS 20, 20, map_designer.dm.w - 20, map_designer.dm.h - 20


void plugin_init(Env* env){
    printf("Hello From Map Designer!\n");
    printf("[INFO] Initiation Requested From %p\n", env);

    SDL_memset(&map_designer, 0, sizeof(map_designer));

    map_designer.env = env;

    void* subsystem = env->channel.channel;

    int(*init_subsystem)() = env->get_object_from_symbol(subsystem, "init");

    if(init_subsystem()){
        printf("[ERROR] Plugin: Unable To Initiate Subsystem\n");
        exit(EXIT_FAILURE);
    }

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

    printf(__FILE__ " %i : %zu\n", mdir_s, sizeof(__FILE__) /sizeof(char));

    char mother_dir[sizeof(__FILE__) / sizeof(char)] = __FILE__;
    mother_dir[mdir_s] = '\0';

    char* font_path = (char*)malloc((mdir_s + 3) * sizeof(char) + sizeof(PATH(PATH("assets", "fonts"), "Abel_400Regular.ttf")));

    concatonate(
        font_path, mother_dir,
        PATH(PATH("assets", "fonts"), "Abel_400Regular.ttf")
    );

    map_designer.font = TTF_OpenFont(font_path, 25);
    if(!map_designer.font){
        printf("[ERROR] %s\n[ERROR] Unable To Load Font '%s'\n", TTF_GetError(), font_path);
        free(font_path);
        exit(EXIT_FAILURE);
    }

    free(font_path);
    font_path = NULL;

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
    SDL_memset(map_designer.map, 0, map_designer.map_sizex * map_designer.map_sizey * sizeof(int));

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