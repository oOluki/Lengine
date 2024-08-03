#ifndef LE_FIRST_PLUGIN
#define LE_FIRST_PLUGIN
#include "SDL2/SDL.h"
#include <stdlib.h>
#include <stdbool.h>
#include "../src/plugins.h"

#define DRAW_TEXTURE_SIZEX 1200
#define DRAW_TEXTURE_SIZEY 720
#define DRAW_TEXTURE_SIZE DRAW_TEXTURE_SIZEX, DRAW_TEXTURE_SIZEY

SDL_Event event;
Uint64 t;
Env* environment = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* draw_texture = NULL;


int mousex = 0, mousey = 0;

typedef struct{
    Env* env;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* draw_texture;
    int targetx, targety;
    SDL_Rect rect;
    float x, y;
    float wdss_factorx, wdss_factory;
} PluginState;

PluginState plugin;

void plugin_init(Env* env){
    printf("[INFO] Plugin: Hi From Plugin At %p\n", &event);
    printf("[INFO] Plugin: Initiation Of Plugin Requested From %p\n", env);

    environment = env;
    EnvSubSysFlags flags;
    flags.Env_sdl_flags = SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO;
    env->init_subsystem(ENV_SDL, flags);

    SDL_DisplayMode dm;
    SDL_GetDisplayMode(0, 0, &dm);
    window = SDL_CreateWindow("Template Plugin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w / 2, dm.h / 2, SDL_WINDOW_RESIZABLE); 
    renderer = SDL_CreateRenderer(window, -1, 0);
    draw_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
    DRAW_TEXTURE_SIZE);

    plugin = (PluginState){env, window, renderer, draw_texture};
    plugin.wdss_factorx = 2.00000f * (float)(DRAW_TEXTURE_SIZEX) / (float)(dm.w);
    plugin.wdss_factory = 2.00000f * (float)(DRAW_TEXTURE_SIZEY) / (float)(dm.h);
    plugin.rect = (SDL_Rect){0, 0, 50, 50};
    t = SDL_GetTicks64();
}

static void quit(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(draw_texture);
    plugin.env->close_subsystem(ENV_SDL);
}

static inline void draw_thick_rect(SDL_Rect rect, SDL_Color color, unsigned int thickness){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for(unsigned int i = 0; i < thickness; i+=1){
        SDL_RenderDrawRect(renderer, &rect);
        rect.x += 1; rect.y += 1;
        rect.w -= 2; rect.h -= 2;
    }
}

bool plugin_update(Plugin* self){
    static double frame = 0;
    float delta_time = 0;
    static float max_dt = 0;
    {
        Uint64 dt = SDL_GetTicks64() - t;
        if(dt < 1000 / 60){
            SDL_Delay(1000 / 60 - dt);
            dt = 1000 / 60;
        }
        t = SDL_GetTicks64();
        delta_time = (float)dt / 1000.0000f;
    }
    if(max_dt < delta_time - 0.0001f){
        max_dt = delta_time;
        printf("%f : max_dt = %f\n", frame, max_dt);
    }
    plugin.x += 10 * delta_time * (plugin.targetx - plugin.x);
    plugin.y += 10 * delta_time * (plugin.targety - plugin.y);
    plugin.rect.x = (int)plugin.x;
    plugin.rect.y = (int)plugin.y;

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, draw_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_thick_rect(plugin.rect, (SDL_Color){159, 25, 69, 255}, 5);

    SDL_SetRenderDrawColor(renderer, 20, 159, 15, 255);

    SDL_RenderDrawPoint(renderer, mousex, mousey);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, draw_texture, NULL, NULL);

    SDL_RenderPresent(renderer);

    while (SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            quit();
            self->flags = PLUGIN_QUIT;
            return false;
        }
        if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED){
            plugin.wdss_factorx = (float)(DRAW_TEXTURE_SIZEX) / (float)(event.window.data1);
            plugin.wdss_factory = (float)(DRAW_TEXTURE_SIZEY) / (float)(event.window.data2);
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_r){
                void* state = malloc(sizeof(PluginState));
                *(PluginState*)state = plugin;
                self->state = (void*)(state);
                self->flags |= PLUGIN_HOT_RELOAD;
                return false;
            }
            if(event.key.keysym.sym == SDLK_c){
                if(system("gcc -fPIC -shared plugins/plugin.c -o plugins/plugin.so")){
                    printf("[WARNING] Recompilation Failed\n");
                } else{
                    printf("[INFO] Compilation Success, No Erros, Check For Warnings.\n");
                }
            }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
            plugin.targetx = mousex;
            plugin.targety = mousey;
        }
        if(event.type == SDL_MOUSEMOTION){
            mousex = ((float)(event.motion.x) * (float)(plugin.wdss_factorx));
            mousey = ((float)(event.motion.y) * (float)(plugin.wdss_factory));
        }
    }
    frame += 1;
    return true;
    
}

void plugin_retrieve_state(void* plugin_state){
    printf("getting state from %p to %p\n", plugin_state, &plugin);
    plugin = *(PluginState*)plugin_state;
    free(plugin_state);
    environment = plugin.env;
    window = plugin.window;
    renderer = plugin.renderer;
    draw_texture = plugin.draw_texture;
    t = SDL_GetTicks64();
}







#endif // END OF CODE ========================================