#ifndef LE_MAPDESIGNER
#define LE_MAPDESIGNER

#include "funcs.h"
#include "../src/environment.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define CODE(_CODE) _CODE

#define TILE_CAP 100

#define STD_VEL 0.5

#define TILESIZEW (int)(map_designer.tile_dim_w * map_designer.zoom)
#define TILESIZEH (int)(map_designer.tile_dim_h * map_designer.zoom)
#define TILESIZE TILESIZEW, TILESIZEH

#define XOFFSET map_designer.map_canvas.x
#define YOFFSET map_designer.map_canvas.y

#define TEXT_BUFFER_ICAP 1000

#define CHAR_SIZEW 50
#define CHAR_SIZEH 50

#define MOUSE_POSITION map_designer.mousex, map_designer.mousey
#define MOUSE_POSITION_POINT (SDL_Point){MOUSE_POSITION};


#define EXPAND_RECT_E(RECT, XF, YF, WF, HF) XF(RECT.x), YF(RECT.y), WF(RECT.w), HF(RECT.h)

#define EXPAND_RECT(RECT) RECT.x, RECT.y, RECT.w, RECT.h


#define SPRITE_SHEET_SIZE SPRITE_SHEET_W, SPRITE_SHEET_H

#define SPECIAL_CHARACTERS "./_'"

#define SUPPORTED_CHARACTERS "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" SPECIAL_CHARACTERS

#define N_SUP_CHAR (sizeof(SUPPORTED_CHARACTERS) / sizeof(char))

#define ARRAY_SIZE(TYPE, ARRAY) (sizeof(ARRAY) / sizeof(TYPE))


#define MAX_MAPSIZEX 1000
#define MAX_MAPSIZEY 1000

#define STD_EVENT_HANDLIND()\
    case SDL_WINDOWEVENT:\
        if(event.window.event == SDL_WINDOWEVENT_RESIZED){\
            map_designer.wdtfx = (float)map_designer.dm.w / (float)event.window.data1;\
            map_designer.wdtfy = (float)map_designer.dm.h / (float)event.window.data2;\
        }\
        break;\
    case SDL_MOUSEMOTION:\
        map_designer.mousex = (int)(float)(event.motion.x * (float)map_designer.wdtfx);\
        map_designer.mousey = (int)(float)(event.motion.y * (float)map_designer.wdtfy);\
        break

SDL_Event event;
Uint64 t = 0;

static bool place, erase;

static int x_offset = 0, y_offset = 0;

float dt = 0;


typedef struct{
int id;
SDL_Texture* sprite;
} Tile;

enum MapDesignerFlags{
MDF_NONE = 0,
MDF_PLACING = 1 << 0,
MDF_ERASING = 1 << 1,
MDF_SELECTING = 1 << 2,
MDF_MOUSE_RBUTTON = 1 << 3,
MDF_MOUSE_LBUTTON = 1 << 4,
MDF_MOUSE_MBUTTON = 1 << 5,
MDF_CTRL = 1 << 6,
MDF_HOME = 1 << 7,
MDF_SAVING = 1 << 8,
MDF_LOADING = 1 << 9,
MDF_SETTING_TILE = 1 << 10,
MDF_TYPING = 1 << 11,
MDF_SELECTION_ACTIVE = 1 << 12,
MDF_EDIT_MAP = 1 << 13,
MDF_SUDO_MOD = 1 << 14,
MDF_SELECTION2_ACTIVE = 1 << 15,
MDF_SETTINGS_CHANGED = 1 << 16,
MDF_DEBUG = 1 << 17
};

enum TextPurpose{
TP_NONE = 0,
TP_LOAD_PATH,
TP_SAVE_PATH,
TP_MSX,
TP_MSY,
TP_TW,
TP_TH,
TP_SET_TILE_PATH,
TP_SET_TILE_ID
};

typedef struct{
Env* env;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* draw_texture;
TTF_Font* font;

SDL_Texture* characters;
SDL_Rect char_sheet;
SDL_Texture* tiles_sprite_sheet;
SDL_FRect tile_sheet_camera;
SDL_Rect tile_sheet;
int tile_dim_w, tile_dim_h;

SDL_DisplayMode dm;
float mousex, mousey;
float wdtfx, wdtfy;

float held_tile;
unsigned int number_of_tiles;

float scrollx, scrolly;
int u_scrollx, u_scrolly;
float scrollx_vel, scrolly_vel;

int* map;
size_t map_sizex, map_sizey;
SDL_Rect map_canvas;

SDL_Rect sliderx_wrapper, slidery_wrapper;
uint64_t flags;

SDL_Rect selection, selection2;

char text_buffer[TEXT_BUFFER_ICAP];
unsigned int text_position;
SDL_Rect text_canvas;
int text_purpose;

char path_to_map[TEXT_BUFFER_ICAP];
int size_of_map_path;
bool reload_map;
size_t n_map_sizex, n_map_sizey;

float zoom;
} MapDesigner;

MapDesigner map_designer;

enum TextError{
TEXT_ERROR_NONE = 0,
TEXT_ERROR_CANVAS_OVERFLOW = 1 << 0,
TEXT_ERROR_UNSUPPORTED_CHARACTER = 1 << 2
};

int text_err_special_flags = 0;

extern void compile();

static inline void load_map(const char* path){
    printf("[INFO] Loading Map '%s'\n", path);
    
    SDL_Surface* surface = IMG_Load(path);
    if(!surface){
        printf("[WARNING] Load Failed, %s.\n", IMG_GetError());
        return;
    }

    SDL_PixelFormat* f = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    SDL_Surface* converted_surface = SDL_ConvertSurface(surface, f, 0);
    SDL_FreeFormat(f);
    SDL_FreeSurface(surface);

    if(!converted_surface){
        printf("[WARNING] Load Failed, %s.\n", SDL_GetError());
        return;
    }

    SDL_LockSurface(converted_surface);
    void* pixels = converted_surface->pixels;
    int picth = converted_surface->pitch;
    map_designer.map_sizex = converted_surface->w;
    map_designer.map_sizey = converted_surface->h;

    if(map_designer.map) free(map_designer.map);
    map_designer.map = (int*)malloc(map_designer.map_sizex * map_designer.map_sizey * sizeof(int));

    for(size_t i = 0; i < map_designer.map_sizey; i+=1){
        for(size_t j = 0; j < map_designer.map_sizex; j+=1){
            map_designer.map[j + i * map_designer.map_sizex] = *(int*)(Uint32*)(pixels + j * sizeof(Uint32) + i * picth);
        }
    }

    SDL_UnlockSurface(converted_surface);
    SDL_FreeSurface(converted_surface);

    printf("[INFO] Map '%s' Loaded Successfully!\n", path);
}

static inline void save_map(const char* path){
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, map_designer.map_sizex, map_designer.map_sizey, 4, SDL_PIXELFORMAT_RGBA32);
    printf("[INFO] Saving Map To '%s'\n", path);
    if(!surface){
        printf("[WARNING] Save Failed, Unable To Create Surface: %s.\n", IMG_GetError());
        return;
    }

    SDL_LockSurface(surface);

    void* pixels = surface->pixels;
    int pitch = surface->pitch;

    for(int i = 0; i < map_designer.map_sizey; i+=1){
        for(int j = 0; j < map_designer.map_sizex; j+=1){
            *(Uint32*)(pixels + j * sizeof(Uint32) + i * pitch) = map_designer.map[j + i * map_designer.map_sizex];
        }
    }
    SDL_UnlockSurface(surface);
    if(IMG_SavePNG(surface, path))
        printf("[WANRING] Save Failed: %s.\n", IMG_GetError());
    else
        printf("[INFO] Map Saved To '%s' Successfully!\n", path);
    SDL_FreeSurface(surface);
}

static inline void clean_memory(){
    free(map_designer.map);
    map_designer.map = NULL;
    SDL_DestroyRenderer(map_designer.renderer);
    map_designer.renderer = NULL;
    SDL_DestroyWindow(map_designer.window);
    map_designer.window = NULL;
    SDL_DestroyTexture(map_designer.characters);
    map_designer.characters = NULL;
    SDL_DestroyTexture(map_designer.tiles_sprite_sheet);
    map_designer.tiles_sprite_sheet = NULL;
    SDL_DestroyTexture(map_designer.draw_texture);
    map_designer.draw_texture = NULL;
    TTF_CloseFont(map_designer.font);
    map_designer.font = NULL;
    void* subsystem = map_designer.env->channel.channel;
    void(*close_subsystem)() = map_designer.env->get_object_from_symbol(subsystem, "close");
    close_subsystem();
}

static inline void draw_thick_rect(SDL_Rect rect, int thickness){
    rect.x -= thickness / 2;
    rect.y -= thickness / 2;
    SDL_RenderDrawRect(map_designer.renderer, &rect);
    for(int i = 1; i < thickness; i+=1){
        rect.x += 1;
        rect.y += 1;
        rect.w -= 2;
        rect.h -= 2;
        SDL_RenderDrawRect(map_designer.renderer, &rect);
    }
}

static inline bool point_rect_collision(const SDL_Point point, const SDL_Rect rect){
    if((abs(point.x - rect.x) < abs(rect.w) && abs(point.y - rect.y) < abs(rect.h))){
        return ((point.x - rect.x) * rect.w > 0 && (point.y - rect.y) * rect.h > 0);
    }
    return false;
}

void init_chars_sheet(){

    SDL_SetRenderTarget(map_designer.renderer, map_designer.characters);
    SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);
    SDL_RenderClear(map_designer.renderer);


    
    for(int i = 0; i < N_SUP_CHAR; i+=1){
        SDL_Surface* text_surface = TTF_RenderText_Solid(
        map_designer.font,
        (char[2]){SUPPORTED_CHARACTERS[i], '\0'},
        (SDL_Color){255, 255, 255, 255}
        );
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(map_designer.renderer, text_surface);
        SDL_FreeSurface(text_surface);

        SDL_Rect dest= (SDL_Rect){
            (i * CHAR_SIZEW) % map_designer.char_sheet.w, (int)((i * CHAR_SIZEW) / map_designer.char_sheet.w) * CHAR_SIZEH,
            CHAR_SIZEW, CHAR_SIZEH
        };

        SDL_RenderCopy(map_designer.renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);
    }
}

static inline void get_text_input(const char* text){
    for(int i = 0; text[i]; i+=1){
        if(map_designer.text_position >= TEXT_BUFFER_ICAP){
            printf("[WARNING] Text Buffer Filled\n");
            return;
        }
        map_designer.text_buffer[map_designer.text_position] = text[i];
        map_designer.text_position += 1;
    }
}

static inline int get_char_sprite_id(char c){

    const char* spc = SUPPORTED_CHARACTERS;
    
    
    for(int i = 0; i < N_SUP_CHAR; i+=1){
        if(c == spc[i]){
            return i;
        }
    } return -1;
}

static inline void draw_text(const char* text, SDL_Rect char_canvas, const SDL_Rect canvas){

    char_canvas.x = canvas.x;
    char_canvas.y = canvas.y;

    for(int i = 0; text[i]; i+=1){
        if(text[i] == ' '){
            char_canvas.x += char_canvas.w;
            if(char_canvas.x + char_canvas.w > canvas.x + canvas.w){
                char_canvas.y += (int)((char_canvas.x - canvas.x) / canvas.w) * char_canvas.h;
                if(char_canvas.y > canvas.y + canvas.h){
                    if(!(text_err_special_flags & TEXT_ERROR_CANVAS_OVERFLOW)){
                        printf("[WARNING] Unable To Draw Full Text, Text Canvas Overflow.\n");
                        text_err_special_flags |= TEXT_ERROR_CANVAS_OVERFLOW;
                    }
                    break;
                }
                char_canvas.x = canvas.x + ((char_canvas.x - canvas.x) % canvas.w);
            } continue;
        } else if(text[i] == '\n'){
            char_canvas.y += char_canvas.h;
            if(char_canvas.y + char_canvas.h > canvas.y + canvas.h){
                if(!(text_err_special_flags & TEXT_ERROR_CANVAS_OVERFLOW)){
                    printf("[WARNING] Unable To Draw Full Text, Text Canvas Overflow.\n");
                    text_err_special_flags |= TEXT_ERROR_CANVAS_OVERFLOW;
                }
                break;
            }
            char_canvas.x = canvas.x;
            continue;
        }
        int id = get_char_sprite_id(text[i]);
        if(id < 0){
            if(!(text_err_special_flags & TEXT_ERROR_UNSUPPORTED_CHARACTER)){
                printf("[WARNING] Unsupported Character \'%c\'\n", text[i]);
                text_err_special_flags |= TEXT_ERROR_UNSUPPORTED_CHARACTER;
            } continue;
        }
        SDL_Rect char_src = (SDL_Rect){
            (id * CHAR_SIZEW) % map_designer.char_sheet.w, (int)((id * CHAR_SIZEH) / map_designer.char_sheet.w) * CHAR_SIZEH,
            CHAR_SIZEW, CHAR_SIZEH
        };
        SDL_RenderCopy(map_designer.renderer, map_designer.characters, &char_src, &char_canvas);

        char_canvas.x += char_canvas.w;
        if(char_canvas.x + char_canvas.w > canvas.x + canvas.w){
            char_canvas.y += (int)((char_canvas.x - canvas.x) / canvas.w) * char_canvas.h;
            if(char_canvas.y + char_canvas.h > canvas.y + canvas.h){
                if(!(text_err_special_flags & TEXT_ERROR_CANVAS_OVERFLOW)){
                    printf("[WARNING] Unable To Draw Full Text, Text Canvas Overflow.\n");
                    text_err_special_flags |= TEXT_ERROR_CANVAS_OVERFLOW;
                }
                break;;
            }
            char_canvas.x = canvas.x + ((char_canvas.x - canvas.x) % canvas.w);
        }
    }
}

static inline void draw_number(int number, SDL_Rect digit_canvas, const SDL_Rect canvas){
    int nd = 0;
    for(int nb = number; nb; nb /= 10){
        nd += 1;
    }
    char* str = (char*)alloca((nd + 1) * sizeof(char));
    to_string(number, str, nd);
    draw_text(str, digit_canvas, canvas);
    
}

void load_tile_sheet(const char* path, const SDL_Rect tile_sheet_camera){
    SDL_Surface* tile_sheet_s = IMG_Load(path);
    printf("[INFO] Loading Sprite Sheet '%s'\n", path);
    if(!tile_sheet_s){
        printf("[WARNING] Unable To Load Sprite Sheet '%s', %s\n", path, IMG_GetError());
        SDL_ClearError();
        return;
    }
    map_designer.tile_sheet.w = tile_sheet_s->w;
    map_designer.tile_sheet.h = tile_sheet_s->h;
    if(map_designer.tiles_sprite_sheet) SDL_DestroyTexture(map_designer.tiles_sprite_sheet);
    map_designer.tiles_sprite_sheet = SDL_CreateTextureFromSurface(map_designer.renderer, tile_sheet_s);
    SDL_FreeSurface(tile_sheet_s);
    map_designer.number_of_tiles = (int)(map_designer.tile_sheet.w / map_designer.tile_dim_w) *
        (int)(map_designer.tile_sheet.h / map_designer.tile_dim_h);
    if(map_designer.held_tile > map_designer.number_of_tiles - 1) map_designer.held_tile = map_designer.number_of_tiles - 1;
    map_designer.tile_sheet_camera = (SDL_FRect){
        (float)0, (float)0,
        (float)map_designer.tile_dim_w, (float)map_designer.tile_dim_h
    };
    printf("[INFO] Load Of Sprite Sheet '%s' Successfully\n", path);
}

static inline void draw_tile(int tile, const SDL_Rect dest){
    tile -= 1;
    const SDL_Rect src = (SDL_Rect){
        (map_designer.tile_sheet.x + tile * map_designer.tile_dim_w) % map_designer.tile_sheet.w,
        map_designer.tile_sheet.y + (int)(tile * map_designer.tile_dim_w/ map_designer.tile_sheet.w) * map_designer.tile_dim_h,
        map_designer.tile_dim_w, map_designer.tile_dim_h
    };
    SDL_RenderCopy(map_designer.renderer, map_designer.tiles_sprite_sheet, &src, &dest);
}

static inline void fill(SDL_Texture* texture){
    SDL_SetRenderTarget(map_designer.renderer, texture);
    SDL_SetRenderDrawColor(map_designer.renderer, 0, 0, 0, 255);
    SDL_RenderClear(map_designer.renderer);
}

#endif // END OF FILE ============================================