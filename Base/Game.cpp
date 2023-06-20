#include <iostream>
#include <fstream>
#include <Base/Game.hpp>
#include <Logger/Logger.hpp>

//Components
#include <ECS/Components/TransformComponent.hpp>
#include <ECS/Components/RigidBodyComponent.hpp>
#include <ECS/Components/SpriteComponent.hpp>
#include <ECS/Components/AnimationComponent.hpp>
#include <ECS/Components/BoxColliderComponent.hpp>

//Systems
#include <ECS/Systems/MoveSystem.hpp>
#include <ECS/Systems/RenderSystem.hpp>
#include <ECS/Systems/AnimationSystem.hpp>
#include <ECS/Systems/CollisionSystem.hpp>
#include <ECS/Systems/DebugRenderSystem.hpp>
#include <ECS/Systems/DamageSystem.hpp>
#include <ECS/Systems/KeyboardControlSystem.hpp>

//TODO:Set up renderer with a virtual resolution
//TODO:Make sprite sheets work for tilemaps and animations

Game::Game()
{
    //Loger
    Logger::Log("Constructor called");
    assetManager = std::make_unique<AssetManager>();
    eventBus = std::make_unique<EventBus>();
    world = std::make_unique<World>();
}

Game::~Game()
{
    Logger::Log("Destructor called");
}

void Game::Initialize()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::string sdlError = SDL_GetError();
        std::cerr << "Error initializing SDL! SDL ERROR:" << sdlError << std::endl;
        Logger::Err("Error initializing SDL! SDL ERROR: " + sdlError);
        m_isRunning = false;
        return;
    }
    
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    wWidth = 800;
    wHeight = 600;
    
    m_window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wWidth, wHeight, SDL_WINDOW_BORDERLESS);
    if(m_window == nullptr)
    {
        std::string sdlError = SDL_GetError();
        std::cerr << "Could not create window! SDL ERROR:" << sdlError << std::endl;
        Logger::Err("Could not create window! SDL ERROR: " + sdlError);
        m_isRunning = false;
        return;
    }
    
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if(m_renderer == nullptr)
    {
        std::string sdlError = SDL_GetError();
        std::cerr << "Could not create renderer! SDL ERROR:" << sdlError << std::endl;
        Logger::Err("Could not create renderer! SDL ERROR: " + sdlError);
        m_isRunning = false;
        return;
    }
    
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        std::string imgError = IMG_GetError();
        std::cerr << "SDL_image could not initialize! SDL ERROR:" << imgError << std::endl;
        Logger::Err("SDL_image could not initialize! SDL ERROR: " + imgError);
    }
    Logger::Log("Initialized SDL_image");
    
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    {
        std::string mixError = Mix_GetError();
        std::cerr << "SDL_mixer could not initialize! SDL_mixer ERROR:" << mixError << std::endl;
        Logger::Err("SDL_mixer could not initialize! SDL_mixer ERROR: " + mixError);
    }
    Logger::Log("Initialized SDL_mixer");
    
    //For later
    //SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

void Game::ProcessInput()
{
    
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE)
                {
                    m_isRunning = false;
                }
                break;
        }
    }
}

void Game::Setup()
{
    LoadScene(1);
}


void Game::LoadScene(int sceneIndex)
{
    //Add assets
    assetManager->AddTexture(m_renderer, "tank-image", "../assets/images/tank-panther-right.png");
    assetManager->AddTexture(m_renderer, "truck-image", "../assets/images/truck-ford-right.png");
    assetManager->AddTexture(m_renderer, "chopper-image", "../assets/images/chopper.png");
    assetManager->AddTexture(m_renderer, "chopper-sheet", "../assets/images/chopper-spritesheet.png");
    assetManager->AddTexture(m_renderer, "radar-image", "../assets/images/radar.png");
    assetManager->AddTexture(m_renderer, "jungle-tileset", "../assets/tilemaps/jungle.png");
    
    //Add systems
    world->AddSystem<MovementSystem>();
    world->AddSystem<RenderSystem>();
    world->AddSystem<AnimationSystem>();
    world->AddSystem<CollisionSystem>();
    world->AddSystem<DebugRenderSystem>();
    world->AddSystem<DamageSystem>();
    world->AddSystem<KeyboardControlSystem>();
    
    //TODO:Change this horrible thing for a better way of loading maps
    // Load the tilemap
    int tileSize = 32;
    double tileScale = 2.0;
    int mapNumCols = 25;
    int mapNumRows = 20;

    std::fstream mapFile;
    mapFile.open("../assets/tilemaps/jungle.map");
    
    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) 
        {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = world->CreateEntity();
            tile.AddComponent<Transform_C>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<Sprite_C>("jungle-tileset", tileSize, tileSize, 0, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    
    //Create entities
    Entity tank = world->CreateEntity();
    Entity truck = world->CreateEntity();
    Entity chopper = world->CreateEntity();
    Entity radar = world->CreateEntity();
    
    //Add components
    tank.AddComponent<Transform_C>(glm::vec2(32.0, 32.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBody_C>(glm::vec2(50.0, 0.0));
    tank.AddComponent<Sprite_C>("tank-image", 32, 32, 1);
    tank.AddComponent<BoxCollider_C>(32, 32, glm::vec2(0));
    
    chopper.AddComponent<Transform_C>(glm::vec2(50.0, 50.0), glm::vec2(1.0, 1.0), 0.0);
    chopper.AddComponent<RigidBody_C>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<Sprite_C>("chopper-image", 32, 32, 1);
    chopper.AddComponent<Animation_C>(2, 10, true);
    
    radar.AddComponent<Transform_C>(glm::vec2(wWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
    radar.AddComponent<RigidBody_C>(glm::vec2(0.0, 0.0));
    radar.AddComponent<Sprite_C>("radar-image", 64, 64, 2);
    radar.AddComponent<Animation_C>(8, 8, true);
    
    truck.AddComponent<Transform_C>(glm::vec2(32.0, 32.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidBody_C>(glm::vec2(35.0, 0.0));
    truck.AddComponent<Sprite_C>("truck-image", 32, 32, 2);
    truck.AddComponent<BoxCollider_C>(32, 32, glm::vec2(0));
}

void Game::Update()
{
    //Delta time
    //See if you can change this for std::chrono
    int timeToWait = MILLI_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    
    //Coment this for uncapped FPS
    if(timeToWait > 0 && timeToWait <= MILLI_PER_FRAME)
    {
        SDL_Delay(timeToWait);
    }
    
    //waste cpu power -> while(!SDL_TICKS_PASSED(SDL_GetTicks(), millisecsPreviousFrame + MILLI_PER_FRAME));
    
    
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    
    millisecsPreviousFrame = SDL_GetTicks();
    
    eventBus->Reset();
    
    //Register events here
    world->GetSystem<DamageSystem>().SubscribeToEvent(eventBus);
    world->GetSystem<KeyboardControlSystem>().SubscribeToEvent(eventBus);
    
    //Run ECS logic here
    world->Update();
    world->GetSystem<MovementSystem>().Update(deltaTime);
    world->GetSystem<CollisionSystem>().Update(eventBus);
    world->GetSystem<AnimationSystem>().Update();
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);
    
    //Render here
    world->GetSystem<RenderSystem>().Render(m_renderer, assetManager);
    world->GetSystem<DebugRenderSystem>().Render(m_renderer, debugMode);
    
    SDL_RenderPresent(m_renderer);
}

void Game::Run()
{
    Setup();
    while(m_isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}
