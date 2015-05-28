#pragma once

#include "StdAfx.h"
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "LTexture.h"

#pragma once
class Sprite
{
public:
	Sprite();
	Sprite(SDL_Renderer* passed_renderer, std::string FilePath, int x, int y, int w, int h, float *passed_CameraX, float *passed_CameraY);

	~Sprite(void);

	void DrawSteady();
	void Draw();

	void SetX(float X);
	void SetY(float Y);
	void SetPosition(float X, float Y);

	float GetX();
	float GetY();

	void SetWidth(int W);
	void SetHeight(int H);
	int GetWidth();
	int GetHeight();

	int GetAngle();
	void SetAngle(int passed_angle);

	void SetRect(SDL_Rect rec);
	SDL_Rect GetRect();

	void SetPosition(SDL_Rect rec);
	SDL_Rect GetPosition();

	void setfliptype(SDL_RendererFlip passed_fliptype);
	SDL_RendererFlip getfliptype();

	SDL_Rect Position;
	void SetOrgin(float X, float Y);
	void PlayAnimation(int BeginFrame, int EndFrame, int Row, float Speed);
	void SetUpAnimation(int passed_Amount_X, int passed_Amount_Y);

	SDL_Surface* getsurface();

private:
	float *CameraX;
	float *CameraY;

	float Orgin_X;
	float Orgin_Y;

	float X_pos;
	float Y_pos;

	SDL_Rect rect;

	SDL_Surface* surfaceimage = NULL;
	SDL_Texture* image = NULL;
	SDL_Rect crop;

	int img_width;
	int img_height;
	int CurrentFrame;
	int animationDelay;

	int Amount_Frame_X;
	int Amount_Frame_Y;

	int angle = 0;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	SDL_Renderer* renderer;
};



