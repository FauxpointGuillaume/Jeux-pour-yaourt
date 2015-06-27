#pragma once

#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Sprite.h"

class Bouton
{
public:
	Bouton(Sprite* passed_sprite, int passed_bouton_action);
	~Bouton();

	Sprite* bouton_getsprite();
	void bouton_setsprite(Sprite* passed_bouton_sprite);

	bool bouton_getmouseover();
	void bouton_setmouseover(bool passed_bouton_mouseover);

	bool bouton_getclick();
	void bouton_setclick(bool passed_bouton_click);

	bool bouton_getclicked();
	void bouton_setclicked(bool passed_bouton_clicked);

	int bouton_getaction();
	void bouton_setaction(int passed_bouton_action);



private:
	Sprite* bouton_sprite;
	bool bouton_mouseover = false;
	bool bouton_click = false;
	bool bouton_clicked = false;
	int bouton_action = 0;
};

