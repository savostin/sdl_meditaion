#include "video.h"
#include <iostream>
#include <SDL2/SDL_opengl.h>


Video::Video(VisualisationType type) : visualisation(NULL), window(NULL), renderer(NULL)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("CALM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == NULL)
		std::cout << "WINDOW" << std::endl;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL)
		std::cout << "renderer" << std::endl;
	visualisation = Visualisation::get(type, renderer);
	resized();
}


Video::~Video()
{
	if (visualisation)
		delete visualisation;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Video::tick()
{
	if (visualisation)
		visualisation->tick();
	SDL_RenderPresent(renderer);	
}

void Video::resized()
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	if (visualisation)
		visualisation->setSize(width, height);
}

void Video::run()
{
	SDL_Event e;
	bool quit = false;
	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					resized();
				}
				break;
			}
		}
		tick();
		SDL_Delay(100);
	}
}

