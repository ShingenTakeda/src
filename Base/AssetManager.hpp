#pragma once
#include <map>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <Logger/Logger.hpp>

struct Audio
{
    Mix_Music *music = nullptr;
    Mix_Chunk *scratch = nullptr;
    Mix_Chunk *high = nullptr;
    Mix_Chunk *medium = nullptr;
    Mix_Chunk *low = nullptr;
};

struct Map
{
    SDL_Rect rect;
    int tileSize;
    int tileScale;
    double scale;
    int rows;
    int columns;
    std::string data;
};

//TODO: Add sounds,TTF
class AssetManager
{
    public:
        AssetManager();
        ~AssetManager();
        
        void ClearAssets();
        
        //Texture work
        void AddTexture(SDL_Renderer *renderer, const std::string &assetID, const std::string &filePath);
        SDL_Texture *GetTexture(const std::string &assetID);
        
        //Sound work
        void AddSound(const std::string &assetID, const std::string &filePath);
        //TODO:Add GetSound();
        
        //TODO: TTF work
        void AddTexture(const std::string &assetID, const std::string &filePath);
        
        //TODO: Map
        void AddMap(const std::string &assetID, const std::string &filePath);
        
    private:
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, Audio*> audio;
        std::map<std::string, TTF_Font*> fonts;
        std::map<std::string, Map*> maps;
};
