#pragma once
#include "StdAfx.h"
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Unit.h"
#include "Sprite.h"


class Skills
{
public:
	Skills(SDL_Renderer* passed_renderer, Unit* passed_unit, Unit* passed_targetunit, int passed_usedskill, float *passed_camerax, float *passed_cameray, int passed_mousex, int passed_mousey);
	~Skills();

	void use_skill();	
	void hand_grenade();

	void setmousexy(int passed_mousexy[]);

	bool getstart_skill();
	
	bool getfinish_skill();

	Sprite* gethand_grenade_sprite();

	Unit* getunit();
	Sprite* getunit_sprite();

	//float getunit_damage();



private:
	SDL_Renderer* renderer;
	float *camerax = 0;
	float *cameray = 0;
	//std::vector<int>bite;
	int mousexy[2];
	Sprite* unit_sprite;
	Unit* targetunit;
	Sprite* hand_grenade_sprite;
	Unit* unit;
	//Unit* unit_lol = new Unit(unit_sprite, "roger", 200, 38, 20, 1, 2, 2, bite, hand_grenade_sprite, 0);

	int used_skill;
	bool start_skill = false;
	bool finish_skill = false;
	//int skill_number = 0;
	bool skill_go = false;
	bool no_reset;
	//std::string unit_name;

	int hand_grenade_timer = 0;
	bool hand_grenade_launched = false;
	int hand_grenade_distancex = 0;
	int hand_grenade_distancey = 0;
	std::vector<double> hand_grenade_movingx;
	std::vector<double> hande_grenade_movingy;
	double hand_grenade_coef = 0;
	int hand_grenade_iteratorx = 0;
	int hand_grenade_iteratory = 0;
	int hand_grenade_iteratormaxx = 0;
	int hand_grenade_iteratormaxy = 0;
	bool hand_grenade_finish = false;
	int hand_grenade_speed = 1;


	int i = 0;
};

