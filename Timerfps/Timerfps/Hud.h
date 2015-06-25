#pragma once

#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Unit.h"
#include "Sprite.h"
#include "LTexture.h"

class Hud
{
public:
	Hud(SDL_Renderer* passed_renderer, Sprite* hud, Unit* unit, Sprite* passed_minimap);
	~Hud();
	
	void setunit(Unit* unit);
	Unit* gethudunit();

	Sprite*	gethudstyle();

	Sprite* getminimap();
	void setminimap(Sprite* passed_minimap);


	void addunit(Unit* passed_unit);
	std::vector<Unit*> gethudunits();
	bool hudunitsempty();
	void hudunitsclear();
	int hudunitssize();

	std::stringstream gettext();

	void drawtext();

	void drawminimap(std::vector<Unit*> units);

	void drawskills(Unit* passed_priority_unit);

	bool skill_mouse_actions(Unit* passed_priority_unit, int mousex, int mousey, bool passed_leftclick, bool passed_leftclick_release, bool passed_shift);

	void draw_skill_text(int skill);

private:
	std::stringstream text;
	std::stringstream text_skill;
	SDL_Renderer *renderer = NULL;
	SDL_Surface *textsurface = NULL;
	SDL_Surface *textsurface_skill = NULL;
	Sprite* hudstyle = NULL;
	Sprite* minimap = NULL;
	Sprite* grenade = NULL;
	Unit* hudunit = NULL;
	std::vector<Unit*> hudunits;
	int hudunitsx;

	//Camera = 0
	float classhudposx = 0;
	float classhudposy = 0;

	//std::stringstream text;
	//std::stringstream Text;

	//In memory text stream
	LTexture hudtext;
	LTexture skill_text_texture;
	SDL_Color skill_text_color;
	SDL_Color textColor;
	SDL_Point point;

	std::string hudtextstring;
	std::string text_skill_string;

	//Skill
	SDL_Rect skill_preselect_rect;
	bool skill_selected = false;
	bool skill_preselect = false;
};

