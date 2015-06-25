



#include "stdafx.h"

#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include "moveunit.h"


SDL_Rect moveunit(int mousexy[], SDL_Rect unit, bool movecube2)
{

	printf("mdrrrr");
	if (movecube2 == true)
	{
		movingcubex2.clear();
		movingcubey2.clear();
		printf("%d", mousexy[0]);
		printf("lololol");
		printf("%d", mousexy[1]);
		printf("posdfji");
		if (mousexy[0] == unit.x && mousexy[1] == unit.y)
		{
			movecube2 = false;
		}
		else
		{
			printf("qsdqsdqsdqsd");
			if (mousexy[0] <= unit.x)
			{
				distancecube2x = unit.x - mousexy[0];
				for (i = 0; i < distancecube2x; ++i)
				{

					movingcubex2.push_back(unit.x - i);
				}
			}
			else if (mousexy[0] > unit.x)
			{
				distancecube2x = mousexy[0] - unit.x;
				for (i = 0; i < distancecube2x; ++i)
				{
					movingcubex2.push_back(unit.x + i);
				}
			}
			if (mousexy[1] <= unit.y)
			{
				distancecube2y = unit.y - mousexy[1];
				for (i = 0; i < distancecube2y; ++i)
				{
					movingcubey2.push_back(unit.y - i);
				}
			}
			else if (mousexy[1] > unit.y)
			{
				distancecube2y = mousexy[1] - unit.y;
				for (i = 0; i < distancecube2y; ++i)
				{
					movingcubey2.push_back(unit.y + i);
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

		if (coefcube2 >= 1)
		{
			if (iteratorcube2xmax < movingcubex2.size())
			{
				++iteratorcube2xmax;
				unit.x = movingcubex2[++iteratorcube2x - distancecube2x - 1];
			}
			if (iteratorcube2ymax < movingcubey2.size() && iteratorcube2ymax*coefcube2 <= iteratorcube2xmax)
			{
				++iteratorcube2ymax;
				unit.y = movingcubey2[++iteratorcube2y - distancecube2y - 1];
			}

		}
		else
		{

			if (iteratorcube2xmax < movingcubex2.size() && iteratorcube2ymax*coefcube2 >= iteratorcube2xmax)
			{
				++iteratorcube2xmax;
				unit.x = movingcubex2[++iteratorcube2x - distancecube2x - 1];
			}
			if (iteratorcube2ymax < movingcubey2.size())
			{
				++iteratorcube2ymax;
				unit.y = movingcubey2[++iteratorcube2y - distancecube2y - 1];
			}
		}

	}
	return unit;
}