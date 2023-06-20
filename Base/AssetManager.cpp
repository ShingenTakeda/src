#include <SDL2/SDL_image.h>
#include <Base/AssetManager.hpp>

//TODO:Add checks to verify if a file is valid/exists
//TODO:Add TTF and audio

void DestroyAudioStruct(Audio *audio)
{
    Mix_FreeChunk(audio->scratch);
    Mix_FreeChunk(audio->high);
    Mix_FreeChunk(audio->medium);
    Mix_FreeChunk(audio->low);
    
    Mix_FreeMusic(audio->music);
}

AssetManager::AssetManager()
{
    Logger::Log("Created an AssetManager!");
}

AssetManager::~AssetManager()
{
    ClearAssets();
    Logger::Log("Destroyed an AssetManager");
}

//TODO:Clear everything
void AssetManager::ClearAssets()
{
    for(auto texture: textures)
    {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

//TODO:Add null checks here
void AssetManager::AddTexture(SDL_Renderer *renderer, const std::string& assetID, const std::string& filePath)
{
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if(surface == nullptr)
    {
        Logger::Err("AssetManager could not find " + filePath);
        SDL_FreeSurface(surface);
    }
    else
    {
        
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if(texture == nullptr)
        {
            Logger::Err("AssetManger could not create texture");
            SDL_FreeSurface(surface);
        }
        else
        {
            SDL_FreeSurface(surface);
        
            textures.emplace(assetID, texture);
            Logger::Log("AssetManger added " + filePath + " with ID: " + assetID);
        }
    }
}

SDL_Texture * AssetManager::GetTexture(const std::string& assetID)
{
    return textures[assetID];
}
