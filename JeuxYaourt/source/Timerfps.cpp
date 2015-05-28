// Timerfps.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

/*
#pragma once
//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include "moveunit.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


SDL_Rect rectToDraw = { 100, 100, 100, 100 };
SDL_Surface *ecran = NULL, *rectangle = NULL, *rectangle2 = NULL, *hud = NULL;
SDL_Texture *texture = NULL, *texture2 = NULL, *hudtex = NULL;



//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//The application time based timer
class LTimer
{
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

//Scene textures
LTexture gFPSTextTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}

class Player
{

public:
	//Initializes variables
	Player();

	//Deallocates memory
	~Player();

	void drawplayer();

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;


};
void input_handle(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;
		}
	}
}

void UpdateEvents(SDL_Surface* ecran)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_RESIZED:
			break;
		}
	}
}

// -------------- CLASS INPUT

class Input
{
protected:
	int mousex, mousey;
	int mousexrel, mouseyrel;
	bool key[SDL_NUM_SCANCODES];
	char mousebuttons[8];
	char quit;
public:
	Input();
	~Input(){}
	void Update();
	bool Key(bool key){ return key; }
	inline int MouseX(){ return mousex; }
	inline int MouseY(){ return mousey; }
	inline int MouseXrel(){ return mousexrel; }
	inline int MouseYrel(){ return mouseyrel; }
	inline int MouseButton(int i){ return mousebuttons[i]; }
	inline int Quit(){ return quit; }

	bool getTouche(const SDL_Scancode touche) const;
};

Input::Input()
{
	SDL_memset(this, 0, sizeof(*this));
	for (int i(0); i < SDL_NUM_SCANCODES; i++)
		key[i] = false;

}

void Input::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			key[event.key.keysym.scancode] = true;
			break;
		case SDL_KEYUP:
			key[event.key.keysym.scancode] = false;
			break;
		case SDL_MOUSEMOTION:
			mousex = event.motion.x;
			mousey = event.motion.y;
			mousexrel = event.motion.xrel;
			mouseyrel = event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mousebuttons[event.button.button] = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button && event.button.button)
				mousebuttons[event.button.button] = 0;
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_WINDOWEVENT_RESIZED:
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		default:
			break;
		}
	}
}

bool Input::getTouche(const SDL_Scancode touche) const
{
	return key[touche];
}


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		ecran = SDL_GetWindowSurface(gWindow);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gFPSTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	
	Input input;

	rectangle = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 10, 10, 10);
	rectangle2 = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 10, 10, 10);
	SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 100, 200, 100));
	SDL_FillRect(rectangle2, NULL, SDL_MapRGB(rectangle2->format, 200, 200, 100));
	SDL_Rect SrcR;
	SDL_Rect DestR;
	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 160;
	SrcR.h = 160;

	DestR.x = 640 / 2 - 16 / 2;
	DestR.y = 480 / 2 - 16 / 2;
	DestR.w = 160;
	DestR.h = 160;

	SDL_Rect SrcR2;
	SDL_Rect DestR2;
	SrcR2.x = 0;
	SrcR2.y = 0;
	SrcR2.w = 160;
	SrcR2.h = 160;

	DestR2.x = 640 / 2 - 16 / 2;
	DestR2.y = 480 / 2 - 16 / 2;
	DestR2.w = 160;
	DestR2.h = 160;
	bool clickcube = false;
	bool clickcube2 = false;
	bool movecube2 = false;
	int mousexy[1];
	double coefcube2 = 0;
	bool rightclick = false;
	/*
	bool moved = false;
	int movingcube2[1];
	std::vector<double> movingcubex2;
	std::vector<double> movingcubey2;
	int distancecube2x;
	int distancecube2y;
	int differencecube2;
	int i;
	int iteratorcube2x = 0;
	int iteratorcube2y = 0;
	int iteratorcube2xmax = 0;
	int iteratorcube2ymax = 0;
	int checkcube2x = 0;
	int checkcube2y = 0;

	*//*

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 20, 255 };

			//The frames per second timer
			LTimer fpsTimer;

			//The frames per second cap timer
			LTimer capTimer;

			//In memory text stream
			std::stringstream timeText;

			//Start counting frames per second
			int countedFrames = 0;
			fpsTimer.start();

			hud = SDL_LoadBMP("bombe1.bmp");
			hudtex = SDL_CreateTextureFromSurface(gRenderer, hud);
			SDL_Rect lol;
			lol.x = 0;
			lol.y = 0;
			lol.w = 640;
			lol.h = 480;
			//While application is running
			while (!quit)
			{
				//Start cap timer
				capTimer.start();


				input.Update();

				
					if (input.MouseButton(SDL_BUTTON_LEFT))
					{
						if (input.MouseX() > DestR.x && input.MouseX() < DestR.x + DestR.w && input.MouseY() > DestR.y && input.MouseY() < DestR.y + DestR.h)
						{
							DestR.w = 10;
							clickcube = true;
						}
						if (clickcube == true)
						{
							DestR.y = input.MouseY();
						}
						if (input.MouseX() > DestR2.x && input.MouseX() < DestR2.x + DestR2.w && input.MouseY() > DestR2.y && input.MouseY() < DestR2.y + DestR2.h)
						{
							clickcube2 = true;
						}
						else
						{
							clickcube2 = false;
						}

					}
					if (input.MouseButton(SDL_BUTTON_RIGHT) && rightclick == false)
					{
						rightclick = true;
						clickcube = false;
						if (clickcube2 == true)
						{
							movecube2 = true;
							mousexy[0] = input.MouseX();
							mousexy[1] = input.MouseY();
						}
					}
				
				if (input.MouseButton(SDL_BUTTON_RIGHT) == false)
				{
					rightclick = false;
				}

				
				if (input.getTouche(SDL_SCANCODE_K))
				{
					DestR.x = 2;
					printf("%s", input.getTouche(SDL_SCANCODE_SPACE) ? "true" : "false");
				}
				if (input.getTouche(SDL_SCANCODE_J))
				{
					DestR.x = 2;
					printf("%s", input.getTouche(SDL_SCANCODE_SPACE) ? "true" : "false");
				}
				DestR.x += 1;
				if (DestR.x > 400)
				{
					DestR.x = 10;
				}
				texture = SDL_CreateTextureFromSurface(gRenderer, rectangle);
				texture2 = SDL_CreateTextureFromSurface(gRenderer, rectangle2);
				
				//Mouvements

				if (movecube2 == true)
				{/*
					movecube2 = false;
					movingcubex2.clear();
					movingcubey2.clear();
					printf("mdrrrr");
					printf("%d", movingcube2[0]);
					printf("lololol");
					printf("%d", movingcube2[1]);
					printf("posdfji");
					if (movingcube2[0] == DestR2.x && movingcube2[1] == DestR2.y)
					{
						movecube2 = false;
					}
					else
					{
						printf("qsdqsdqsdqsd");
						if (movingcube2[0] <= DestR2.x)
						{
							distancecube2x = DestR2.x - movingcube2[0];
							for (i = 0; i < distancecube2x; ++i)
							{
								movingcubex2.push_back(DestR2.x - i);
							}
						}
						else if (movingcube2[0] > DestR2.x)
						{
							distancecube2x = movingcube2[0] - DestR2.x;
							for (i = 0; i < distancecube2x; ++i)
							{
								movingcubex2.push_back(DestR2.x + i);
							}
						}
						if (movingcube2[1] <= DestR2.y)
						{
							distancecube2y = DestR2.y - movingcube2[1];
							for (i = 0; i < distancecube2y; ++i)
							{
								movingcubey2.push_back(DestR2.y - i);
							}
						}
						else if (movingcube2[1] > DestR2.y)
						{
							distancecube2y = movingcube2[1] - DestR2.y;
							for (i = 0; i < distancecube2y; ++i)
							{
								movingcubey2.push_back(DestR2.y + i);
							}
						}
						distancecube2y += distancecube2y == 0;
						coefcube2 = (double)distancecube2x / (double)distancecube2y;
						iteratorcube2x = distancecube2x;
						iteratorcube2y = distancecube2y;
						iteratorcube2xmax = 0;
						iteratorcube2ymax = 0;
						moved = true;
					}
				}
				if (moved == true)
				{

					if (countedFrames % 2 == 1)
					{
						if (coefcube2 >= 1)
						{
							if (iteratorcube2xmax < movingcubex2.size())
							{
								++iteratorcube2xmax;
								DestR2.x = movingcubex2[++iteratorcube2x - distancecube2x - 1];
							}
							if (iteratorcube2ymax < movingcubey2.size() && iteratorcube2ymax*coefcube2 <= iteratorcube2xmax)
							{
								++iteratorcube2ymax;
								DestR2.y = movingcubey2[++iteratorcube2y - distancecube2y - 1];
							}

						}
						else
						{

							if (iteratorcube2xmax < movingcubex2.size() && iteratorcube2ymax*coefcube2 >= iteratorcube2xmax)
							{
								++iteratorcube2xmax;
								DestR2.x = movingcubex2[++iteratorcube2x - distancecube2x - 1];
							}
							if (iteratorcube2ymax < movingcubey2.size())
							{
								++iteratorcube2ymax;
								DestR2.y = movingcubey2[++iteratorcube2y - distancecube2y - 1];
							}
						}

						} *//*
					moveunit(mousexy, DestR2);
				}
				if (clickcube2 == true)
				{
					//SDL_BlitSurface(hud, NULL, ecran, &lol);
				}
				/*
				printf("aaaaaa");
				printf("%d", fmod(coefcube2, (double)iteratorcube2xmax));
				printf("ddddddd", DestR2.x);
				printf("%d", movingcube2[1]);
				printf("%d", DestR2.x);
				printf("<<<<<<", DestR2.x);
				printf("%f", coefcube2);
				printf(">>>>>>", DestR2.x);
				*//*
				//Calculate and correct fps
				float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
				if (avgFPS > 2000000)
				{
					avgFPS = 0;
				}

				//Set text to be rendered
				timeText.str("");
				timeText << "Average Frames Per Second (With Cap) " << avgFPS;

				//Render text
				if (!gFPSTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
				{
					printf("Unable to render FPS texture!\n");
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render textures
				gFPSTextTexture.render((SCREEN_WIDTH - gFPSTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gFPSTextTexture.getHeight()) / 16);
				SDL_RenderDrawRect(gRenderer, &rectToDraw);
				//Update screen

				SDL_RenderCopy(gRenderer, hudtex, &SrcR2, &lol);
				SDL_RenderCopy(gRenderer, texture, &SrcR, &DestR);
				SDL_RenderCopy(gRenderer, texture2, &SrcR2, &DestR2);

				SDL_RenderPresent(gRenderer);
				++countedFrames;

				//If frame finished early
				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICK_PER_FRAME)
				{
					//Wait remaining time
					SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
				}


				
			}
		}
	}
	SDL_DestroyTexture(hudtex);
	//Free resources and close SDL
	close();

	return 0;
}*/