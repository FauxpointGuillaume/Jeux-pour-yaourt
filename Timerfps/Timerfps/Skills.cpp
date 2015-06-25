#include "stdafx.h"
#include "Skills.h"


Skills::Skills(SDL_Renderer* passed_renderer, Unit* passed_unit, Unit* passed_targetunit, int passed_usedskill, float *passed_camerax, float *passed_cameray, int passed_mousex, int passed_mousey)
{
	renderer = passed_renderer;
	unit = passed_unit;
	targetunit = passed_targetunit;
	used_skill = passed_usedskill;
	camerax = passed_camerax;
	cameray = passed_cameray;

	mousexy[0] = passed_mousex;
	mousexy[1] = passed_mousey;;
	unit_sprite = passed_unit->getsprite();
	//hand_grenade_sprite = NULL;
	//skill_number = passed_skill_number;
	//skill_number = unit->getnumber_of_skill_used();
	//unit_name = unit->getname();
	//no_reset = passed_no_reset;
}


Skills::~Skills()
{
	//grenade->~Sprite();
}

void Skills::hand_grenade()
{
	if (hand_grenade_finish == false)
	{
		if (hand_grenade_launched)
		{
			hand_grenade_angle += 4;
			hand_grenade_sprite->SetAngle(hand_grenade_angle);
			if (hand_grenade_coef >= 1)
			{
				if (hand_grenade_iteratormaxx*hand_grenade_speed + 1 * hand_grenade_speed< hand_grenade_movingx.size())
				{
					++hand_grenade_iteratormaxx;
					hand_grenade_sprite->SetX(hand_grenade_movingx[++hand_grenade_iteratorx*hand_grenade_speed]);
				}
				else
				{
					hand_grenade_finish = true;
				}
				if (hand_grenade_iteratormaxy*hand_grenade_speed + 1 * hand_grenade_speed< hande_grenade_movingy.size() && hand_grenade_iteratormaxy*hand_grenade_coef <= hand_grenade_iteratormaxx)
				{
					++hand_grenade_iteratormaxy;
					hand_grenade_sprite->SetY(hande_grenade_movingy[++hand_grenade_iteratory*hand_grenade_speed]);
				}
			}
			else
			{
				if (hand_grenade_iteratormaxx*hand_grenade_speed + 1 * hand_grenade_speed< hand_grenade_movingx.size() && hand_grenade_iteratormaxy*hand_grenade_coef >= hand_grenade_iteratormaxx)
				{
					++hand_grenade_iteratormaxx;
					hand_grenade_sprite->SetX(hand_grenade_movingx[++hand_grenade_iteratorx*hand_grenade_speed]);
				}
				if (hand_grenade_iteratormaxy*hand_grenade_speed + 1 * hand_grenade_speed< hande_grenade_movingy.size())
				{
					++hand_grenade_iteratormaxy;
					hand_grenade_sprite->SetY(hande_grenade_movingy[++hand_grenade_iteratory*hand_grenade_speed]);
				}
				else
				{
					hand_grenade_finish = true;
				}
			}
			hand_grenade_sprite->Draw();
		}
		else
		{
			extern SDL_Surface* surface_grenade;
			//unit->setaction_to_do(2);
			hand_grenade_sprite = new Sprite(renderer, surface_grenade, unit_sprite->GetX(), unit_sprite->GetY(), 40, 40, camerax, cameray);

			//hand_grenade_sprite = unit_sprite;
			//unit->setmove(false);
			//unit->setmoving(false);
			hand_grenade_movingx.clear();
			hande_grenade_movingy.clear();
			if (mousexy[0] <= hand_grenade_sprite->GetX() + hand_grenade_sprite->GetWidth()/2)
			{
				hand_grenade_distancex = hand_grenade_sprite->GetX() + hand_grenade_sprite->GetWidth() / 2 - mousexy[0];
				for (i = 0; i < hand_grenade_distancex; ++i)
				{
					hand_grenade_movingx.push_back(hand_grenade_sprite->GetX() - i);
				}
			}
			else if (mousexy[0] > hand_grenade_sprite->GetX() - hand_grenade_sprite->GetWidth() / 2)
			{
				hand_grenade_distancex = mousexy[0] - hand_grenade_sprite->GetX() - hand_grenade_sprite->GetWidth() / 2;
				for (i = 0; i < hand_grenade_distancex; ++i)
				{
					hand_grenade_movingx.push_back(hand_grenade_sprite->GetX() + i);
				}
			}
			if (mousexy[1] <= hand_grenade_sprite->GetY() + hand_grenade_sprite->GetHeight() / 2)
			{
				hand_grenade_distancey = hand_grenade_sprite->GetY() + hand_grenade_sprite->GetHeight() / 2 - mousexy[1];
				for (i = 0; i < hand_grenade_distancey; ++i)
				{
					hande_grenade_movingy.push_back(hand_grenade_sprite->GetY() - i);
				}
			}
			else if (mousexy[1] > hand_grenade_sprite->GetY() - hand_grenade_sprite->GetHeight() / 2)
			{
				hand_grenade_distancey = mousexy[1] - hand_grenade_sprite->GetY() - hand_grenade_sprite->GetHeight() / 2;
				for (i = 0; i < hand_grenade_distancey; ++i)
				{
					hande_grenade_movingy.push_back(hand_grenade_sprite->GetY() + i);
				}
			}
			//hand_grenade_distancey += hand_grenade_distancey == 0;
			hand_grenade_coef = (double)hand_grenade_distancex / (double)hand_grenade_distancey;
			hand_grenade_iteratorx = 0;
			hand_grenade_iteratory = 0;
			hand_grenade_iteratormaxx = 0;
			hand_grenade_iteratormaxy = 0;
			hand_grenade_launched = true;
		}
	}
	else
	{
		if (hand_grenade_explosion_animstart)
		{
			if (hand_grenade_explosion_animdelay + hand_grenade_explosion_animtime < countedFrames)
			{
				
				hand_grenade_explosion_sprite->~Sprite();
				finish_skill = true;
			}
			else
			{
				hand_grenade_explosion_sprite->PlayAnimation(0, 2, 0, 2, true);
				hand_grenade_explosion_sprite->Draw();
			}

		}
		else
		{
			hand_grenade_sprite->~Sprite();
			extern SDL_Surface* surface_explosion_grenade;
			hand_grenade_explosion_sprite = new Sprite(renderer, surface_explosion_grenade, hand_grenade_sprite->GetX()-20, hand_grenade_sprite->GetY()-20, 80, 80, camerax, cameray);
			hand_grenade_explosion_sprite->SetUpAnimation(3, 4);
			hand_grenade_explosion_animspeed = 3;
			hand_grenade_explosion_animtime = 54;
			hand_grenade_explosion_animdelay = countedFrames;
			hand_grenade_explosion_animstart = true;
			extern std::vector<Unit*> ennemyunits;
			int rayon = 40;
			float norma;
			for (auto &aze : ennemyunits)
			{
				//norma = sqrt((((hand_grenade_explosion_sprite->GetX()+20) - (aze->getsprite()->GetX() + aze->getsprite()->GetWidth() / 2))*((hand_grenade_explosion_sprite->GetX()+20) - (aze->getsprite()->GetX() + aze->getsprite()->GetWidth() / 2))) + (((hand_grenade_explosion_sprite->GetY()+20) - (aze->getsprite()->GetY() + aze->getsprite()->GetHeight() / 2))*((hand_grenade_explosion_sprite->GetY()+20) - (aze->getsprite()->GetY() + aze->getsprite()->GetHeight() / 2))));
				norma = sqrt(((hand_grenade_explosion_sprite->GetX() + 20) - (aze->getsprite()->GetX() + aze->getsprite()->GetWidth() / 2))*((hand_grenade_explosion_sprite->GetX() + 20) - (aze->getsprite()->GetX() + aze->getsprite()->GetWidth() / 2)) + ((hand_grenade_explosion_sprite->GetY() + 20) - (aze->getsprite()->GetY() + aze->getsprite()->GetHeight() / 2))*((hand_grenade_explosion_sprite->GetY() + 20) - (aze->getsprite()->GetY() + aze->getsprite()->GetHeight() / 2)));
				printf("Norm = %f", norma);
				if (norma > rayon)
				{
					std::cout << "FERME TA GUELE JE SUIS PAS MORT" << std::endl;
				}
				else
				{
					aze->sethp(aze->gethp()-10);
					std::cout << "PETIT BATARD" << std::endl;
				}
			}
		}
	}
}

void Skills::use_skill()
{
	//printf("\n%d", skill_number);
	//printf("%d", skill_number);
	//printf("%d", skill_number);
	//printf("%d", skill_number);
	//printf("%d", skill_number);
	//printf("%d", unit->getskill_number());


	//if (skill_go)
	//{
		if (used_skill == 0)
		{
			hand_grenade();
		}
		else
		{
			printf("\n\nERROR : SKILL NOT FOUND\n");
		}
	//}
	//else if (unit->getskill_go())
	//{
	//	//skill_go = true;
	//	//i = unit->getskill_number();
	//	printf("BZIOEFBZOEIFZEIOFBZEOFIBZEOFIBZEFOIZEBFOIZB\n\n\nEFZOEIFBZEOFBIZOEFIB\n");
	//	if (unit->getskill_number() == skill_number)
	//	{
	//		skill_go = true;
	//	}
	//
	//	unit->setskill_go(false);
	//
	//
	//	//if (unit->skills_bool_size() > 0)
	//	//{
	//	//
	//	//	//for (int i = 0; i < 2; ++i)
	//	//	//{
	//	//	//	printf("\n%d\n", unit->skills_bool_size());
	//	//	//	printf("\n%d\n", unit->skills_bool_size());
	//	//	//	printf("\n%d\n", unit->skills_bool_size());
	//	//	//}
	//	//	//switch (unit->skills_bool_get()[skill_number])
	//	//	switch (unit->skills_bool_at(0))
	//	//	{
	//	//	case 0:
	//	//		printf("MESCOUILLES\nMESCOUILLES\nMESCOUILLES\nMESCOUILLES\nMESCOUILLES\nMESCOUILLES\n");
	//	//		break;
	//	//	case 1:
	//	//		printf("LOL\nLOL\nLOL\nLOL\nLOL\nLOL\nLOL\nLOL\n");
	//	//		skill_go = true;
	//	//		break;
	//	//	case 2:
	//	//		printf("BITE\nBITE\nBITE\nBITE\nBITE\nBITE\nBITE\n");
	//	//		//unit->skills_bool_get().erase(unit->skills_bool_get().begin());
	//	//		finish_skill = true;
	//	//	}
	//	//}
	//	//else
	//	//{
	//	//	finish_skill = true;
	//	//}
	//}
	//else if (unit->getskill_reset() == true && no_reset == false)
	//{
	//	finish_skill = true;
	//}

}

void Skills::setmousexy(int passed_mousexy[])
{
	mousexy[0] = passed_mousexy[0];
	mousexy[1] = passed_mousexy[1];
}

bool Skills::getstart_skill()
{
	return start_skill;
}

bool Skills::getfinish_skill()
{
	return finish_skill;
}


Sprite* Skills::gethand_grenade_sprite()
{
	return hand_grenade_sprite;
}

Unit* Skills::getunit()
{
	return unit;
}

Sprite* Skills::getunit_sprite()
{
	return unit_sprite;
}

/*
float Skills::getunit_damage()
{
	return unit_lol->damage;
}*/