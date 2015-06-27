#include "stdafx.h"
#include "Obstacle.h"


Obstacle::Obstacle(SDL_Rect rect)
{
	setpath(rect);
	setpath2(rect);
	setobstacle(rect);
}


Obstacle::~Obstacle()
{
}

void Obstacle::setobstacle(SDL_Rect rect)
{
	obstacle = rect;
}


void Obstacle::setpath(SDL_Rect rect)
{
	//Set blending function
	if (rect.w <= rect.h)
	{
		openpath.x = rect.x;
		openpath.y = rect.y - 60;
		openpath.w = 60;
		openpath.h = 60;
	}else
	{
		openpath.x = rect.x - 60;
		openpath.y = rect.y;
		openpath.w = 60;
		openpath.h = 60;
	}

}
void Obstacle::setpath2(SDL_Rect rect)
{
	//Set blending function
	if (rect.w <= rect.h)
	{
		openpath2.x = rect.x;
		openpath2.y = rect.y + rect.h;
		openpath2.w = 60;
		openpath2.h = 60;
	}
	else
	{
		openpath2.x = rect.x + rect.w;
		openpath2.y = rect.y;
		openpath2.w = 60;
		openpath2.h = 60;
	}

}

SDL_Rect Obstacle::getpath()
{
	return openpath;
}

SDL_Rect Obstacle::getpath2()
{
	return openpath2;
}
SDL_Rect Obstacle::getobstacle()
{
	return obstacle;
}