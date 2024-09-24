// cRPG.cpp : Defines the entry point for the application.
//

#include "cRPG.h"
#include "Game.h"
#include "Logger.h"

using namespace std;

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
