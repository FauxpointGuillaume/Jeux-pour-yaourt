#pragma once
#include "stdafx.h"

#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>



bool movecube2 = false;
bool moved = false;
bool rightclick = false;
//int movingcube2[1];
std::vector<double> movingcubex2;
std::vector<double> movingcubey2;
int distancecube2x;
int distancecube2y;
double coefcube2;
int differencecube2;
int i;
int iteratorcube2x = 0;
int iteratorcube2y = 0;
int iteratorcube2xmax = 0;
int iteratorcube2ymax = 0;
int checkcube2x = 0;
int checkcube2y = 0;


SDL_Rect moveunit(int mousexy[], SDL_Rect unit, bool movecube2);
