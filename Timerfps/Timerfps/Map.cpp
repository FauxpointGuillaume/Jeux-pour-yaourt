#include "stdafx.h"
#include "map.h"


Map::Map(SDL_Renderer* passed_renderer, std::string mapfile, float *passed_CameraX, float *passed_CameraY)
{
	bool obstaclebool = false;
	bool obstacleligne = true;
	bool obstacleencours = false;
	bool moncul = true;
	SDL_Rect obstacle;
	int xendline= 0;

	int x = 0;
	int y = 0;
	CameraX = passed_CameraX;
	CameraY = passed_CameraY;

	renderer = passed_renderer;

	std::ifstream mapp(mapfile);

	std::string fichiersprite;
	std::string unused;
	tiles.clear();
	obstacles.clear();

	char current_char;
	int c = 0;
	int mod = 0;
	int lignes;
	int colonnes = 0;
	int nbcolonnes = 0;


	int obstx;
	int obsty;
	int obstw;
	int obsth;


	if (mapp.is_open())
	{

		while (mapp.get(current_char)){

			if (current_char == EOF)
			{
				break;
			}
			++mod;
			if (mod % 3 == 1)
			{
				++c;
			}
		}
		
		mapp.clear();
		mapp.seekg(0);
		//while (std::getline(mapp, unused))
//			++lignes;

	}
	mapp.clear();
	mapp.seekg(0);
	printf("%d", c);
	printf("lol");
	printf("%d", c);
	printf("lol");
	printf("%d", c);
	printf("lol");
	//Determines what kind of tile will be made
	int tileType = 0;
	if (mapp)
	{
		printf("Able to load map file!\n");
	}

	
	for (int i = 0; i < c; ++i)
	{
		
		SDL_Delay(1);
		//Read tile from map file
		mapp >> tileType;
		//If the was a problem in reading the map
		if (mapp.fail())
		{
			//Stop loading map
			printf("Error loading map: Unexpected end of file!\n");
			break;
		}
		if (tileType == 99)
		{
			xendline = x;
			sizex = x;
			y += TILE_HEIGHT;
			x = 0;
			obstacleligne = false;
			nbcolonnes = colonnes;
			colonnes = 0;

		}
		else if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
		{
			++colonnes;
			switch (tileType)
			{
			case 00:
				fichiersprite = "grass_tile2.png";
				break;
			case 01:
				fichiersprite = "wall_tile.bmp";
				break;
			default :
				fichiersprite = "grass_tile2.png";
				break;
			}

			tile = new Sprite(renderer, fichiersprite.c_str(), x, y, TILE_WIDTH, TILE_HEIGHT, CameraX, CameraY);
			tiles.push_back(tile);
			/*
			if (obstacleligne == false && obstacleencours == true)
			{

				printf("\n\nweshsisistavu\n\n");
				obstacle.w = xendline - obstacle.x;
				obstacles.push_back(obstacle);
				obstacleligne = true;
				obstacleencours = false;
				obstaclebool = false;
			}
			else if (obstacleligne == false)
			{
				obstacleligne = true;
			}
			printf("OMGGGGGGGGGGGGGGGGGGGGGGGGGGGGdfgdfgGGGGGGGGGGGGGGGGGGGGGGGGGG\n");
			printf("%s", obstacleencours ? "true" : "false");
			printf("OMGGGGGGGGGGGGGGGGGGGGGGGGGGGGdfgdfgGGGGGGGGGGGGGGGGGGGGGGGGGG\n");
			
			if (tileType >= 5 && tileType < 10)
			{
				obstaclebool = true;
			}
			else
			{
				obstaclebool = false;
			}

			if (obstaclebool == true && obstacleligne == true && obstacleencours == false)
			{
				obstaclebool = true;
				obstacleencours = true;
				obstacle.x = x;

				printf("\nOMGGGGGGGGGGGGGGGGGGGGGGGGGGGGdfgdfgGGGGGGGGGGGGGGGGGGGGGGGGGG\n");
				printf("%d", obstacle.x);
				printf("OMGGGGGGGGGGGGGGGGGGGGGGGGGGGGdfgdfgGGGGGGGGGGGGGGGGGGGGGGGGGG\n");

				obstacle.y = y;
				obstacle.h = TILE_HEIGHT;
				obstacleencours = true;
			}
			if (obstaclebool == false && obstacleencours == true)
			{
				printf("\nwtfffffffffffffffffffffffffffffffffffGGGG\n");
				printf("%d", obstacle.x);
				printf("OwtfffffffffffffffffffffffffffffffffffffffffffffGGGGGGG\n");

				obstacle.w = x  - obstacle.x;
				obstacles.push_back(obstacle);
				obstacleencours = false;
				obstaclebool = false;


			}
			else
			{
				printf("lolololololo");
			}*/



			x += TILE_WIDTH;



		}
		//printf("blebuelebuleub");
		//printf("\n\n\n");
		//printf("%d", nbcolonnes);
		//printf("\n\n\n");

	}/*
	for (auto &obs : obstacles) // access by reference to avoid copying
	{

		printf(" x == %d  ", obs.x);
		printf(" y == %d  ", obs.y);
		printf(" h == %d ", obs.h);
		printf("w ==  %d ", obs.w);
		printf("OMGGGGGGGG\n");
		printf("OMGGGGGGGG\n");
	}*/
	sizey = nbcolonnes*TILE_HEIGHT + TILE_HEIGHT + TILE_HEIGHT;
}


Map::~Map()
{
}


std::vector<Sprite*> Map::getTiles()
{
	return tiles;
}

void Map::cleartiles()
{
	tiles.clear();
}


void Map::Draw()
{

}

int Map::getsizex()
{
	return sizex;
}

int Map::getsizey()
{
	return sizey;
}