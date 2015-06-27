#include "stdafx.h"
#include "camera.h"
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>


//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAMES_PER_SECOND = 20;

//The dot dimensions
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

//The dimensions of the level
const int LEVEL_WIDTH = 1024;
const int LEVEL_HEIGHT = 768;

//The surfaces
SDL_Surface *dot = SDL_LoadBMP("bombe1.bmp");
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Texture * dottex = SDL_CreateTextureFromSurface(gRenderer,dot);
//The event structure
SDL_Event event;

//The camera
SDL_Rect cam = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };




camera::camera()
{
}


camera::~camera()
{
}



void camera::move()
{
	//Move the dot left or right
	x += xVel;

	//If the dot went too far to the left or right
	if ((x < 0) || (x + DOT_WIDTH > LEVEL_WIDTH))
	{
		//move back
		x -= xVel;
	}

	//Move the dot up or down
	y += yVel;

	//If the dot went too far up or down
	if ((y < 0) || (y + DOT_HEIGHT > LEVEL_HEIGHT))
	{
		//move back
		y -= yVel;
	}
}


void camera::set_camera()
{
	//Center the camera over the dot
	cam.x = (x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	cam.y = (y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds.
	if (cam.x < 0)
	{
		cam.x = 0;
	}
	if (cam.y < 0)
	{
		cam.y = 0;
	}
	if (cam.x > LEVEL_WIDTH - cam.w)
	{
		cam.x = LEVEL_WIDTH - cam.w;
	}
	if (cam.y > LEVEL_HEIGHT - cam.h)
	{
		cam.y = LEVEL_HEIGHT - cam.h;
	}

}


void camera::show()
{
	//Show the dot

	SDL_RenderCopy(gRenderer, dottex, &cam, &cam);
}




SDL_Rect camera::getcam()
{
	return cam;
}
void camera::setcam(SDL_Rect rect)
{
	cam = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}