#pragma once
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

#include <ECS/ECS.hpp>
#include <Base/AssetManager.hpp>
#include <Base/EventBus.hpp>

#define FPS 60
#define MILLI_PER_FRAME 1000 / FPS

class Game
{
    public:
        Game();
        ~Game();
        
        void Initialize();
        void Run();
        void Destroy();
        
        void ProcessInput();
        void Update();
        void Render();
        
        void Setup();
        void LoadScene(int sceneIndex);
        
        int wWidth, wHeight;
        
        //For later
        //bool vsync = false;
        
    private:
        std::unique_ptr<World> world;
        std::unique_ptr<AssetManager> assetManager;
        std::unique_ptr<EventBus> eventBus;
        
        bool m_isRunning = true;
        SDL_Window *m_window = nullptr;
        SDL_Renderer *m_renderer = nullptr;
        int millisecsPreviousFrame = 0;
        bool debugMode = false;//TODO:Make a debug component, or put a debug variable inside components
};
