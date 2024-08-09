#ifndef LE_MAPDESIGNER_HOME_H
#define LE_MAPDESIGNER_HOME_H
#include "map_designer.h"


enum Buttons{
ACTIVE_PATH,
MAPSIZEX,
MAPSIZEY,
TILE_WIDTH,
TILE_HEIGHT,
LAST_LABELED_BUTTON = TILE_HEIGHT,
LOAD,
SAVE,
TILE_DISPLAY,
HELD_TILE,
HELD_TILE_ID,
LAST_BUTTON
};

typedef struct{
    SDL_Rect rect;
} Button;

Button buttons[LAST_BUTTON];
SDL_Rect char_size;

#define BUTTON_H (map_designer.dm.h / (2 * (LAST_LABELED_BUTTON + 1)))
#define BUTTON_W (map_designer.dm.w / 5)
#define BUTTON_CANVAS(BUTTON) buttons[BUTTON].rect

#define SET_LABELED_BUTTON(BUTTON) buttons[BUTTON] = (Button){(SDL_Rect){BUTTON_W, (BUTTON) * (BUTTON_H), BUTTON_W, BUTTON_H}}
#define SET_BUTTON(BUTTON, ...) BUTTON_CANVAS(BUTTON) = (SDL_Rect){__VA_ARGS__}

#define BUTTON_CLICK(BUTTON, FLAG)\
    printf("[INFO] Click On " #BUTTON ": %i (%i, %i, %i, %i)\n",\
    BUTTON, EXPAND_RECT(BUTTON_CANVAS(BUTTON)));\
    map_designer.text_canvas = BUTTON_CANVAS(BUTTON);\
    map_designer.text_purpose = FLAG;\
    map_designer.flags |= MDF_TYPING


#define DRAW_BUTTON_LABEL(BUTTON) draw_text(#BUTTON, char_size, (SDL_Rect){-BUTTON_CANVAS(BUTTON).x + EXPAND_RECT(BUTTON_CANVAS(BUTTON))})
#define DRAW_LABELED_BUTTON(BUTTON, COLOR...) DRAW_BUTTON_LABEL(BUTTON); SDL_SetRenderDrawColor(map_designer.renderer, COLOR); draw_thick_rect(BUTTON_CANVAS(BUTTON), 5)

#define DRAW_BUTTON(BUTTON, COLOR...) draw_text(#BUTTON, (SDL_Rect){0, 0, 25, 25}, BUTTON_CANVAS(BUTTON)); SDL_SetRenderDrawColor(map_designer.renderer, COLOR); draw_thick_rect(BUTTON_CANVAS(BUTTON), 5)

static inline void button_click_check(){
    for(int button_id = 0; button_id < LAST_BUTTON; button_id +=1){
        if(point_rect_collision((SDL_Point){map_designer.mousex, map_designer.mousey}, buttons[button_id].rect)){
            switch (button_id)
            {
            case ACTIVE_PATH:
                BUTTON_CLICK(ACTIVE_PATH, TP_LOAD_PATH);
                if((map_designer.size_of_map_path + 1) > map_designer.text_canvas.w)
                    map_designer.text_canvas.w = char_size.w * (map_designer.size_of_map_path + 1);
                break;
            case MAPSIZEX:
                BUTTON_CLICK(MAPSIZEX, TP_MSX);
                break;
            case MAPSIZEY:
                BUTTON_CLICK(MAPSIZEY, TP_MSY);
                break;
            case TILE_WIDTH:
                BUTTON_CLICK(TILE_WIDTH, TP_TW);
                break;
            case TILE_HEIGHT:
                BUTTON_CLICK(TILE_HEIGHT, TP_TH);
                break;
            case LOAD:
                load_map(map_designer.path_to_map);
                break;
            case SAVE:
                save_map(map_designer.path_to_map);
                break;
            case TILE_DISPLAY:
                printf("[INFO] Click On " __STRING(TILE_DISPLAY) " : %i (%i, %i, %i, %i)\n",
                TILE_DISPLAY, EXPAND_RECT(BUTTON_CANVAS(TILE_DISPLAY)));
                map_designer.flags |= MDF_SETTING_TILE;
                break;
            
            default:
                break;
            }
            return;
        }
    }
}

static inline void set_buttons(){
    SET_LABELED_BUTTON(ACTIVE_PATH);
    SET_LABELED_BUTTON(MAPSIZEX);
    SET_LABELED_BUTTON(MAPSIZEY);
    SET_LABELED_BUTTON(TILE_WIDTH);
    SET_LABELED_BUTTON(TILE_HEIGHT);

    SET_BUTTON(
        TILE_DISPLAY,
        0, map_designer.dm.h / 2,
        map_designer.dm.w, map_designer.dm.h / 2
    );
    SET_BUTTON(
        LOAD,
        map_designer.dm.w - BUTTON_W, 0,
        BUTTON_W, BUTTON_H
    );
    SET_BUTTON(
        SAVE,
        EXPAND_RECT_E(BUTTON_CANVAS(LOAD), , BUTTON_CANVAS(LOAD).h+, , )
    );
    SET_BUTTON(
        HELD_TILE,
        BUTTON_CANVAS(SAVE).x, BUTTON_CANVAS(SAVE).y + BUTTON_CANVAS(SAVE).h + 5,
        BUTTON_CANVAS(SAVE).w, BUTTON_CANVAS(TILE_DISPLAY).y - BUTTON_CANVAS(SAVE).y - BUTTON_CANVAS(SAVE).h - 10
    );
    SET_BUTTON(
        HELD_TILE_ID,
        map_designer.dm.w - 3 * BUTTON_CANVAS(HELD_TILE).w / 2, BUTTON_CANVAS(HELD_TILE).y,
        BUTTON_CANVAS(HELD_TILE).w / 2, BUTTON_CANVAS(HELD_TILE).h / 2
    );
    char_size = (SDL_Rect){
        0, 0,
        BUTTON_W / 15, BUTTON_H
    };
}

static inline void draw_home(){

    if(map_designer.flags & MDF_SETTING_TILE){
        if(map_designer.tiles_sprite_sheet)
            SDL_RenderCopy(map_designer.renderer, map_designer.tiles_sprite_sheet, NULL, NULL);
        else{
            SDL_SetRenderDrawColor(map_designer.renderer, 159, 26, 147, 255);
            SDL_RenderFillRect(map_designer.renderer, &(SDL_Rect){0, 0, map_designer.dm.w, map_designer.dm.h});
        }
        
        if(map_designer.flags & MDF_CTRL){
            const float __tile_sizew = map_designer.dm.w / (map_designer.tile_sheet.w / map_designer.tile_dim_w);
            const float __tile_sizeh = map_designer.dm.h / (map_designer.tile_sheet.h / map_designer.tile_dim_h);

            SDL_SetRenderDrawBlendMode(map_designer.renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(map_designer.renderer, 75, 75, 75, 200);
            if(__tile_sizeh > 1) for(int i = 0; i < map_designer.dm.h; i += __tile_sizeh){
                for(int j = i - 1; j < i + 2; j+=1){
                    SDL_RenderDrawLine(map_designer.renderer, 0, j, map_designer.dm.w, j);
                }
            }
            if(__tile_sizew > 1) for(int i = 0; i < map_designer.dm.w; i += __tile_sizew){
                for(int j = i - 1; j < i + 2; j+=1){
                    SDL_RenderDrawLine(map_designer.renderer, j, 0, j, map_designer.dm.h);
                }
            }
            SDL_SetRenderDrawBlendMode(map_designer.renderer, SDL_BLENDMODE_NONE);
        }
        return;
    }


    draw_text(
        map_designer.path_to_map,
        char_size,
        BUTTON_CANVAS(ACTIVE_PATH)
    );

    draw_number(
        map_designer.n_map_sizex,
        char_size,
        BUTTON_CANVAS(MAPSIZEX)
    );

    draw_number(
        map_designer.n_map_sizey,
        char_size,
        BUTTON_CANVAS(MAPSIZEY)
    );

    draw_number(
        map_designer.tile_dim_w,
        char_size,
        BUTTON_CANVAS(TILE_WIDTH)
    );

    draw_number(
        map_designer.tile_dim_h,
        char_size,
        BUTTON_CANVAS(TILE_HEIGHT)
    );
    draw_number(
        (int)map_designer.held_tile,
        char_size,
        BUTTON_CANVAS(HELD_TILE_ID)
    );

    if(map_designer.flags & MDF_TYPING){
        SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(map_designer.renderer, &map_designer.text_canvas);
        draw_text(
            map_designer.text_buffer,
            char_size,
            map_designer.text_canvas
        );
    }
    

    DRAW_LABELED_BUTTON(ACTIVE_PATH, 25, 159, 25, 255);

    DRAW_LABELED_BUTTON(TILE_WIDTH, 159, 25, 50, 255);
    DRAW_LABELED_BUTTON(TILE_HEIGHT, 159, 25, 50, 255);

    DRAW_LABELED_BUTTON(MAPSIZEX, 25, 100, 147, 255);
    DRAW_LABELED_BUTTON(MAPSIZEY, 25, 100, 147, 255);

    DRAW_BUTTON(LOAD, 50, 50, 25, 255);
    DRAW_BUTTON(SAVE, 50, 50, 25, 255);

    SDL_SetRenderDrawColor(map_designer.renderer, 200, 200, 200, 255);

    draw_thick_rect(BUTTON_CANVAS(TILE_DISPLAY), 5);

    if(map_designer.tiles_sprite_sheet){
        const SDL_Rect src = (SDL_Rect){
            (int)map_designer.tile_sheet_camera.x, (int)map_designer.tile_sheet_camera.y,
            (int)map_designer.tile_sheet_camera.w, (int)map_designer.tile_sheet_camera.h
        };
        const SDL_Rect dest = (SDL_Rect){
            EXPAND_RECT_E(BUTTON_CANVAS(TILE_DISPLAY), 5+, 5+, -10+, -10+)
        };
        SDL_RenderCopy(
            map_designer.renderer, map_designer.tiles_sprite_sheet,
            NULL, &dest);
        int tile = (int)map_designer.held_tile;
        if(tile) draw_tile(tile, BUTTON_CANVAS(HELD_TILE));
    } else{
        const SDL_Rect dest = (SDL_Rect){
            BUTTON_CANVAS(TILE_DISPLAY).x + 5, BUTTON_CANVAS(TILE_DISPLAY).y + 5,
            BUTTON_CANVAS(TILE_DISPLAY).w - 10, BUTTON_CANVAS(TILE_DISPLAY).h - 10
        };
        draw_text(
            "Drag and drop a valid png\n"
            "sprite sheet",
            (SDL_Rect){0, 0, 50, 50},
            dest
        );
        SDL_SetRenderDrawColor(map_designer.renderer, 69, 25, 159, 255);
        if((int)map_designer.held_tile) SDL_RenderFillRect(map_designer.renderer, &BUTTON_CANVAS(HELD_TILE));
    }
    
}

static inline bool handle_home_events(Plugin* plugin){
    while (SDL_PollEvent(&event)){
        switch (event.type)
        {
        case SDL_QUIT:
            clean_memory();
            return false;
        case SDL_KEYDOWN:
            if(map_designer.flags & MDF_TYPING){
                if(event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER){
                    map_designer.flags &= ~MDF_TYPING;
                    switch (map_designer.text_purpose)
                    {
                    case TP_LOAD_PATH:
                        if(!map_designer.text_buffer[0]) break;
                        memcpy(map_designer.path_to_map, map_designer.text_buffer, TEXT_BUFFER_ICAP * sizeof(char));
                        map_designer.size_of_map_path = map_designer.text_position;
                        printf("[INFO] New Path : %s\n", map_designer.path_to_map);
                        map_designer.reload_map = true;
                        break;
                    case TP_MSX:{
                        SIZE_OF_STRS_E(int _size, map_designer.text_buffer);
                        int nmsx = to_int(map_designer.text_buffer, _size);
                        if(nmsx < map_designer.map_canvas.w / map_designer.tile_dim_w || nmsx > MAX_MAPSIZEX){
                            printf("[WARNING] Invalid/Unsupported Map Sizex: %i\n", nmsx);
                            break;
                        }
                        map_designer.n_map_sizex = nmsx;
                        map_designer.flags |= MDF_SETTINGS_CHANGED;
                        break;
                    }
                    case TP_MSY:{
                        SIZE_OF_STRS_E(int _size, map_designer.text_buffer);
                        int nmsy = to_int(map_designer.text_buffer, _size);
                        if(nmsy < map_designer.map_canvas.h / map_designer.tile_dim_h || nmsy > MAX_MAPSIZEY){
                            printf("[WARNING] Invalid/Unsupported Map Sizey: %i\n", nmsy);
                            break;
                        }
                        map_designer.n_map_sizey = nmsy;
                        map_designer.flags |= MDF_SETTINGS_CHANGED;
                        break;
                    }
                    case TP_TH:{
                        int tdh = to_int(map_designer.text_buffer, map_designer.text_position);
                        if(tdh < 1){
                            printf("[WARNING] Invalid Tile Heigth: %i\n", tdh);
                            break;
                        }
                        map_designer.tile_dim_h = tdh;
                        if(map_designer.tiles_sprite_sheet){
                            map_designer.number_of_tiles = (int)(map_designer.tile_sheet.w / map_designer.tile_dim_w) *
                            (int)(map_designer.tile_sheet.h / map_designer.tile_dim_h);
                        }                        
                        break;
                    }
                    case TP_TW:{
                        int tdw = to_int(map_designer.text_buffer, map_designer.text_position);
                        if(tdw < 1){
                            printf("[WARNING] Invalid Tile WIDTH %i\n", tdw);
                            break;
                        }
                        map_designer.tile_dim_w = tdw;
                        if(map_designer.tiles_sprite_sheet){
                            map_designer.number_of_tiles = (int)(map_designer.tile_sheet.w / map_designer.tile_dim_w) *
                            (int)(map_designer.tile_sheet.h / map_designer.tile_dim_h);
                        }    
                        break;
                    }
                    default:
                        break;
                    }
                    memset(map_designer.text_buffer, '\0', TEXT_BUFFER_ICAP);
                    map_designer.text_position = 0;
                    map_designer.text_purpose = TP_NONE;
                } else if(event.key.keysym.sym == SDLK_BACKSPACE && map_designer.text_position){
                    map_designer.text_position -= 1;
                    map_designer.text_buffer[map_designer.text_position] = '\0';
                }
                break;
            }
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                map_designer.flags |= MDF_DEBUG;
                break;
            case SDLK_r:{
                MapDesigner* md_state = (MapDesigner*)malloc(sizeof(MapDesigner));
                *md_state = map_designer;
                plugin->state = (void*)md_state;
                map_designer.env->overwrite_plugin(plugin, NULL);
                break;
            }
            case SDLK_c:
                if(map_designer.flags & MDF_CTRL){
                    compile();
                }
                break;
            case SDLK_KP_ENTER:
            case SDLK_ESCAPE:
            case SDLK_h:
                map_designer.flags &= ~MDF_SETTING_TILE;
                break;

            case SDLK_LCTRL:
            case SDLK_RCTRL:
                if(map_designer.flags & MDF_CTRL) break;
                map_designer.flags |= MDF_CTRL;
                break;
            
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                map_designer.flags &= ~MDF_DEBUG;
                break;
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                if(!(map_designer.flags & MDF_CTRL)) break;
                map_designer.flags &= ~MDF_CTRL;
                break;

            case SDLK_m:
                if(map_designer.flags & MDF_TYPING) break;
                if(map_designer.flags & MDF_SETTINGS_CHANGED){
                    map_designer.flags &= ~MDF_SETTINGS_CHANGED;
                    if(map_designer.n_map_sizex != map_designer.map_sizex
                    || map_designer.n_map_sizey != map_designer.map_sizey){
                        int* new_map = (int*)malloc(map_designer.n_map_sizex * map_designer.n_map_sizey * sizeof(int));
                        memset(new_map, 0, map_designer.n_map_sizex * map_designer.n_map_sizey * sizeof(int));
                        size_t __sy = map_designer.map_sizey < map_designer.n_map_sizey?
                            map_designer.map_sizey : map_designer.n_map_sizey;
                        size_t __sx = map_designer.map_sizex < map_designer.n_map_sizex?
                            map_designer.map_sizex : map_designer.n_map_sizex;

                        for(size_t i = 0; i < __sy; i+=1){
                            memcpy(
                                new_map + i * map_designer.n_map_sizex,
                                map_designer.map + i * map_designer.map_sizex,
                                __sx * sizeof(int)
                            );
                        }
                        printf("[WARNING] Map Changed From %p: (%lu, %lu) To %p: (%lu, %lu).\n",
                        map_designer.map, map_designer.map_sizex, map_designer.map_sizey,
                        new_map, map_designer.n_map_sizex, map_designer.n_map_sizey);
                        free(map_designer.map);
                        map_designer.map = new_map;
                        map_designer.map_sizex = map_designer.n_map_sizex;
                        map_designer.map_sizey = map_designer.n_map_sizey;
                    }
                }
                printf("[INFO] going to map editor\n");
                map_designer.flags &= ~MDF_HOME;
                map_designer.flags |= MDF_EDIT_MAP;
                break;
            
            default:
                break;
            }
            break;
    case SDL_TEXTINPUT:
        if(map_designer.flags & MDF_TYPING) get_text_input(event.text.text);
        break;

    case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT){
            map_designer.flags |= MDF_MOUSE_LBUTTON;
            if(map_designer.flags & MDF_SETTING_TILE){
                const float __tile_sizew = map_designer.dm.w / (map_designer.tile_sheet.w / map_designer.tile_dim_w);
                const float __tile_sizeh = map_designer.dm.h / (map_designer.tile_sheet.h / map_designer.tile_dim_h);
                int tx = map_designer.mousex / __tile_sizew;
                int ty = map_designer.mousey / __tile_sizeh;
                map_designer.held_tile = 1 + tx + ty * (int)(map_designer.tile_sheet.w / map_designer.tile_dim_w);
                if(map_designer.held_tile < 0) map_designer.held_tile = 0;
                else if(map_designer.held_tile > map_designer.number_of_tiles) map_designer.held_tile = map_designer.number_of_tiles;
            }
            else button_click_check();
        } else if(event.button.button == SDL_BUTTON_RIGHT){
            map_designer.flags |= MDF_MOUSE_RBUTTON;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if(event.button.button == SDL_BUTTON_LEFT){
            map_designer.flags &= ~MDF_MOUSE_LBUTTON;
        } else if(event.button.button == SDL_BUTTON_RIGHT){
            map_designer.flags &= ~MDF_MOUSE_RBUTTON;
        }
        break;
    case SDL_MOUSEWHEEL:
        map_designer.held_tile += event.wheel.preciseY;
        if(map_designer.held_tile > map_designer.number_of_tiles){
            map_designer.held_tile = map_designer.number_of_tiles;
        } else if(map_designer.held_tile < 0){
            map_designer.held_tile = 0;
        }
        break;
    case SDL_DROPFILE:{
        char* path_to_file = event.drop.file;
        load_tile_sheet(path_to_file, BUTTON_CANVAS(TILE_DISPLAY));
        SDL_free(path_to_file);
        break;
    }

        STD_EVENT_HANDLIND();

        default:
            break;
        }
    }
    return true;
}


static inline void update_home(){
    SDL_SetRenderTarget(map_designer.renderer, NULL);
    SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);
    SDL_RenderClear(map_designer.renderer);

    SDL_SetRenderTarget(map_designer.renderer, map_designer.draw_texture);
    SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);
    SDL_RenderClear(map_designer.renderer);

    draw_home();

    SDL_SetRenderTarget(map_designer.renderer, NULL);
    SDL_RenderCopy(map_designer.renderer, map_designer.draw_texture, NULL, NULL);
    SDL_RenderPresent(map_designer.renderer);

}




#endif // END OF FILE