#include "lines.h"

#define PI 3.14159265358979323846
#define numPoints 6
#define maxTicks 3000
#define tickSpeed 5

using namespace std;

Lines::Lines(SDL_Renderer *_renderer) : Visualisation(_renderer), ticks(0), add(1)
{
srand(time(NULL));
for (int i = 0; i < numPoints; i++) 
	points.push_back(new Point());
}

Lines::~Lines()
{
	for (int i = 0; i < points.size(); i++)
	{
		delete points[i];
	}
}

void Lines::setSize(const int _width, const int _height)
{
	width = _width;
	height = _height;
	reset();
}

void Lines::tick()
{
	Point *p;
	if (add > 0)
	{
		if (ticks <= maxTicks)
		{
			for (int n = 0; n < tickSpeed; n++)
			{
				for (int i = 0; i < points.size(); i++)
				{
					p = points[i];
					p->update(width, height);
					SDL_SetRenderDrawColor(renderer, p->color.r, p->color.g, p->color.b, p->color.a);
					SDL_RenderDrawLine(renderer, (int)p->x, (int)p->y, (int)p->neighbor->x, (int)p->neighbor->y);
				}
				ticks += add;
			}
		}
		else if (ticks <= maxTicks * 1.2)
		{
			ticks += add;
		}
		else
		{
			add = -10;
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		}
	}
	if (add < 0)
	{
		if (ticks > 0)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
			SDL_RenderFillRect(renderer, NULL);
			ticks += add;
		}
		else
		{
			add = 1;
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
			reset();
		}
	}
}

void Lines::reset()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
	ticks = 0;
	add = 1;
	Point::base = rand() % 360;
	int j;
	int psize = points.size();
	for (int i = 0; i < psize; i++)
	{
		points[i]->init(width, height);
		j = i;
		while (j == i)
			j = floor(rand() % psize);
		points[i]->neighbor = points[j];
	}
}

RGBA Point::HSL2RGB(HSLA in)
{
	double hh, p, q, t, ff;
	long i;
	RGBA out;
	in.s /= 100.;
	in.l /= 100.;

	if (in.s <= 0.0)
	{
		out.r = in.l;
		out.g = in.l;
		out.b = in.l;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0)
		hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.l * (1.0 - in.s);
	q = in.l * (1.0 - (in.s * ff));
	t = in.l * (1.0 - (in.s * (1.0 - ff)));

	switch (i)
	{
	case 0:
		out.r = in.l;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.l;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.l;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.l;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.l;
		break;
	case 5:
	default:
		out.r = in.l;
		out.g = p;
		out.b = q;
		break;
	}
	out.a = in.a;
	out.r *= 255;
	out.g *= 255;
	out.b *= 255;
	return out;
}

int Point::base = 180;

Point::Point() : x(0), y(0), dx(0), dy(0)
{
}

void Point::init(const int width, const int height)
{
	x = rand() % width;
	y = rand() % height;
	float a = PI * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	dx = cos(a);
	dy = sin(a);
	HSLA c;
	c.h = rand() % 360;
	c.s = 100;
	c.l = 70;
	c.a = 10;
	color = Point::HSL2RGB(c);
}

Point::~Point()
{
}

void Point::update(const int width, const int height)
{
	x += dx;
	y += dy;
	if (x <= 0 || x >= width)
		dx = -dx;
	if (y <= 0 || y >= height)
		dy = -dy;
}
