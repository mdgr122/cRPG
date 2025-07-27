#include "Game.h"
#include "Logger.h"


int main()
{
    Logger logger;
    //logger.Log("Testing");

    // Creating the Game object called game.
    Game game;

    // Calling the public member function Run();
    game.Run();
    return 0;
}
