#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	glm::vec2  velocity = glm::vec2(5.0, -2.0);

	std::cout << "Hello, World!" << std::endl;
	return 0;
}
