#pragma once

#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>


//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer* renderer ,std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer* renderer, SDL_Surface* textsurface, std::string textureText, SDL_Color textColor);
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
	void render(SDL_Renderer* renderer, int x, int y);

	//Gets image dimensions
	int getWidth();
	int getHeight();


	bool setFont(const char* ttffile, int size);
	
	TTF_Font* getFont();

private:
	//The actual hardware texture
	SDL_Texture* mTexture = NULL;

	//Globally used font
	TTF_Font* gFont = NULL;

	SDL_Rect* clip = NULL;
	double angle = 0.0;
	SDL_Point* center = NULL;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Rect renderQuad;

	//Image dimensions
	int mWidth;
	int mHeight;
};

