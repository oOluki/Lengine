#ifndef LE_MAPDESIGNER_MAPEDITOR_H
#define LE_MAPDESIGNER_MAPEDITOR_H

#include "map_designer.h"


#define RECT_POINT_UL(RECT) (SDL_Point){RECT.x, RECT.y}
#define RECT_POINT_UR(RECT) (SDL_Point){RECT.x + RECT.w, RECT.y}
#define RECT_POINT_BL(RECT) (SDL_Point){RECT.x, RECT.y + RECT.h}
#define RECT_POINT_BR(RECT) (SDL_Point){RECT.x + RECT.w, RECT.y + RECT.h}


static inline void draw_map(){
    x_offset = map_designer.u_scrollx % TILESIZEW;
    y_offset = map_designer.u_scrolly % TILESIZEH;

    SDL_SetRenderDrawColor(map_designer.renderer, 159, 69, 24, 255);

    for(int j = map_designer.map_canvas.x;
    j < map_designer.map_canvas.x + map_designer.map_canvas.w + TILESIZEW;
    j += TILESIZEW){
        for(int x = j - 1; x < j + 3; x += 1){
            SDL_RenderDrawLine(map_designer.renderer, x - x_offset, YOFFSET,
            x - x_offset, map_designer.dm.h);
        }
    }
    for(int i = map_designer.map_canvas.y;
    i < map_designer.map_canvas.y + map_designer.map_canvas.h + TILESIZEH;
    i += TILESIZEH){
        for(int y = i - 1; y < i + 3; y += 1){
            SDL_RenderDrawLine(map_designer.renderer, XOFFSET, y - y_offset,
            map_designer.dm.w, y - y_offset);
        }
    }

    SDL_SetRenderDrawColor(map_designer.renderer, 69, 25, 159, 255);

    for(size_t i = map_designer.scrolly / TILESIZEH;
    i < (map_designer.scrolly + map_designer.map_canvas.h) / (TILESIZEH) && i < map_designer.map_sizey;
    i+=1){
        for(size_t j = map_designer.scrollx / TILESIZEW;
        j < (map_designer.scrollx + map_designer.map_canvas.w) / (TILESIZEW) && j < map_designer.map_sizex;
        j+=1){
            int tile = map_designer.map[j + i * map_designer.map_sizex];
            if(tile){
                const SDL_Rect dest = {
                    XOFFSET + (j - (int)(map_designer.scrollx / TILESIZEW)) * TILESIZEW - x_offset + 3,
                    YOFFSET + (i - (int)(map_designer.scrolly / TILESIZEH)) * TILESIZEH - y_offset + 3,
                    TILESIZEW - 3, TILESIZEH - 3
                };
                if((tile < map_designer.number_of_tiles) && map_designer.tiles_sprite_sheet)
                    draw_tile(tile, dest);
                else
                    SDL_RenderFillRect(map_designer.renderer, &dest);
            }
        }
    }
    SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);

    SDL_RenderFillRect(map_designer.renderer, &(SDL_Rect){0, 0, XOFFSET, map_designer.dm.h});
    SDL_RenderFillRect(map_designer.renderer, &(SDL_Rect){0, 0, map_designer.dm.w, YOFFSET});

    SDL_SetRenderDrawBlendMode(map_designer.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(map_designer.renderer, 100, 100, 100, 100);

    SDL_RenderFillRect(
        map_designer.renderer, &map_designer.sliderx_wrapper
    );
    SDL_RenderFillRect(
        map_designer.renderer, &map_designer.slidery_wrapper
    );
    SDL_SetRenderDrawColor(map_designer.renderer, 200, 150, 100, 200);
    const float syp = map_designer.scrolly / (map_designer.map_sizey * TILESIZEH - map_designer.dm.h);
    const float sys = (float)map_designer.dm.h / (float)(map_designer.map_sizey * TILESIZEH);
    const SDL_Rect slidery = {
        0, (int)(map_designer.slidery_wrapper.y + syp * (map_designer.dm.h - sys * map_designer.dm.h - map_designer.slidery_wrapper.y)),
        map_designer.slidery_wrapper.w, sys * map_designer.dm.h
    };
    SDL_RenderFillRect(
        map_designer.renderer, &slidery
    );
    const float sxp = map_designer.scrollx / (map_designer.map_sizex * TILESIZEW - map_designer.dm.w);
    const float sxs = (float)map_designer.dm.w / (float)(map_designer.map_sizex * TILESIZEW);
    const SDL_Rect sliderx = {
        (int)(sxp * (map_designer.dm.w - sxs * map_designer.dm.w)), 0,
        (int)(sxs * map_designer.dm.w), map_designer.sliderx_wrapper.h
    };
    SDL_RenderFillRect(
        map_designer.renderer, &sliderx
    );

    const SDL_Rect selection = (SDL_Rect){
        TILESIZEW * (map_designer.selection.x) - map_designer.scrollx + XOFFSET,
        TILESIZEH * (map_designer.selection.y) - map_designer.scrolly + YOFFSET,
        TILESIZEW * map_designer.selection.w, TILESIZEH *  map_designer.selection.h
    };
    
    const SDL_Rect selection2 = (SDL_Rect){
        TILESIZEW * (map_designer.selection2.x) - map_designer.scrollx + XOFFSET,
        TILESIZEH * (map_designer.selection2.y) - map_designer.scrolly + YOFFSET,
        TILESIZEW * map_designer.selection2.w, TILESIZEH * map_designer.selection2.h
    };

    if(map_designer.flags & MDF_SELECTING){
        SDL_SetRenderDrawColor(map_designer.renderer, 159, 26, 147, 100);

        SDL_RenderFillRect(map_designer.renderer, &map_designer.selection);

    } else if(
        (map_designer.flags & MDF_SELECTION_ACTIVE) &&
        (
            point_rect_collision(RECT_POINT_UL(selection), map_designer.map_canvas) ||
            point_rect_collision(RECT_POINT_UR(selection), map_designer.map_canvas) ||
            point_rect_collision(RECT_POINT_BL(selection), map_designer.map_canvas) ||
            point_rect_collision(RECT_POINT_BR(selection), map_designer.map_canvas)
        )
    ){

        SDL_SetRenderDrawColor(map_designer.renderer, 159, 26, 147, 100);

        SDL_RenderFillRect(map_designer.renderer, &selection);
    }
    if(
        (map_designer.flags & MDF_SELECTION2_ACTIVE) &&
        (
            point_rect_collision(RECT_POINT_UL(selection2), map_designer.map_canvas) ||
            point_rect_collision(RECT_POINT_UR(selection2), map_designer.map_canvas) ||
            point_rect_collision(RECT_POINT_BL(selection2), map_designer.map_canvas) ||
            point_rect_collision(RECT_POINT_BR(selection2), map_designer.map_canvas)
        )
    ){
        SDL_SetRenderDrawColor(map_designer.renderer, 147, 159, 100, 100);

        SDL_RenderFillRect(map_designer.renderer, &selection2);
    }

    SDL_SetRenderDrawBlendMode(map_designer.renderer, SDL_BLENDMODE_NONE);
    
    int mx = ((int)((map_designer.mousex + x_offset - (XOFFSET % TILESIZEW)) / TILESIZEW) * TILESIZEW) - x_offset + (XOFFSET % TILESIZEW);
    int my = ((int)((map_designer.mousey + y_offset - (YOFFSET % TILESIZEH)) / TILESIZEH) * TILESIZEH) - y_offset + (YOFFSET % TILESIZEH);
    
    if(map_designer.tiles_sprite_sheet){
        draw_tile((int)map_designer.held_tile, (SDL_Rect){mx + TILESIZEW / 6, my + TILESIZEH / 6, 2 * TILESIZEW / 3, 2 * TILESIZEH / 3});
    } else{
        SDL_SetRenderDrawColor(map_designer.renderer, 159, 26, 147, 255);
        SDL_RenderFillRect(
            map_designer.renderer,
            &(SDL_Rect){mx + TILESIZEW / 6, my + TILESIZEH / 6, 2 * TILESIZEW / 3, 2 * TILESIZEH / 3}
        );
    }
}


static inline bool handle_map_events(Plugin* plugin){
    while (SDL_PollEvent(&event)){
        switch (event.type)
        {
        case SDL_QUIT:
            clean_memory();
            return false;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_r:{
                MapDesigner* state = (MapDesigner*)malloc(sizeof(MapDesigner));
                *state = map_designer;
                plugin->state = (void*)state;
                map_designer.env->overwrite_plugin(plugin, NULL);
                break;
            }
            case SDLK_c:
                if(map_designer.flags & MDF_CTRL)
                    compile();
                else if(map_designer.flags & MDF_SELECTION_ACTIVE){
                    map_designer.flags |= MDF_SELECTION2_ACTIVE;
                    map_designer.selection2 = map_designer.selection;
                }
                break;
            case SDLK_o:
                map_designer.scrollx = 0;
                map_designer.scrolly = 0;
                map_designer.zoom = 1.0f;
                break;
            case SDLK_v:
                if(map_designer.flags & MDF_SELECTION_ACTIVE){
                    size_t sl2_pos = map_designer.selection2.x;
                    sl2_pos += (int)(map_designer.selection2.y) * map_designer.map_sizex;

                    int __reachx = map_designer.selection2.w;
                    __reachx = __reachx < map_designer.selection.w?
                    __reachx : map_designer.selection.w;
                    int __reachy = map_designer.selection2.h + map_designer.selection.y;
                    __reachy = __reachy < (map_designer.selection.y + map_designer.selection.h)?
                    __reachy : (map_designer.selection.y + map_designer.selection.h);

                    for(size_t i = map_designer.selection.y;
                    i < __reachy;
                    i+=1){
                        int* dest = map_designer.map + i * map_designer.map_sizex +
                        (map_designer.selection.x);
                        int* src = map_designer.map + sl2_pos +
                        (size_t)(i - map_designer.selection.y) * map_designer.map_sizex;
                        memcpy(dest, src, sizeof(int) * __reachx);
                    }
                }
                break;
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                if(map_designer.flags & MDF_CTRL) break;
                map_designer.flags |= MDF_CTRL;
                map_designer.sliderx_wrapper.h *= 2;
                map_designer.slidery_wrapper.y = map_designer.sliderx_wrapper.h;
                map_designer.slidery_wrapper.w *= 2;
                map_designer.map_canvas.x = map_designer.slidery_wrapper.w;
                map_designer.map_canvas.y = map_designer.sliderx_wrapper.h;
                break;
            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
                if(map_designer.flags & MDF_CTRL){
                    map_designer.flags &= ~MDF_SELECTION_ACTIVE;
                    map_designer.flags &= ~MDF_SELECTION2_ACTIVE;
                }
                break;
            case SDLK_w:
                map_designer.scrolly_vel = -STD_VEL;
                break;
            case SDLK_s:
                if(map_designer.flags & MDF_CTRL){
                    save_map(map_designer.path_to_map);
                    break;
                }
                map_designer.scrolly_vel = STD_VEL;
                break;
            case SDLK_a:
                    map_designer.scrollx_vel = -STD_VEL;
                break;
            case SDLK_d:
                map_designer.scrollx_vel = STD_VEL;
                break;
            case SDLK_0: map_designer.held_tile = 0; break;
            case SDLK_1: map_designer.held_tile = 1; break;
            case SDLK_2: map_designer.held_tile = 2; break;
            case SDLK_3: map_designer.held_tile = 3; break;
            case SDLK_4: map_designer.held_tile = 4; break;
            case SDLK_5: map_designer.held_tile = 5; break;
            case SDLK_6: map_designer.held_tile = 6; break;
            case SDLK_7: map_designer.held_tile = 7; break;
            case SDLK_8: map_designer.held_tile = 8; break;
            case SDLK_9: map_designer.held_tile = 9; break;

            case SDLK_UP:
                if(map_designer.flags & MDF_SELECTION_ACTIVE){
                    if(map_designer.selection.y >= 1)
                        map_designer.selection.y -= 1;
                }
                break;
            case SDLK_DOWN:
                if(map_designer.flags & MDF_SELECTION_ACTIVE){
                    if(map_designer.selection.y + map_designer.selection.h <= map_designer.map_sizey - 1)
                        map_designer.selection.y += 1;
                }
                break;
            case SDLK_LEFT:
                if(map_designer.flags & MDF_SELECTION_ACTIVE){
                    if(map_designer.selection.x >= 1)
                        map_designer.selection.x -= 1;
                }
                break;
            case SDLK_RIGHT:
                if(map_designer.flags & MDF_SELECTION_ACTIVE){
                    if(map_designer.selection.x + map_designer.selection.w <= map_designer.map_sizex - 1)
                        map_designer.selection.x += 1;
                }
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
            case SDLK_s:
                if(map_designer.flags & MDF_CTRL) break;
                map_designer.scrolly_vel = 0;
                break;
            case SDLK_d:
            case SDLK_a:
                map_designer.scrollx_vel = 0;
                break;
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                if(!(map_designer.flags & MDF_CTRL)) break;
                map_designer.flags &= ~MDF_CTRL;
                map_designer.sliderx_wrapper.h /= 2;
                map_designer.slidery_wrapper.y = map_designer.sliderx_wrapper.h;
                map_designer.slidery_wrapper.w /= 2;
                map_designer.map_canvas.x = map_designer.slidery_wrapper.w;
                map_designer.map_canvas.y = map_designer.sliderx_wrapper.h;
                break;
            case SDLK_e:
            case SDLK_DELETE:
                if(map_designer.flags & MDF_SELECTION_ACTIVE){
                    for(size_t i = (map_designer.selection.y);
                    i < (map_designer.selection.y + map_designer.selection.h);
                    i+=1){
                        int* begin = map_designer.map + i * map_designer.map_sizex +
                        (map_designer.selection.x);
                        memset(begin, 0, sizeof(int) * map_designer.selection.w);
                    }
                }
                break;
            case SDLK_h:
                map_designer.flags |= MDF_HOME;
                map_designer.scrollx_vel = 0;
                map_designer.scrolly_vel = 0;
                map_designer.flags &= ~(
                    MDF_EDIT_MAP | MDF_PLACING | MDF_ERASING | MDF_SELECTING
                    | MDF_SELECTION_ACTIVE | MDF_SELECTION2_ACTIVE
                );
                printf("[INFO] going to home\n");
                break;
            default:
                break;
            }
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT){
                map_designer.flags |= MDF_MOUSE_LBUTTON;
                if(map_designer.mousex > map_designer.map_canvas.x &&
                map_designer.mousex < map_designer.map_canvas.x + map_designer.map_canvas.w &&
                map_designer.mousey > map_designer.map_canvas.y &&
                map_designer.mousey < map_designer.map_canvas.y + map_designer.map_canvas.w){
                    if(map_designer.flags & MDF_CTRL){
                        map_designer.flags |= MDF_SELECTING;
                        map_designer.selection = (SDL_Rect){
                            map_designer.mousex, map_designer.mousey, 0, 0
                        };
                    }
                    else map_designer.flags |= MDF_PLACING;
                }
            } else if(event.button.button == SDL_BUTTON_RIGHT){
                map_designer.flags |= MDF_MOUSE_RBUTTON;
                if(map_designer.mousex > map_designer.map_canvas.x &&
                    map_designer.mousex < map_designer.map_canvas.x + map_designer.map_canvas.w &&
                    map_designer.mousey > map_designer.map_canvas.y &&
                    map_designer.mousey < map_designer.map_canvas.y + map_designer.map_canvas.w){
                    if(map_designer.flags & MDF_CTRL){
                        map_designer.flags |= MDF_SELECTING;
                        map_designer.selection = (SDL_Rect){
                            map_designer.mousex, map_designer.mousey, 0, 0
                        };
                    }
                    else map_designer.flags |= MDF_ERASING;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT){
                map_designer.flags &= ~MDF_MOUSE_LBUTTON;
                if(map_designer.flags & MDF_PLACING) map_designer.flags &= ~MDF_PLACING;
                if(map_designer.flags & MDF_SELECTING){
                    map_designer.flags &= ~MDF_SELECTING;
                    map_designer.flags |= MDF_SELECTION_ACTIVE;
                    if(map_designer.selection.w < 0){
                        map_designer.selection.w = -map_designer.selection.w;
                        map_designer.selection.x -= map_designer.selection.w;
                    }
                    if(map_designer.selection.h < 0){
                        map_designer.selection.h = -map_designer.selection.h;
                        map_designer.selection.y -= map_designer.selection.h ;
                    }
                    map_designer.selection.x =
                    (int)((map_designer.selection.x + map_designer.scrollx - XOFFSET) / TILESIZEW);
                    map_designer.selection.y =
                    (int)((map_designer.selection.y + map_designer.scrolly - YOFFSET) / TILESIZEH);
                    map_designer.selection.w = (int)(1 + map_designer.selection.w / TILESIZEW);
                    map_designer.selection.h = (int)(1 + map_designer.selection.h / TILESIZEH);
                    if(map_designer.selection.x + map_designer.selection.w > map_designer.map_sizex
                    || map_designer.selection.y + map_designer.selection.h > map_designer.map_sizey)
                        map_designer.selection = (SDL_Rect){0, 0, 0, 0};
                }
            } else if(event.button.button == SDL_BUTTON_RIGHT){
                map_designer.flags &= ~MDF_MOUSE_RBUTTON;
                if(map_designer.flags & MDF_ERASING) map_designer.flags &= ~MDF_ERASING;
                if(map_designer.flags & MDF_SELECTING){
                    map_designer.flags &= ~MDF_SELECTING;
                    map_designer.flags |= MDF_SELECTION_ACTIVE;
                    if(map_designer.selection.w < 0){
                        map_designer.selection.w = -map_designer.selection.w;
                        map_designer.selection.x -= map_designer.selection.w;
                    }
                    if(map_designer.selection.h < 0){
                        map_designer.selection.h = -map_designer.selection.h;
                        map_designer.selection.y -= map_designer.selection.h ;
                    }
                    map_designer.selection.x =
                    (int)((map_designer.selection.x + map_designer.scrollx - XOFFSET) / TILESIZEW);
                    map_designer.selection.y =
                    (int)((map_designer.selection.y + map_designer.scrolly - YOFFSET) / TILESIZEH);
                    map_designer.selection.w = (int)(1 + map_designer.selection.w / TILESIZEW);
                    map_designer.selection.h = (int)(1 + map_designer.selection.h / TILESIZEH);

                    if(map_designer.selection.x + map_designer.selection.w > map_designer.map_sizex
                    || map_designer.selection.y + map_designer.selection.h > map_designer.map_sizey)
                        map_designer.selection = (SDL_Rect){0, 0, 0, 0};

                    for(size_t i = (map_designer.selection.y);
                    i < (map_designer.selection.y + map_designer.selection.h);
                    i+=1){
                        int* begin = map_designer.map + i * map_designer.map_sizex +
                        (map_designer.selection.x);
                        memset(begin, 0, sizeof(int) * (map_designer.selection.w));
                    }
                }
            }
            break;
        case SDL_MOUSEWHEEL:
            if(map_designer.flags & MDF_CTRL){
                const float min_zoom = SDL_min(
                    map_designer.dm.w / (map_designer.map_sizex * map_designer.tile_dim_w),
                    map_designer.dm.h / (map_designer.map_sizey * map_designer.tile_dim_h)
                );
                map_designer.zoom += event.wheel.preciseY / 10;
                if(map_designer.zoom < min_zoom) map_designer.zoom = min_zoom;
                else if(map_designer.zoom > 10) map_designer.zoom = 10;
                break;
            }
            map_designer.held_tile += event.wheel.preciseY;
            if(map_designer.held_tile > map_designer.number_of_tiles){
                map_designer.held_tile = map_designer.number_of_tiles;
            } else if(map_designer.held_tile < 0){
                map_designer.held_tile = 0;
            }
            break;
        case SDL_DROPFILE:{
            char* path_to_file = event.drop.file;
            load_map(path_to_file);
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


static inline void scroll_map(){
    map_designer.scrollx += map_designer.scrollx_vel * dt;
    map_designer.scrolly += map_designer.scrolly_vel * dt;
    
    if(map_designer.scrollx + map_designer.map_canvas.w > map_designer.map_sizex * TILESIZEW){
        map_designer.scrollx = map_designer.map_sizex * TILESIZEW - map_designer.map_canvas.w;
    } else if(map_designer.scrollx < 0) map_designer.scrollx = 0;
    
    if(map_designer.scrolly + map_designer.map_canvas.h > map_designer.map_sizey * TILESIZEH){
        map_designer.scrolly = map_designer.map_sizey * TILESIZEH - map_designer.map_canvas.h;
    } else if(map_designer.scrolly < 0) map_designer.scrolly = 0;
    
    map_designer.u_scrollx = map_designer.scrollx;
    map_designer.u_scrolly = map_designer.scrolly;
}


static inline void update_map(){
    scroll_map();

    SDL_SetRenderTarget(map_designer.renderer, map_designer.draw_texture);
    SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);
    SDL_RenderClear(map_designer.renderer);

    draw_map();

    SDL_SetRenderTarget(map_designer.renderer, NULL);
    SDL_RenderCopy(map_designer.renderer, map_designer.draw_texture, NULL, NULL);
    SDL_RenderPresent(map_designer.renderer);

    if(map_designer.flags & MDF_PLACING){
        size_t position = (map_designer.mousex + map_designer.scrollx - XOFFSET) / TILESIZEW;
        position += map_designer.map_sizex * (int)((map_designer.mousey + map_designer.scrolly - YOFFSET) / TILESIZEH);
        if(position > 0 && position < map_designer.map_sizex * map_designer.map_sizey) map_designer.map[position] = (int)map_designer.held_tile;
    } else if(map_designer.flags & MDF_ERASING){
        size_t position = (map_designer.mousex + map_designer.scrollx - XOFFSET) / TILESIZEW;
        position += map_designer.map_sizex * (int)((map_designer.mousey + map_designer.scrolly - YOFFSET) / TILESIZEH);
        if(position > 0 && position < map_designer.map_sizex * map_designer.map_sizey) map_designer.map[position] = 0;
    } else if(map_designer.flags & MDF_SELECTING){
        map_designer.selection.w = map_designer.mousex - map_designer.selection.x;
        map_designer.selection.h = map_designer.mousey - map_designer.selection.y;
    } else if(map_designer.flags & MDF_MOUSE_LBUTTON){
        if(map_designer.mousey < map_designer.slidery_wrapper.y){
            int mouse_rel_posx = map_designer.mousex - map_designer.sliderx_wrapper.x;
            map_designer.scrollx = ((float)mouse_rel_posx / (float)map_designer.sliderx_wrapper.w)
            * (map_designer.map_sizex * TILESIZEW - map_designer.map_canvas.w);
        } else if(map_designer.mousex < map_designer.slidery_wrapper.w){
            int mouse_rel_posy = map_designer.mousey - map_designer.slidery_wrapper.y;
            mouse_rel_posy = (mouse_rel_posy < 0) ? 0 : mouse_rel_posy;
            map_designer.scrolly = ((float)mouse_rel_posy / (float)map_designer.slidery_wrapper.h)
            * (map_designer.map_sizey * TILESIZEH - map_designer.map_canvas.h);
        }
    }
}



#endif // END OF FILE =================================================================================