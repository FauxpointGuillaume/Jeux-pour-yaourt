#include "stdafx.h"
#include "Bouton.h"


Bouton::Bouton(Sprite* passed_sprite, int passed_bouton_action)
{
	bouton_sprite = passed_sprite;
	bouton_action = passed_bouton_action;
}


Bouton::~Bouton()
{
}


Sprite* Bouton::bouton_getsprite()
{
	return bouton_sprite;
}
void Bouton::bouton_setsprite(Sprite* passed_bouton_sprite)
{
	bouton_sprite = passed_bouton_sprite;
}
bool Bouton::bouton_getmouseover()
{
	return bouton_mouseover;
}
void Bouton::bouton_setmouseover(bool passed_bouton_mouseover)
{
	bouton_mouseover = passed_bouton_mouseover;
}
bool Bouton::bouton_getclick()
{
	return bouton_click;
}
void Bouton::bouton_setclick(bool passed_bouton_click)
{
	bouton_click = passed_bouton_click;
}
bool Bouton::bouton_getclicked()
{
	return bouton_clicked;
}
void Bouton::bouton_setclicked(bool passed_bouton_clicked)
{
	bouton_clicked = passed_bouton_clicked;
}
int Bouton::bouton_getaction()
{
	return bouton_action;
}
void Bouton::bouton_setaction(int passed_bouton_action)
{
	bouton_action = passed_bouton_action;
}