#include "stdafx.h"
#include "Unit.h"


Unit::Unit()
{}

Unit::Unit(Sprite* passed_sprite, std::string passed_name, float passed_hpmax, float passed_hp, float passed_damage, float passed_attackspeed, float passed_speed, float passed_defense, std::vector<int> & passed_skills, Sprite* passed_hudpicture, int passed_friendneutralfoe)
{
	sprite = passed_sprite;
	name = passed_name;
	hpmax = passed_hpmax;
	hp = passed_hp;
	damage = passed_damage;
	attackspeed = passed_attackspeed;
	speed = passed_speed;
	defense = passed_defense;
	skills = passed_skills;
	//skills.reserve(passed_skills.size());
	//std::copy(passed_skills.begin(), passed_skills.end(), std::back_inserter(skills));
	//skills.swap(passed_skills);
	hudpicture = passed_hudpicture;
	friendneutralfoe = passed_friendneutralfoe;
	//skills.clear();


	//Test
	//lifesurface = NULL;
	//liferect = { 0, 0, 0, 0 };
	//select = false;
	//pre_select = false;
	//moving = false;
	//selectedskill = false;
	//used_skill_number = 0;
	//used_skill = 0;
	//move = false;
	//moved = false;
	//rightclick = false;
	//distancecube2x=0;
	//distancecube2y=0;
	//coefcube2=0;
	//differencecube2=0;
	//i=0;
	//iteratorcube2x = 0;
	//iteratorcube2y = 0;
	//iteratorcube2xmax = 0;
	//iteratorcube2ymax = 0;


}


Unit::~Unit(void)
{
}


Sprite* Unit::getsprite()
{
	return sprite;
}

std::string Unit::getname()
{
	return name;
}

float Unit::gethp()
{
	return hp;
}

float Unit::gethpmax()
{
	return hpmax;
}

float Unit::getdamage()
{
	return damage;
}

int Unit::getattackspeed()
{
	return attackspeed;
}

int Unit::getspeed()
{
	return speed;
}

int Unit::getdefense()
{
	return defense;
}

std::vector<int> Unit::getskills()
{
	return skills;
}

Sprite* Unit::gethudpicture()
{
	return hudpicture;
}

bool Unit::getselect()
{
	return select;
}

bool Unit::getmove()
{
	return move;
}

bool Unit::getmoving()
{
	return moving;
}


int Unit::getfriendneutralfoe()
{
	return friendneutralfoe;
}

bool Unit::getselectedskill()
{
	return selectedskill;
}

void Unit::setstop_skill(bool passed_stop_skill)
{
	stop_skill = passed_stop_skill;
}
bool Unit::getstop_skill()
{
	return stop_skill;
}

void Unit::sethold_skill(bool passed_hold_skill)
{
	hold_skill = passed_hold_skill;
}
bool Unit::gethold_skill()
{
	return hold_skill;
}

bool Unit::getpre_select()
{
	return pre_select;
}

int Unit::getusedskill()
{
	return used_skill;
}

bool Unit::getanim()
{
	return anim;
}

void Unit::setsprite(Sprite* passed_sprite)
{
	sprite = passed_sprite;
}

void Unit::setname(std::string passed_name)
{
	name = passed_name;
}

void Unit::sethp(float passed_hp)
{
	hp = passed_hp;
}

void Unit::sethpmax(float passed_hpmax)
{
	hpmax = passed_hpmax;
}

void Unit::setdamage(float passed_damage)
{
	damage = passed_damage;
}

void Unit::setattackspeed(float passed_attackspeed)
{
	attackspeed = passed_attackspeed;
}

void Unit::setspeed(float passed_speed)
{
	speed = passed_speed;
}

void Unit::setdefense(float passed_defense)
{
	defense = passed_defense;
}

void Unit::setskills(std::vector<int> passed_skills)
{
	skills = passed_skills;
}

void Unit::skills_pushback(int passed_skill)
{
	skills.push_back(passed_skill);
}

int Unit::skills_at(int passed_i)
{
	return skills.at(passed_i);
}

int Unit::skills_size()
{
	return skills.size();
}

void Unit::sethudpicture(Sprite* passed_hudpicture)
{
	hudpicture = passed_hudpicture;
}

void Unit::setselect(bool passed_select)
{
	select = passed_select;
}

void Unit::setpre_select(bool passed_pre_select)
{
	pre_select = passed_pre_select;
}

void Unit::setmove(bool passed_move)
{
	move = passed_move;
}


void Unit::setmoving(bool passed_moving)
{
	moving = passed_moving;
}

void Unit::setmousexy(int passed_mousexy[])
{
	mousexy[0] = passed_mousexy[0];
	mousexy[1] = passed_mousexy[1];
}

int Unit::getmousex()
{
	return mousexy[0];
}
int Unit::getmousey()
{
	return mousexy[1];
}

void Unit::setfriendneutralfoe(int passed_friendneutralfoe)
{
	friendneutralfoe = passed_friendneutralfoe;
}

void Unit::setselectedskill(bool passed_selectedskill)
{
	selectedskill = passed_selectedskill;
}

void Unit::setusedskill(int passed_usedskill)
{
	used_skill = passed_usedskill;
}

void Unit::setanim(bool passed_anim)
{
	anim = passed_anim;
}
/*

void Unit::drawlife()
{
	hpmax;
}
*/

void Unit::setaction_to_do(int passed_action_to_do)
{
	action_to_do = passed_action_to_do;
}

int Unit::getaction_to_do()
{
	return action_to_do;
}

void Unit::setpriority(bool passed_priority)
{
	priority = passed_priority;
}

bool Unit::getpriority()
{
	return priority;
}

void Unit::setpath_sprite(Sprite* passed_pathsprite)
{
	path_sprite = passed_pathsprite;
}

void Unit::action_pushback(int passed_action)
{
	actions_list.push_back(passed_action);
}

std::vector<int> Unit::action_getlist()
{
	return actions_list;
}

void Unit::action_clearlist()
{
	actions_list.clear();
}

int Unit::action_size()
{
	return actions_list.size();
}

int Unit::getcurrent_action()
{
	return current_action;
}

//std::vector<std::pair<int, int>>  Unit::get_mousexylist()
//{
//	return vector_mousexy;
//}

int Unit::mousex_at(int passed_i)
{
	return vector_mousexy.at(passed_i).first;
}

int Unit::mousey_at(int passed_i)
{
	return vector_mousexy.at(passed_i).second;
}


void Unit::mousexy_pushback(int passed_mousex, int passed_mousey)
{
	vector_mousexy.push_back({ passed_mousex, passed_mousey });
}

void Unit::mousexylist_clear()
{
	vector_mousexy.clear();
}


//void Unit::skills_bool_pushback(int passed_boolint)
//{
//	skills_bool.push_back(passed_boolint);
//}
//
//void Unit::skills_bool_clear()
//{
//	skills_bool.clear();
//}

//int Unit::skills_bool_at(int passed_at)
//{
//	return skills_bool.at(passed_at);
//}

//int Unit::skills_bool_size()
//{
//	return skills_bool.size();
//}

//std::vector<int> Unit::skills_bool_get()
//{
//	return skills_bool;
//}

//void Unit::setskill_number(int passed_skill_number)
//{
//	used_skill_number = passed_skill_number;
//}
//int Unit::getskill_number()
//{
//	return used_skill_number;
//}

//void Unit::setnumber_of_skill_used(int passed_number_of_skill_used)
//{
//	number_of_skill_used = passed_number_of_skill_used;
//}
//
//int Unit::getnumber_of_skill_used()
//{
//	return number_of_skill_used;
//}
//
//void Unit::increment_number_of_skill_used()
//{
//	++number_of_skill_used;
//}


//void Unit::setskill_over(bool passed_skill_over)
//{
//	skill_over = passed_skill_over;
//}

void Unit::setskill_go(bool passed_skill_go)
{
	skill_go = passed_skill_go;
}

bool Unit::getskill_go()
{
	return skill_go;
}

void Unit::setaction_over(int passed_action_over)
{
	action_over = passed_action_over;
}

//void Unit::setskill_reset(bool passed_skill_reset)
//{
//	skill_reset = passed_skill_reset;
//}
//
//bool Unit::getskill_reset()
//{
//	return skill_reset;
//}

void Unit::setreset_action()
{
	reset_action = true;
}

int Unit::getskill_to_go()
{
	return skill_to_go;
}

void Unit::setaction_start(bool passed_action_start)
{
	action_start = passed_action_start;
}

void Unit::moveunit(int destinationX, int destinationY)
{
	if (reset_action)
	{
		move = true;
		reset_action = false;
	}

		if (move == true)
		{
			movingx.clear();
			movingy.clear();

			//mousexy[0] = vector_mousexy.at(0).first;
			//mousexy[1] = vector_mousexy.at(0).second;

			mousexy[0] = destinationX;
			mousexy[1] = destinationY;

			if (mousexy[0] <= sprite->GetX() +sprite->GetWidth()/2)
			{
				distancecube2x = sprite->GetX() + sprite->GetWidth() / 2 - mousexy[0];
				for (i = 0; i < distancecube2x; ++i)
				{
					movingx.push_back(sprite->GetX() - i);
				}
			}
			else if (mousexy[0] > sprite->GetX() - sprite->GetWidth() / 2)
			{
				distancecube2x = mousexy[0] - sprite->GetX() - sprite->GetWidth() / 2;
				for (i = 0; i < distancecube2x; ++i)
				{
					movingx.push_back(sprite->GetX() + i);
				}
			}
			if (mousexy[1] <= sprite->GetY() + sprite->GetHeight() / 2)
			{
				distancecube2y = sprite->GetY() + sprite->GetHeight() / 2 - mousexy[1];
				for (i = 0; i < distancecube2y; ++i)
				{
					movingy.push_back(sprite->GetY() - i);
				}
			}
			else if (mousexy[1] > sprite->GetY() - sprite->GetHeight() / 2)
			{
				distancecube2y = mousexy[1] - sprite->GetY() - sprite->GetHeight() / 2;
				for (i = 0; i < distancecube2y; ++i)
				{
					movingy.push_back(sprite->GetY() + i);
				}
			}
			//distancecube2y += distancecube2y == 0;
			coefcube2 = (double)distancecube2x / (double)distancecube2y;
			iteratorcube2x = distancecube2x;
			iteratorcube2y = distancecube2y;
			iteratorcube2xmax = 0;
			iteratorcube2ymax = 0;
			moving = true;
			move = false;

		}

		if (moving == true)
		{
			//printf("%f\n", coefcube2);
			//printf("%f\n", coefcube2);
			//printf("%f\n", coefcube2);
			//printf("%f\n", coefcube2);
			choose_animrow(2);
			if (coefcube2 >= 1)
			{
				if (iteratorcube2xmax < movingx.size())
				{
					++iteratorcube2xmax;
					sprite->SetX(movingx[++iteratorcube2x - distancecube2x - 1]);
				}
				else
				{
					action_to_do = 0;
					moving = false;
					action_over = true;
				}
				if (iteratorcube2ymax < movingy.size() && iteratorcube2ymax*coefcube2 <= iteratorcube2xmax)
				{
					++iteratorcube2ymax;
					sprite->SetY(movingy[++iteratorcube2y - distancecube2y - 1]);
				}

			}
			else
			{

				if (iteratorcube2xmax < movingx.size() && iteratorcube2ymax*coefcube2 >= iteratorcube2xmax)
				{
					++iteratorcube2xmax;
					sprite->SetX(movingx[++iteratorcube2x - distancecube2x - 1]);
				}
				if (iteratorcube2ymax < movingy.size())
				{
					++iteratorcube2ymax;
					sprite->SetY(movingy[++iteratorcube2y - distancecube2y - 1]);
				}
				else
				{
					action_to_do = 0;
					moving = false;
					action_over = true;
				}
			}

		}
}

void Unit::drawlife(SDL_Renderer *renderer, float camerax, float cameray)
{
	SDL_SetRenderDrawColor(renderer,0, 0, 0, 255);
	liferect.x = sprite->GetX() +camerax;
	liferect.y = sprite->GetY() + cameray - 10;
	liferect.h = 10;
	liferect.w = 50;
	SDL_RenderFillRect(renderer, &liferect);
	if (hp > 0)
	{
		if (friendneutralfoe == 0)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		}
		if (friendneutralfoe == 1)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		}if (friendneutralfoe == 2)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		}
		++liferect.x;
		++liferect.y;
		liferect.h = 8;
		liferect.w = liferect.w*(((hp * 100) / hpmax) / 100) - 2;
		SDL_RenderFillRect(renderer, &liferect);
	}

}

void Unit::mouseover(SDL_Renderer *renderer,float camerax, float cameray)
{

}

void Unit::draw()
{
	sprite->Draw();
	if (anim = true)
	{
		sprite->PlayAnimation(anim_beginframe,anim_endframe,animrow_to_use,anim_speed,NULL);
	}
}

void Unit::anim_nade()
{

	//
	//
	///a centraliser comme use_skill
	//
	//
	if (anim_nade_start)
	{
		if (animationdelay + animationtime < countedFrames)
		{
			//anim_row = animrow_skill_grenade;
			choose_animrow(0);
			anim_speed = 6;
			//nomove_anim = false;
			anim_nade_start = false;
			special_anim_over = true;
			non_cancellable_action_over = true;
			action_over = true;
			skill_over = true;
			//action_to_do = 0;
		}
	}
	else
	{
		//++used_skill_number;
		choose_animrow(10);
		//move = false;
		//moving = false;
		//nomove_anim = true;
		anim_speed = 18;
		//anim_row = 2;
		animationtime = 54;
		animationdelay = countedFrames;
		anim_nade_start = true;
		non_cancellable_action_over = false;
	}
}

void Unit::choose_animrow(int passed_usedanim)		//switch case a faire
{
	if (passed_usedanim < 10)
	{
		if (passed_usedanim == 0)
		{
			animrow_to_use = animrow_stand_1;
		}
		if (passed_usedanim == 1)
		{
			animrow_to_use = animrow_stand_2;
		}
		if (passed_usedanim == 2)
		{
			animrow_to_use = animrow_run_up;
		}
		if (passed_usedanim == 3)
		{
			animrow_to_use = animrow_run_upright;
		}
		if (passed_usedanim == 4)
		{
			animrow_to_use = animrow_run_right;
		}
		if (passed_usedanim == 5)
		{
			animrow_to_use = animrow_run_downright;
		}
		if (passed_usedanim == 6)
		{
			animrow_to_use = animrow_run_down;
		}
		if (passed_usedanim == 7)
		{
			animrow_to_use = animrow_run_downleft;
		}
		if (passed_usedanim == 8)
		{
			animrow_to_use = animrow_run_left;
		}
		if (passed_usedanim == 9)
		{
			animrow_to_use = animrow_run_upleft;
		}

	}
	else if (passed_usedanim < 15)
	{

		if (passed_usedanim == 10)
		{
			animrow_to_use = animrow_skill_grenade;
		}
		if (passed_usedanim == 11)
		{
			animrow_to_use = animrow_skill_2;
		}
		if (passed_usedanim == 12)
		{
			animrow_to_use = animrow_skill_3;
		}
		if (passed_usedanim == 13)
		{
			animrow_to_use = animrow_skill_4;
		}
		if (passed_usedanim == 14)
		{
			animrow_to_use = animrow_skill_4;
		}
	}
	else if (passed_usedanim < 20)
	{
		if (passed_usedanim == 15)
		{
			animrow_to_use = animrow_take_bullet;
		}
		if (passed_usedanim == 16)
		{
			animrow_to_use = animrow_take_explosion;
		}
		if (passed_usedanim == 17)
		{
			animrow_to_use = animrow_take_electricity;
		}
	}
}

//void Unit::anim_to_use(int anim_to_use)
//{
//	if (special_anim_over)
//	{
//		choose_animrow(anim_selected);
//	}
//
//}

void Unit::action()
{
	//if (special_action_over)
	//{
		//current_action = action_to_do;
		////if (actions_list.size() > 0)
		////{
		////	actions_list.erase(actions_list.begin());
		////}

			
		

		//if (reset_action && actions_list.size() > 0)
		//{
		//	if (special_action_over)
		//	{
		//
		//		first_action_temp = actions_list.back();
		//		actions_list.clear();
		//		actions_list.push_back(first_action_temp);
		//		special_action_over = false;
		//	}
		//	//else
		//	//{
		//	//	first_action_temp = actions_list.back();
		//	//	actions_list.clear();
		//	//	actions_list.push_back(first_action_temp);
		//	//}
		//}

		//if (next_action)
		//{
		//	if (actions_list.size() > 0)
		//	{
		//		current_action = actions_list.at(0);
		//		mousexy[0] = vector_mousexy[0].first;
		//		mousexy[1] = vector_mousexy[0].second;
		//	}
		//	else
		//	{
		//		current_action = 0;
		//	}
		//}

	if (action_over)
	{
		//if (actions_list.size() > 0)
		//{
		//	//if (actions_list.at(0) < 3)
		//	//{
		//	//	
		//	//}
		//	//actions_list.erase(actions_list.begin());
		//	//if (vector_mousexy.size() > 0)
		//	//{
		//	//	vector_mousexy.erase(vector_mousexy.begin());
		//	//}
		//
		//
		//	//if (skills_bool.size() == 0)
		//	//{
		//	//	used_skill_number = 0;
		//	//}
		//
		//	//used_skill_number = skills_bool.size();
		//	//if (skills_bool.size() == 0)
		//	//{
		//	//	used_skill_number = 0;
		//	//}
		//	//if (skills_bool.size() > 0)
		//	//{
		//	//	
		//	//	//if (skill_over)
		//	//	//{
		//	//	//	skills_bool.erase(skills_bool.begin());
		//	//	//	skill_over = false;
		//	//	//}
		//	//}
		//}
		action_start = true;
		action_over = false;
	}

	if (action_start)
	{
		if (non_cancellable_action_over)
		{
			if (actions_list.size() > 0)
			{
				current_action = actions_list.at(0);
				mousexy[0] = vector_mousexy[0].first;
				mousexy[1] = vector_mousexy[0].second;
				actions_list.erase(actions_list.begin());
				if (vector_mousexy.size() > 0)
				{
					vector_mousexy.erase(vector_mousexy.begin());
				}
				if (current_action >= 5)
				{
					skill_go = true;
					skill_to_go = 0;
				}
			}
			else
			{
				current_action = 0;
			}
			action_start = false;
		}
	}

	//}

	//if (skill_over = true)
	//{
	//	skill_over = false;
	//}
	//skill_go = false;

	switch (current_action)
	{
		case 0:
			action_over = true;
			choose_animrow(0);
			break;
		case 1:
			if (move == false && moving == false)
			{
				move = true;
			}
			moveunit(mousexy[0],mousexy[1]);
			break;
		case 2:
			////holdposition = true;
			action_over = true;
			choose_animrow(0);
			break;
		case 3:
			//
			//Patrol
			//
			if (move == false && moving == false)
			{
				move = true;
			}
			moveunit(mousexy[0], mousexy[1]);
			break;
		case 4:
			//
			//Moveattack
			//
			if (move == false && moving == false)
			{
				move = true;
			}
			moveunit(mousexy[0], mousexy[1]);
			break;
		case 5:
			//if (skills_bool.size() > 0)		//Peut-être qu'il faudra repasse la dessus
			//{									//Plutot que de rester sur
			//	skills_bool.at(0) = 1;			//skill_go = true au dessus du switch
			//}
			//if (skill_go)
			//{
			//	skill_go = false;
			//}
			anim_nade();
			break;
		case 6:
			//moveunit();
			choose_animrow(4);
			break;
		default:
			printf("\n\nEt mes couilles c'est du kfc?\n\n");
			break;
	}
	
	
}