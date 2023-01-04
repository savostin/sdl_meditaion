#ifndef __LINES_H__
#define __LINES_H__

#include <vector>
#include "../visualisation.h"


struct RGBA
{
	float r, g, b, a;
};

struct HSLA
{
	float h, s, l, a;
};


class Point
{
	friend class Lines;
private:
	float x;
	float y;
	float dx;
	float dy;
	RGBA color;
	Point* neighbor;
	static RGBA HSL2RGB(HSLA in);

public:
	Point();
	void init(const int width, const int height);
	~Point();
	void update(const int width, const int height);
	static int base;
};

class Lines : public Visualisation
{
private:
	std::vector<Point*> points;
	int ticks;
	int add;
	void reset();
public:
	Lines(SDL_Renderer *renderer);
	~Lines();
	void setSize(const int width, const int height);
	void tick();
};
#endif // __LINES_H__