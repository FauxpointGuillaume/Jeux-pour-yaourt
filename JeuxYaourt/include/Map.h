#pragma once

#include <vector>
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include "Sprite.h"



class Map
{
public:
	//Initializes position and type
	Map(SDL_Renderer* passed_renderer, std::string mapfile, float *passed_CameraX, float *passed_CameraY);
	~Map();

	void setmapx();
	void setmapy();

	int getsizex();
	int getsizey();

	
	void Draw();

	//Shows the tile
	void render(SDL_Rect& camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

	void cleartiles();

	std::vector<Sprite*> getTiles();

	std::string rocher = "bang26.jpg";
	std::string herbe = "bang31.jpg";

	const int TILE_WIDTH = 80;
	const int TILE_HEIGHT = 80;
	const int TOTAL_TILES = 192;
	const int TOTAL_TILE_SPRITES = 12;


	const int TILE_RED = 0;
	const int TILE_GREEN = 1;
	const int TILE_BLUE = 2;
	const int TILE_CENTER = 3;
	const int TILE_TOP = 4;
	const int TILE_TOPRIGHT = 5;
	const int TILE_RIGHT = 6;
	const int TILE_BOTTOMRIGHT = 7;
	const int TILE_BOTTOM = 8;
	const int TILE_BOTTOMLEFT = 9;
	const int TILE_LEFT = 10;
	const int TILE_TOPLEFT = 11;



	std::vector<SDL_Rect> obstacles;

private:
	float *CameraX;
	float *CameraY;

	int sizex = 0;
	int sizey = 0;

	std::vector < Sprite* > tiles;
	std::vector<int> mapx;
	std::vector<int> mapy;

	std::ifstream mapp;


	Sprite *tile;
	SDL_Renderer* renderer;

};

