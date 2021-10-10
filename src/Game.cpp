#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>

Game::Game()
{
	std::cout << "constructor" << std::endl;
	isRunning = false;
}

Game::~Game()
{
	std::cout << "destructor" << std::endl;
}

void Game::Initialize()
{
	std::cout << "initialize" << std::endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cerr << "Error SDL_Init" << std::endl;
		return;
	}

	window = SDL_CreateWindow("2D Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS);
	if (!window){
		std::cerr << "Error SDL_CreateWindow" << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer){
		std::cerr << "Error SDL_CreateRenderer" << std::endl;
		return;
	}

	isRunning = true;
}

void Game::Run()
{
	std::cout << "run" << std::endl;
	while(isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput()
{
	std::cout << "processInput" << std::endl;
	SDL_Event sdlEvent;
	while(SDL_PollEvent(&sdlEvent)){
		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE){
				isRunning = false;
			}
			break;
		}
	}
}

void Game::Update()
{
	std::cout << "update" << std::endl;
}

void Game::Render()
{
	std::cout << "render" << std::endl;
}

void Game::Destroy()
{
	std::cout << "destroy" << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}