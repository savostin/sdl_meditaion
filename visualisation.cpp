#include "visualisation.h"

#include "visualisations/lines.h"

Visualisation* Visualisation::get(VisualisationType type, SDL_Renderer *renderer)
{
	switch(type)
	{
		case VIS_LINES:
			return new Lines(renderer);
	}
	throw "UNKNOW VIS TYPE";
}

Visualisation::Visualisation(SDL_Renderer *_renderer) : renderer(_renderer), width(1), height(1)
{
	
}
/*
void Visualisation::setSize(const int _width, const int _height)
{
	width = _width;
	height = _height;
}
*/