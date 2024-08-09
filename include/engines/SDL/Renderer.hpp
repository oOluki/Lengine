#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_SDL



namespace Le::SDL{    

enum BlendModes{
    BLEND = SDL_BLENDMODE_BLEND, ADD = SDL_BLENDMODE_ADD, MUL = SDL_BLENDMODE_MUL,
    NONE = SDL_BLENDMODE_NONE, MOD = SDL_BLENDMODE_MOD
};


class Renderer{
private:

    SDL_Renderer* _Renderer;

public:

Renderer(){}

void Init(SDL_Window* window, Uint32 flags = 0, int index = -1){
    _Renderer = SDL_CreateRenderer(window, index, flags);
}

void Close(){
    SDL_DestroyRenderer(_Renderer);
}

void SetRenderDrawColor(int red, int green, int blue, int alpha = 255){
    SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
}

void SetRenderTarget(SDL_Texture* target){
    SDL_SetRenderTarget(_Renderer, target);
}

void SetRenderDrawBlendMode(SDL_BlendMode blend_mode){
    SDL_SetRenderDrawBlendMode(_Renderer, blend_mode);
}

void Clear(){
    SDL_SetRenderTarget(_Renderer, nullptr);
    SDL_SetRenderDrawColor(_Renderer, 0, 0, 0, 255);
    SDL_RenderClear(_Renderer);
}

void DrawHollowRect(SDL_Rect rect, int red, int green, int blue, int alpha, SDL_Texture* texture)
{
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawRect(_Renderer, &rect);
}

void DrawFilledRect(SDL_Rect rect, int red, int green, int blue, int alpha, SDL_Texture* texture)
{
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderFillRect(_Renderer, &rect);
}

void DrawPoint(int x, int y, int red, int green, int blue, int alpha, SDL_Texture* texture){
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawPoint(_Renderer, x, y);
}

void DrawPoints(const SDL_Point* points, int size, int red, int green, int blue, int alpha, SDL_Texture* texture)
{
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawPoints(_Renderer, points, size);
}

void DrawLine(int x1, int y1, int x2, int y2, int red, int green, int blue, int alpha, SDL_Texture* texture){
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawLine(_Renderer, x1, y1, x2, y2);
}

// Draws a bunch of lines next to each other so it appears thick: Need testing still
void DrawThickLine(int x1, int y1, int x2, int y2, int thickness, int red, int green, int blue, int alpha,
SDL_Texture* texture){
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
float normal[] = {float(y2 - y1), float(x1 - x2)};
float m = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1]);
normal[0] /= m;
normal[1] /= m;
for(float i = - float(thickness) / 2.000000f; i <= float(thickness) / 2.0000f; i++){
    SDL_RenderDrawLine(_Renderer, x1 + i * normal[0], y1 + i * normal[1],
    x2 + i * normal[0], y2 + i * normal[1]);
}
}

void DrawLines(const SDL_Point* points, int number_of_points, int red, int green, int blue, int alpha,
SDL_Texture* texture){
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawLines(_Renderer, points, number_of_points);
}

void DrawTexture(SDL_Texture* source_texture, SDL_Rect* source, SDL_Rect* destiny,
 SDL_Texture* destiny_texture)
{
SDL_SetRenderTarget(_Renderer, destiny_texture);
SDL_RenderCopy(_Renderer, source_texture, source, destiny);
}

void ExDrawTexture(SDL_Texture* source_texture, SDL_Texture*& destiny_texture, const double angle,
SDL_Rect* source = nullptr, SDL_Rect* destiny = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Point* center = nullptr)
{
SDL_SetRenderTarget(_Renderer, destiny_texture);
SDL_RenderCopyEx(_Renderer, source_texture, source, destiny, angle, center, flip);
}

void Fill(SDL_Texture* texture, int red, int green, int blue, int alpha = 255)
{
SDL_SetRenderTarget(_Renderer, texture);
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderClear(_Renderer);
}

// Lower Level Draw Functions ================================================================================
void DrawHollowRect(SDL_Rect rect, int red, int green,
int blue, int alpha = 255)
{
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawRect(_Renderer, &rect);
}

void DrawFilledRect(SDL_Rect rect, int red, int green,
int blue, int alpha = 255)
{
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderFillRect(_Renderer, &rect);
}

void DrawPoint(int x, int y, int red, int green,
int blue, int alpha = 255){
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawPoint(_Renderer, x, y);
}

void DrawPoints(const std::vector<SDL_Point>& points, int red, int green,
int blue, int alpha = 255)
{
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawPoints(_Renderer, points.data(), points.size());
}

void DrawLine(int x1, int y1, int x2, int y2, int red, int green,
int blue, int alpha = 255){
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawLine(_Renderer, x1, y1, x2, y2);
}

// Draws a bunch of lines next to each other so it appears thick: Need testing still
void DrawThickLine(int x1, int y1, int x2, int y2, int thickness, int red, int green, int blue, int alpha = 255){
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
float normal[] = {float(y2 - y1), float(x1 - x2)};
float m = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1]);
normal[0] /= m;
normal[1] /= m;
for(float i = - float(thickness) / 2.000000f; i <= float(thickness) / 2.0000f; i++){
    SDL_RenderDrawLine(_Renderer, x1 + i * normal[0], y1 + i * normal[1],
    x2 + i * normal[0], y2 + i * normal[1]);
}
}

void DrawLines(const SDL_Point* points, int number_of_points, int red, int green,
int blue, int alpha = 255){
SDL_SetRenderDrawColor(_Renderer, red, green, blue, alpha);
SDL_RenderDrawLines(_Renderer, points, number_of_points);
}

void DrawTexture(SDL_Texture* source_texture, SDL_Rect* source = nullptr, SDL_Rect* destiny = nullptr)
{
SDL_RenderCopy(_Renderer, source_texture, source, destiny);
}

void ExDrawTexture(SDL_Texture* source_texture, const double angle,
SDL_Rect* source = nullptr, SDL_Rect* destiny = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Point* center = nullptr)
{
SDL_RenderCopyEx(_Renderer, source_texture, source, destiny, angle, center, flip);
}


void SetTextureBlendMode(SDL_Texture* texture, SDL_BlendMode blend_mode){
    SDL_SetTextureBlendMode(texture, blend_mode);
}

void Blit(SDL_Surface* source_surface, SDL_Surface* destiny_surface,
SDL_Rect* source_rect = nullptr, SDL_Rect* destiny_rect = nullptr)
{
SDL_BlitSurface(source_surface, source_rect, destiny_surface, destiny_rect);
}

void SetSurfaceBlendMode(SDL_Surface* surface, SDL_BlendMode blend_mode){
    SDL_SetSurfaceBlendMode(surface, blend_mode);
}

void Render()
{
SDL_RenderPresent(_Renderer);
}

void DestroyTexture(SDL_Texture* texture){
    SDL_DestroyTexture(texture);
}

void DestroySurface(SDL_Surface* surface){
    SDL_FreeSurface(surface);
}

void SaveImgBMP(SDL_Surface* surface, const char* path){
    SDL_RWops* Rw = SDL_RWFromFile(path, "wb");
    SDL_SaveBMP_RW(surface, Rw, 1);
    SDL_RWclose(Rw);
}

void SaveImgPNG(SDL_Surface* surface, const char* path){
    IMG_SavePNG(surface, path);
}

void SaveImgJPG(SDL_Surface* surface, const char* path, int quality){
    IMG_SaveJPG(surface, path, quality);
}

SDL_Texture* GetRenderTarget(){
    return SDL_GetRenderTarget(_Renderer);
}

SDL_Texture* Texture(int width, int height, Uint32 format = SDL_PIXELFORMAT_RGBA8888, SDL_TextureAccess access = SDL_TEXTUREACCESS_TARGET){
SDL_Texture* texture = SDL_CreateTexture(_Renderer, format, access, width, height);
return texture;
}

SDL_Texture* ImgTexture(const char* path)
{
SDL_Surface* dummy = IMG_Load(path);
SDL_Texture* texture = SDL_CreateTextureFromSurface(_Renderer, dummy);
SDL_FreeSurface(dummy);
return texture;
}

SDL_Surface* ImgSurface(const char* path){
    return IMG_Load(path);
}

SDL_Texture* TextureFromSurface(SDL_Surface* source_surface)
{
return SDL_CreateTextureFromSurface(_Renderer, source_surface);
}

SDL_Texture* Text(const char* string, TTF_Font* font, unsigned char red, unsigned char green, unsigned char blue)
{
SDL_Color textcolor = {red, green, blue};
SDL_Surface* TextSurface = TTF_RenderText_Solid(font, string, textcolor);
SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(_Renderer, TextSurface);
SDL_FreeSurface(TextSurface); return TextTexture;
}

SDL_Surface* Surface(int width, int height, int depth = 32, Uint32 Rmask = 0, Uint32 Gmask = 0, Uint32 Bmask = 0,
Uint32 Amask = 1, SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE){
    SDL_Surface* out_surface = SDL_CreateRGBSurface(0, width, height, depth, Rmask, Gmask, Bmask, Amask);
    SDL_SetSurfaceBlendMode(out_surface, blend_mode);
    return out_surface;
}

SDL_Surface* Surface(int width, int height, int depth = 32, Uint32 format = SDL_PIXELFORMAT_RGBA32, SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE){
    SDL_Surface* out_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format);
    SDL_SetSurfaceBlendMode(out_surface, blend_mode);
    return out_surface;
}

SDL_Surface* SufaceFromTexture(SDL_Texture* texture, Uint32 format = SDL_PIXELFORMAT_RGBA32){
    SDL_Surface* surface;
    SDL_Texture* target = SDL_GetRenderTarget(_Renderer);
    SDL_SetRenderTarget(_Renderer, texture);
    SDL_RenderReadPixels(_Renderer, nullptr, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(_Renderer, target);
    return surface;
}

};


}



#endif
