#pragma once
#include "../Core/__init__.hpp"
#ifdef LE_INTERNAL_INITMACRO_SDL

namespace Le::SDL{


class Mixer{
public:
Mixer(){}

void Init(int frequency = 44100, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 2048){
    Mix_OpenAudio(frequency, format, channels, chunksize);
}

void Close(){
    Mix_CloseAudio();
}

void Play(Mix_Chunk* sound, int channel = -1, int loops = 0)
{
Mix_PlayChannel(channel, sound, loops);
}

void FadeChannel(int channel, int ms, Mix_Chunk* chunk = nullptr, int loops = 0, int ticks = -1)
{
if (ticks < 0){
if (ms >= 0) Mix_FadeOutChannel(channel, ms);
else Mix_FadeInChannel(channel, chunk, loops, -ms);
}
else{
if (ms < 0) ms = -ms;
Mix_FadeInChannelTimed(channel, chunk, loops, ms, ticks);
}
}

void PauseChannel(int channel = -1){
    Mix_Pause(channel);
}

void HaltChannel(int channel = -1){
    Mix_HaltChannel(channel);
}

void ResumeChannel(int channel = -1){
    Mix_Resume(channel);
}

void PlayMusic(Mix_Music* music, int loops = 0)
{
Mix_PlayMusic(music, loops);
}

void FadeMusic(int ms, Mix_Music* music = nullptr, int loops = 0)
{
if (ms < 0) {
    Mix_FadeInMusic(music, loops, -ms); return;
}
Mix_FadeOutMusic(ms);
}

void PauseMusic(){
    Mix_PauseMusic();
}

void HaltMusic(){
    Mix_HaltMusic();
}

void ResumeMusic(){
    Mix_ResumeMusic();
}

void DestroySound(Mix_Chunk* sound){
    Mix_FreeChunk(sound);
}

void DestroyMusic(Mix_Music* music){
    Mix_FreeMusic(music);
}

Mix_Chunk* LoadSound(const char* path, int volume = -1)
{
Mix_Chunk* sound = Mix_LoadWAV(path); Mix_VolumeChunk(sound, volume); return sound;
}

Mix_Music* LoadMusic(const char* path)
{
Mix_Music* music = Mix_LoadMUS(path); return music;
}

int SoundVolume(Mix_Chunk* sound, int volume){
    return Mix_VolumeChunk(sound, volume);
}

int ChannelVolume(int channel = -1, int volume = -1)
{
return Mix_Volume(channel, volume);
}

int MusicVolume(int volume = -1)
{
return Mix_VolumeMusic(volume);
}

int SetMusicPosition(double position = 0, bool rewind = false)
{
if (!position || rewind) {Mix_RewindMusic(); return 1;}
return Mix_SetMusicPosition(position);
}

int ChannelState(int channel = -1)
{
return Mix_Playing(channel);
}

int MusicState(){
return Mix_PlayingMusic();
}

};


}

#endif
