#pragma once
#include "StdAfx.h"
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Sprite.h"


class Unit
{
public:
	Unit();
	Unit(Sprite* passed_sprite, std::string passed_name, float passed_hpmax, float hp, float passed_damage, float passed_attackspeed, float passed_speed, float passed_defense, std::vector<int> passed_skills, Sprite* passed_hudpicture,int passed_friendneutralfoe);
	~Unit(void);

	void setsprite(Sprite* passed_sprite);
	Sprite* getsprite();

	void setname(std::string passed_name);
	std::string getname();

	void sethpmax(float passed_hpmax);
	float gethpmax();

	void sethp(float passed_hp);
	float gethp();

	void setdamage(float passed_damage);
	float getdamage();

	void setattackspeed(float passed_attackspeed);
	int getattackspeed();

	void setspeed(float passed_speed);
	int getspeed();

	void setdefense(float passed_defense);
	int getdefense();

	void setskills(std::vector<int> passed_skills);
	std::vector<int> getskills();
	void skills_pushback(int passed_skill);
	int skills_at(int passed_i);
	int skills_size();

	void sethudpicture(Sprite* passed_hudpicture);
	Sprite* gethudpicture();

	void setselect(bool passed_select);
	bool getselect();

	void setpre_select(bool passed_pre_select);
	bool getpre_select();

	void setmove(bool passed_move);
	bool getmove();

	void setmoving(bool passed_moving);
	bool getmoving();

	void setfriendneutralfoe(int passed_friendneutralfoe);
	int getfriendneutralfoe();

	void setselectedskill(bool passed_selectedskill);
	bool getselectedskill();

	void setusedskill(int passed_usedskill);
	int getusedskill();

	void setanim(bool passed_anim);
	bool getanim();

	void setpriority(bool passed_priority);
	bool getpriority();

	//actions
	void setaction_to_do(int passed_action_to_do);
	int getaction_to_do();

	void action_pushback(int passed_action);
	std::vector<int> action_getlist();
	void action_clearlist();
	int action_size();

	int getcurrent_action();

	//std::vector<std::pair<int, int>> get_mousexylist();
	void mousexylist_clear();
	int mousex_at(int passed_i);
	int mousey_at(int passed_i);

	void mousexy_pushback(int passed_mousex, int passed_mousey);

	//void skills_bool_pushback(int passed_boolint);
	//void skills_bool_clear();
	////int skills_bool_at(int passed_at);
	//int skills_bool_size();
	//std::vector<int> skills_bool_get();
	//void setskill_number(int passed_skill_number);
	//int getskill_number();
	
	//void setnumber_of_skill_used(int passed_number_of_skill_used);
	//int getnumber_of_skill_used();
	//void increment_number_of_skill_used();

	//void setskill_reset(bool passed_skill_reset);
	//bool getskill_reset();

	void setaction_over(int passed_action_over);

	void setreset_action();

	//void setskill_over(bool passed_skill_over);

	void setskill_go(bool passed_skill_go);
	bool getskill_go();

	int getskill_to_go();

	void setaction_start(bool passed_action_start);

	void moveunit(int destinationX,int destinationY);

	void setmousexy(int passed_mousexy[]);
	int getmousex();
	int getmousey();

	void drawlife(SDL_Renderer *renderer,float camerax, float cameray);

	void mouseover(SDL_Renderer *renderer, float camerax, float cameray);

	void draw();

	void choose_animrow(int passed_usedanim);

	void anim_to_use(int anim_to_use);

	void anim_nade();

	void action();

	void setpath_sprite(Sprite* passed_pathsprite);




private:
	//i
	int i = 0;

	//
	//Affichage
	Sprite* sprite;
	SDL_Surface* lifesurface;
	SDL_Rect liferect;
	Sprite* hudpicture;

	//
	//Caractéristiques
	std::string name;
	float hp;
	float hpmax;
	float damage;
	float attackspeed;
	float speed;
	float defense;
	int friendneutralfoe;

	//
	//Sélection
	bool select = false;
	bool pre_select = false;

	//
	//Skills
	std::vector<int> skills;
	bool selectedskill= false;
	int used_skill = -1;
	int skill_to_go = 0;
	bool skill_reset = false;

	//
	//Mouvements
	bool move = false;
	bool moving;
	bool rightclick = false;
	std::vector<double> movingx;
	std::vector<double> movingy;
	int distancecube2x;
	int distancecube2y;
	double coefcube2;
	int differencecube2;
	int iteratorcube2x = 0;
	int iteratorcube2y = 0;
	int iteratorcube2xmax = 0;
	int iteratorcube2ymax = 0;

	//Souris
	int mousexy[2];
	std::vector<std::pair<int, int>> vector_mousexy;

	//
	//Animation grenade
	bool anim_nade_start = false;
	int anim_row = 1;
	int anim_beginframe = 0;
	int anim_endframe = 2;
	int anim_speed = 100;
	bool anim = false;
	//bool nomove_anim = false;
	int animationtime = 0;
	int animationdelay;


	//Setup des skills
	int skill_1 = 1;
	int skill_2 = 2;
	int skill_3 = 3;
	int skill_4 = 4;

	//Lignes de sprites pour animation
	int animrow_stand_1 = 0;
	int animrow_stand_2 = 0;
	int animrow_run_up = 1;
	int animrow_run_upleft = 1;
	int animrow_run_upright = 1;
	int animrow_run_right = 1;
	int animrow_run_left = 1;
	int animrow_run_down = 1;
	int animrow_run_downright = 1;
	int animrow_run_downleft = 1;
	int animrow_skill_grenade = 2;
	int animrow_skill_2 = 2;
	int animrow_skill_3 = 2;
	int animrow_skill_4 = 2;
	int animrow_take_bullet = 2;
	int animrow_take_explosion = 2;
	int animrow_take_electricity = 2;

	int animrow_to_use = 0;				//prend la valeur de l'un des animrow
	int anim_selected = 0;				//selectionnera l'animrow_to_use		
	bool block_animrow = false;
	int special_anim_over = true;
	//std::vector<int>choose_anim_row;  //setup des animations lors de la création de l'unité


	//
	//Actions
	//
	bool action_start = false;
	bool action_over= true;
	bool non_cancellable_action_over = true;
	bool skill_over = false;
	bool skill_go = false;
	int action_to_do = 0;
	int current_action = 0;
	std::vector<int>actions_list;
	bool reset_action;
	int first_action_temp;
	bool next_action = true;

	////bool hold_position = false;


	//std::vector<int>skills_bool;

	bool priority = false;

	//pathsprite
	Sprite* path_sprite;
	int path_sprite_angle;
	int path_sprite_X;
	int path_sprite_Y;



};

