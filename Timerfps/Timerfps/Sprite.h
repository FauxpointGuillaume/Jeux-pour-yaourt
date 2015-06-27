#pragma once

/* Fichier responsables de la gestion des sprites
	- Chargement de l'image associée dans le DD ou une surface
	- Configure l'animation et permet de la lancer en fonction 
		paramètres donnée
	*/


#include "StdAfx.h"
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "LTexture.h"

extern int countedFrames;

class Sprite
{
public:
	// Constructor
	Sprite();
	Sprite(SDL_Renderer* passed_renderer, std::string FilePath, int x, int y, int w, int h, float *passed_CameraX, float *passed_CameraY);
	Sprite(SDL_Renderer* passed_renderer,SDL_Surface* surface, int x, int y, int w, int h, float *passed_CameraX, float *passed_CameraY);

	// Destructor
	~Sprite(void);

	// Fonction de dessin (dessine les sprites 
	void Draw();

	// Setter/getter de la position de la sprite (Utile pour la faire bouger)
	void SetX(float X);
	void SetY(float Y);
	void SetPosition(float X, float Y);
	float GetX();
	float GetY();

	// Setter/getter des dimensions des images
	void SetWidth(int W);
	void SetHeight(int H);
	int GetWidth();
	int GetHeight();

	// Setter/getter de l'angle du sprite
	int GetAngle();
	void SetAngle(int passed_angle);

	// Type de rotation choisie (Horizontal et/ou vertical)
	void setfliptype(SDL_RendererFlip passed_fliptype);
	SDL_RendererFlip getfliptype();

	// Fonction pour faire une animation (Chargé la suite des images pour faire
	// une animation)
	void SetOrgin(float X, float Y);
	void PlayAnimation(int BeginFrame, int EndFrame, int Row, float Speed,bool allrow);
	void SetUpAnimation(int passed_Amount_X, int passed_Amount_Y);

private:
	// Variable de la camera
	float *CameraX;
	float *CameraY;

	// Position d'origine de la sprite
	float Orgin_X;
	float Orgin_Y;

	// Position de la sprite
	float X_pos;
	float Y_pos;

	// Parametre de stockage de l'image sur la CG
	SDL_Surface* surfaceimage = NULL;
	SDL_Texture* image = NULL;

	// Position sur l'ecran
	SDL_Rect rect;

	// Portion de l'image
	SDL_Rect crop;

	// Parametre de l'image
	int img_width;
	int img_height;

	// Frame actuel sur l'image
	int CurrentFrame;

	// Delay des animations
	int animationDelay;
	int allrow_row = 0;

	// Nombre de frame sur une ligne et sur une collonne
	int Amount_Frame_X;
	int Amount_Frame_Y;

	// Angle de la sprite
	int angle = 0;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	// Position 
	SDL_Rect Position;
	SDL_Renderer* renderer = NULL;
};



