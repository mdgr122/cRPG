#include "Game.h"
#include <iostream>

bool Game::Init()
{
	return true;
}

void Game::ProcessInputs()
{
	std::cout << "ProcessInputs()" << std::endl;
}

void Game::Update()
{
	std::cout << "Update()" << std::endl;
}

void Game::Draw()
{
	std::cout << "Draw()" << std::endl;
}


Game::Game() : is_running_{true}
{

}

Game::~Game()
{

}

void Game::Run()
{
	if (!Init())
		is_running_ = false;
	
	while (is_running_) 
	{
		ProcessInputs();
		Update();
		Draw();
	}
	std::cout << "Exiting Program" << std::endl;
}

