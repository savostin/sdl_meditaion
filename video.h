#include <SDL2/SDL.h>
#include "visualisation.h"

using namespace std;



class Video
{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	Visualisation* visualisation;
	void tick();
	void resized();
public:
	Video(VisualisationType type);
	~Video();
	void run();
};