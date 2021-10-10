#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

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

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS);
	if (!window){
		std::cerr << "Error SDL_CreateWindow" << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer){
		std::cerr << "Error SDL_CreateRenderer" << std::endl;
		return;
	}

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}

void Game::Run()
{
	std::cout << "run" << std::endl;

	Setup();

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

void Game::Setup(){
	playerPosition = glm::vec2(10.0, 20.0);
	playerVelocity = glm::vec2(0.5, 0.0);
}

void Game::Update()
{
	std::cout << "update" << std::endl;
	playerPosition.x += playerVelocity.x;
	playerPosition.y += playerVelocity.y;
}

void Game::Render()
{
	std::cout << "render" << std::endl;
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	//draw a texture
	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect dstRect = {
		static_cast<int>(playerPosition.x),
		static_cast<int>(playerPosition.y),
		32,
		32
	};
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	std::cout << "destroy" << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}