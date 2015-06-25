#include "stdafx.h"
#include "Hud.h"


Hud::Hud(SDL_Renderer* passed_renderer, Sprite* hud, Unit* unit, Sprite* passed_minimap)
{
	hudstyle = hud;
	hudunit = unit;
	minimap = passed_minimap;
	renderer = passed_renderer;
	hudunit->gethudpicture()->SetX(300);
	hudunit->gethudpicture()->SetY(660);
	hudunit->gethudpicture()->SetHeight(100);
	hudunit->gethudpicture()->SetWidth(100);

	minimap->SetX(10);
	minimap->SetY(595);
	hudtext.setFont("Xenotron.ttf", 18);
	skill_text_texture.setFont("Xenotron.ttf", 18);
	//Set text color as black
	textColor = { 0, 0, 20, 255 };
	skill_text_color = { 0, 0, 20, 255 };

	skill_preselect_rect.h = 50;
	skill_preselect_rect.w = 50;

	grenade = new Sprite(renderer, "grenade.jpg", 0, 600, 50, 50, &classhudposx, &classhudposy);
}

Hud::~Hud()
{
}


void Hud::setunit(Unit* unit)
{
	hudunit = unit;
}

void Hud::setminimap(Sprite* passed_minimap)
{
	minimap = passed_minimap;
}

void Hud::addunit(Unit* passed_unit)
{
	hudunits.push_back(passed_unit);
}

bool Hud::hudunitsempty()
{
	return hudunits.empty();
}

void Hud::hudunitsclear()
{
	hudunits.clear();
}

int Hud::hudunitssize()
{
	return hudunits.size();
}

Unit* Hud::gethudunit()
{
	return hudunit;
}

std::vector<Unit*> Hud::gethudunits()
{
	return hudunits;
}

Sprite* Hud::gethudstyle()
{
	return hudstyle;
}

Sprite* Hud::getminimap()
{
	return minimap;
}

void Hud::drawtext()
{
	if (hudunits.size() < 2)
	{
		hudunit->gethudpicture()->SetX(300);
		hudunit->gethudpicture()->SetY(660);
		hudunit->gethudpicture()->SetWidth(100);
		hudunit->gethudpicture()->SetHeight(100);
		hudunit->gethudpicture()->Draw();

		text.str("");
		text << "\n" << "Health : " << hudunit->gethp() << "/" << hudunit->gethpmax() << "  speed : " << hudunit->getspeed() << "\n\n" << "damage : " << hudunit->getdamage() << "   defense : " << hudunit->getdefense();

		hudtextstring = text.str().c_str();
		//Render text
		if (!hudtext.loadFromRenderedText(renderer, textsurface, hudtextstring, textColor))
		{
			printf("Unable to render FPS texture!\n");
		}

		//Render textures
		hudtext.render(renderer, hudstyle->GetX() + 400, hudstyle->GetY() + 60);
	}
	else
	{
		hudunitsx = 300;

		for (auto &aze : hudunits)
		{
			aze->gethudpicture()->SetX(hudunitsx);
			aze->gethudpicture()->SetY(660);
			aze->gethudpicture()->SetWidth(50);
			aze->gethudpicture()->SetHeight(50);

			if (aze->getpriority())
			{
				aze->gethudpicture()->SetX(hudunitsx);
				aze->gethudpicture()->SetY(655);
				aze->gethudpicture()->SetWidth(60);
				aze->gethudpicture()->SetHeight(60);	
				hudunitsx += 10;
			}


			aze->gethudpicture()->Draw();

			hudunitsx += 50;
		}
	}
	
}



void Hud::drawminimap(std::vector<Unit*> units)
{

	for (auto &unit : units) // access by reference to avoid copying
	{
		if (unit->getfriendneutralfoe() == 0)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0xFF);
		}if (unit->getfriendneutralfoe() == 1)
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 50, 0xFF);
		}if (unit->getfriendneutralfoe() == 2)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
		}


		point.x = unit->getsprite()->GetX()/7+10;
		point.y = unit->getsprite()->GetY()/8+600;
		
		SDL_RenderDrawPoint(renderer, point.x, point.y);
		SDL_RenderDrawPoint(renderer, point.x, point.y+1);
		SDL_RenderDrawPoint(renderer, point.x+1, point.y);
		SDL_RenderDrawPoint(renderer, point.x+1, point.y+1);
		//SDL_RenderDrawLine(renderer, unit->getsprite()->GetX()/7 + 10, unit->getsprite()->GetY()/8.5 +600, unit->getsprite()->GetX()/7+20, unit->getsprite()->GetY()/8.5 + 610);

		if (unit->getselect())
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
			SDL_RenderDrawPoint(renderer, point.x-1, point.y+2);
			SDL_RenderDrawPoint(renderer, point.x, point.y+2);
			SDL_RenderDrawPoint(renderer, point.x+1, point.y+2);
			SDL_RenderDrawPoint(renderer, point.x+2, point.y+2);

			SDL_RenderDrawPoint(renderer, point.x-1, point.y-1);
			SDL_RenderDrawPoint(renderer, point.x, point.y-1);
			SDL_RenderDrawPoint(renderer, point.x+1, point.y-1);
			SDL_RenderDrawPoint(renderer, point.x+2, point.y-1);

			SDL_RenderDrawPoint(renderer, point.x-1, point.y);
			SDL_RenderDrawPoint(renderer, point.x-1, point.y+1);

			SDL_RenderDrawPoint(renderer, point.x+2, point.y);
			SDL_RenderDrawPoint(renderer, point.x+2, point.y+1);
		}
	}
}

void Hud::drawskills(Unit* passed_priority_unit)
{
	int x = 0;
	int y = 600;
	int x_skills = 0;
	if (passed_priority_unit->skills_size() > 0)
	{
		for (int i = 0; i < passed_priority_unit->skills_size(); ++i)
		{

			switch (passed_priority_unit->skills_at(i))
			{
			case 1:
				grenade->SetX(757 + x);
				grenade->SetY(y);
				grenade->Draw();
				if (passed_priority_unit->getselectedskill())
				{
					if (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()) == 1)
					{
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &grenade->GetRect());
						SDL_SetRenderDrawColor(renderer, 0, 255, 100, 50);
						SDL_RenderFillRect(renderer, &grenade->GetRect());
					}
				}
				x += 52;
				if (x > 210)
				{
					x = 0;
					y += 52;
				}
				break;
			case 0:
				grenade->SetX(757 + x);
				grenade->SetY(y);
				grenade->Draw();
				if (passed_priority_unit->getselectedskill())
				{
					if (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()) == 0)
					{
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &grenade->GetRect());
						SDL_SetRenderDrawColor(renderer, 0, 255, 100, 50);
						SDL_RenderFillRect(renderer, &grenade->GetRect());
					}
				}
				x += 52;
				if (x > 210)
				{
					x = 0;
					y += 52;
				}
				break;
			case 2:
				grenade->SetX(757 + x);
				grenade->SetY(y);
				grenade->Draw();
				if (passed_priority_unit->getselectedskill())
				{
					if (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()) == 2)
					{
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &grenade->GetRect());
						SDL_SetRenderDrawColor(renderer, 0, 255, 100, 50);
						SDL_RenderFillRect(renderer, &grenade->GetRect());
					}
				}
				x += 52;
				if (x > 210)
				{
					x = 0;
					y += 52;
				}
				break;
			case 3:
				grenade->SetX(757 + x);
				grenade->SetY(y);
				grenade->Draw();
				if (passed_priority_unit->getselectedskill())
				{
					if (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()) == 3)
					{
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &grenade->GetRect());
						SDL_SetRenderDrawColor(renderer, 0, 255, 100, 50);
						SDL_RenderFillRect(renderer, &grenade->GetRect());
					}
				}
				x += 52;
				if (x > 210)
				{
					x = 0;
					y += 52;
				}
				break;
			case 4:
				grenade->SetX(757 + x);
				grenade->SetY(y);
				grenade->Draw();
				if (passed_priority_unit->getselectedskill())
				{
					if (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()) == 4)
					{
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &grenade->GetRect());
						SDL_SetRenderDrawColor(renderer, 0, 255, 100, 50);
						SDL_RenderFillRect(renderer, &grenade->GetRect());
					}
				}
				x += 52;
				if (x > 210)
				{
					x = 0;
					y += 52;
				}
				break;
			case 5:
				grenade->SetX(300 + x_skills);
				grenade->SetY(600);
				grenade->Draw();
				if (passed_priority_unit->getselectedskill())
				{
					if (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()) == 5)
					{
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &grenade->GetRect());
						SDL_SetRenderDrawColor(renderer, 0, 255, 100, 50);
						SDL_RenderFillRect(renderer, &grenade->GetRect());
					}
				}
				x_skills += 52;
				break;

			}




			//if (passed_priority_unit->skills_at(i) == 2)
			//{
			//	grenade->SetX(820 + x);
			//	grenade->Draw();
			//	if (passed_priority_unit->getselectedskill())
			//	{
			//		if (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()) == 2)
			//		{
			//			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			//			SDL_RenderDrawRect(renderer, &grenade->GetRect());
			//			SDL_SetRenderDrawColor(renderer, 0, 255, 100, 50);
			//			SDL_RenderFillRect(renderer, &grenade->GetRect());
			//		}
			//	}
			//}
		}
		if (passed_priority_unit->getselectedskill())
		{

			switch (passed_priority_unit->skills_at(passed_priority_unit->getusedskill()))
			{
			case 1: 
				text_skill.str("");
				text_skill << "Clic gauche \npour se deplacer";

				text_skill_string = text_skill.str().c_str();
				//Render text
				if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
				{
					printf("Unable to render FPS texture!\n");
				}
				//Render textures
				skill_text_texture.render(renderer, 760, 670);
				break;
			case 3:

				text_skill.str("");
				text_skill << "Clic gauche \npour patrouiller";

				text_skill_string = text_skill.str().c_str();
				//Render text
				if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
				{
					printf("Unable to render FPS texture!\n");
				}
				//Render textures
				skill_text_texture.render(renderer, 760, 670);
				break;
			case 4:
				text_skill.str("");
				text_skill << "Clic gauche \npour moveattack";

				text_skill_string = text_skill.str().c_str();
				//Render text
				if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
				{
					printf("Unable to render FPS texture!\n");
				}
				//Render textures
				skill_text_texture.render(renderer, 760, 670);
				break;
			case 5:
				text_skill.str("");
				text_skill << "Clic gauche \npour lancer une grenade";

				text_skill_string = text_skill.str().c_str();
				//Render text
				if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
				{
					printf("Unable to render FPS texture!\n");
				}
				//Render textures
				skill_text_texture.render(renderer, 760, 670);
				break;
			default:
				break;
			}




			//text_skill.str("");
			//text_skill << "SKILL DE OUF :\n MOVE";
			//
			//text_skill_string = text_skill.str().c_str();
			////Render text
			//if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
			//{
			//	printf("Unable to render FPS texture!\n");
			//}
			////Render textures
			//skill_text_texture.render(renderer, 700, 700);

		}




	}

}

bool Hud::skill_mouse_actions(Unit* passed_priority_unit, int mousex, int mousey, bool passed_leftclick, bool passed_leftclick_release, bool passed_shift)
{
	skill_selected = false;
	if (mousey > 600)
	{
		if (mousex > 756)
		{
			if (mousey < 653)
			{
				if (mousex < 808)
				{
					draw_skill_text(passed_priority_unit->skills_at(1));
					if (passed_leftclick)
					{
						skill_preselect_rect.x = 757;
						skill_preselect_rect.y = 600;
						SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
						SDL_RenderFillRect(renderer, &skill_preselect_rect);
						skill_preselect = true;
					}
					else if (passed_leftclick_release && skill_preselect)
					{
						for (auto &aze : hudunits)
						{
							aze->setusedskill(0);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
							aze->setselectedskill(true);
						}
						skill_selected = true;
						skill_preselect = false;
					}
				}
				else if (mousex < 860)
				{
					draw_skill_text(passed_priority_unit->skills_at(0));
					if (passed_leftclick)
					{
						skill_preselect_rect.x = 809;
						skill_preselect_rect.y = 600;
						SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
						SDL_RenderFillRect(renderer, &skill_preselect_rect);
						skill_preselect = true;
					}
					else if (passed_leftclick_release && skill_preselect)
					{
						for (auto &aze : hudunits)
						{
							aze->setusedskill(1);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
							if (passed_shift == false)
							{
								aze->action_clearlist();
								aze->setreset_action();
								aze->mousexylist_clear();
								aze->setaction_over(true);
							}
							aze->action_pushback(0);
							aze->mousexy_pushback(0, 0);
							aze->setselectedskill(false);
						}
						skill_selected = false;
						skill_preselect = false;
					}
				}
				else if (mousex < 912)
				{
					draw_skill_text(passed_priority_unit->skills_at(2));
					if (passed_leftclick)
					{
						skill_preselect_rect.x = 861;
						skill_preselect_rect.y = 600;
						SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
						SDL_RenderFillRect(renderer, &skill_preselect_rect);
						skill_preselect = true;
					}
					else if (passed_leftclick_release && skill_preselect)
					{
						for (auto &aze : hudunits)
						{
							aze->setusedskill(2);
							if (passed_shift == false)
							{
								aze->action_clearlist();
								aze->setreset_action();
								aze->mousexylist_clear();
								aze->setaction_over(true);
							}
							aze->action_pushback(2);
							aze->mousexy_pushback(0, 0);
							aze->setselectedskill(false);
						}
						skill_selected = false;
						skill_preselect = false;
					}
				}
				else if (mousex < 964)
				{
					draw_skill_text(passed_priority_unit->skills_at(3));
					if (passed_leftclick)
					{
						skill_preselect_rect.x = 913;
						skill_preselect_rect.y = 600;
						SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
						SDL_RenderFillRect(renderer, &skill_preselect_rect);
						skill_preselect = true;
					}
					else if (passed_leftclick_release && skill_preselect)
					{
						for (auto &aze : hudunits)
						{
							aze->setusedskill(3);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
							aze->setselectedskill(true);
						}
						skill_selected = true;
						skill_preselect = false;
					}
				}
				else if (mousex < 1016)
				{
					draw_skill_text(passed_priority_unit->skills_at(4));
					if (passed_leftclick)
					{
						skill_preselect_rect.x = 965;
						skill_preselect_rect.y = 600;
						SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
						SDL_RenderFillRect(renderer, &skill_preselect_rect);
						skill_preselect = true;
					}
					else if (passed_leftclick_release && skill_preselect)
					{
						for (auto &aze : hudunits)
						{
							aze->setusedskill(4);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
							aze->setselectedskill(true);
						}
						skill_selected = true;
						skill_preselect = false;
					}
				}
			}
		//	else if (mousey < 705)
		//	{
		//		if (mousex < 808)
		//		{
		//			draw_skill_text(passed_priority_unit->skills_at(5));
		//			if (passed_leftclick)
		//			{
		//				skill_preselect_rect.x = 757;
		//				skill_preselect_rect.y = 652;
		//				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
		//				SDL_RenderFillRect(renderer, &skill_preselect_rect);
		//				skill_preselect = true;
		//			}
		//			else if (passed_leftclick_release && skill_preselect)
		//			{
		//				passed_priority_unit->setusedskill(5);
		//				passed_priority_unit->setselectedskill(true);
		//				skill_selected = true;
		//				skill_preselect = false;
		//			}
		//		}
		//		else if (mousex < 860)
		//		{
		//			grenade->SetY(450);
		//			grenade->SetX(808);
		//			grenade->Draw();
		//			printf("LOOOOOOOL");
		//		}
		//		else if (mousex < 912)
		//		{
		//			grenade->SetY(450);
		//			grenade->SetX(860);
		//			grenade->Draw();
		//			printf("LOOOOOOOL");
		//		}
		//		else if (mousex < 964)
		//		{
		//			grenade->SetY(450);
		//			grenade->SetX(912);
		//			grenade->Draw();
		//			printf("LOOOOOOOL");
		//		}
		//		else if (mousex < 1016)
		//		{
		//			grenade->SetY(450);
		//			grenade->SetX(964);
		//			grenade->Draw();
		//			printf("LOOOOOOOL");
		//		}
		//
		//	}
		}
		else if (mousex > 300)
		{
			if (mousey < 653)
			{
				if (mousex < 350)
				{
					draw_skill_text(passed_priority_unit->skills_at(5));
					if (passed_leftclick)
					{
						skill_preselect_rect.x = 300;
						skill_preselect_rect.y = 600;
						SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
						SDL_RenderFillRect(renderer, &skill_preselect_rect);
						skill_preselect = true;
					}
					else if (passed_leftclick_release && skill_preselect)
					{
						passed_priority_unit->setusedskill(5);
						passed_priority_unit->setselectedskill(true);
						skill_selected = true;
						skill_preselect = false;
					}
				}
			}
		}
	}
	if (!passed_leftclick)
	{
		skill_preselect = false;
	}
	return skill_selected;
}



void Hud::draw_skill_text(int skill)
{
	switch (skill)
	{
	case 0:
		text_skill.str("");
		text_skill << "SKILL DE OUF :\n MOVE";

		text_skill_string = text_skill.str().c_str();
		//Render text
		if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
		{
			printf("Unable to render FPS texture!\n");
		}
		//Render textures
		skill_text_texture.render(renderer, 700, 500);
		break;
	case 1:
		text_skill.str("");
		text_skill << "GROS SKILL DE OUF :\n STOP";

		text_skill_string = text_skill.str().c_str();
		//Render text
		if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
		{
			printf("Unable to render FPS texture!\n");
		}

		//Render textures
		skill_text_texture.render(renderer, 700, 500);
		break;
	case 2:
		text_skill.str("");
		text_skill << "GROS SKILL DE OUF :\n HOLD";

		text_skill_string = text_skill.str().c_str();
		//Render text
		if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
		{
			printf("Unable to render FPS texture!\n");
		}

		//Render textures
		skill_text_texture.render(renderer, 700, 500);
		break;
	case 3:
		text_skill.str("");
		text_skill << "GROS SKILL DE OUF :\n PATROL";

		text_skill_string = text_skill.str().c_str();
		//Render text
		if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
		{
			printf("Unable to render FPS texture!\n");
		}

		//Render textures
		skill_text_texture.render(renderer, 700, 500);
		break;
	case 4:
		text_skill.str("");
		text_skill << "GROS SKILL DE OUF :\n MOVEATTACK";

		text_skill_string = text_skill.str().c_str();
		//Render text
		if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
		{
			printf("Unable to render FPS texture!\n");
		}

		//Render textures
		skill_text_texture.render(renderer, 700, 500);
		break;
	case 5:
		text_skill.str("");
		text_skill << "GROS SKILL DE OUF :\n GRENADE";

		text_skill_string = text_skill.str().c_str();
		//Render text
		if (!skill_text_texture.loadFromRenderedText(renderer, textsurface_skill, text_skill_string, skill_text_color))
		{
			printf("Unable to render FPS texture!\n");
		}

		//Render textures
		skill_text_texture.render(renderer, 700, 500);
		break;
	}
}