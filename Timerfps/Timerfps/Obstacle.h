#pragma once
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

class Obstacle
{
public:
	Obstacle(SDL_Rect rect);
	~Obstacle();

	SDL_Rect getpath();
	SDL_Rect getpath2();
	SDL_Rect getobstacle();

	void setobstacle(SDL_Rect rect);
	void setpath(SDL_Rect rect);
	void setpath2(SDL_Rect rect);


private:
	SDL_Rect obstacle;
	SDL_Rect openpath;
	SDL_Rect openpath2;

};

