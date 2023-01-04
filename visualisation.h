#ifndef __VISUALISATION_H__
#define __VISUALISATION_H__

#include <SDL2/SDL.h>

enum VisualisationType {
	VIS_LINES
};

class Visualisation
{
protected:
	SDL_Renderer *renderer;
	int width;
	int height;
public:
	Visualisation(SDL_Renderer *renderer);
	~Visualisation(){}
	virtual void setSize(const int width, const int height){}
	virtual void tick(){}

	static Visualisation* get(VisualisationType type, SDL_Renderer *renderer);
};

#endif // __VISUALISATION_H__