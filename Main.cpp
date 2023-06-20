#include <iostream>
#include <Base/Game.hpp>

//TODO: port this to c++ 20(especially to modules)

int main()
{
    Game game;
    
    game.Initialize();
    game.Run();
    game.Destroy();
    
    return 0;
}
