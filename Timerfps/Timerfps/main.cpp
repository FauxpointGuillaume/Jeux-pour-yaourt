#include "stdafx.h"
/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

// TODO : 

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL/include/SDL.h>
#include <SDL/include/SDL_image.h>
#include <SDL/include/SDL_ttf.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_projection.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/core/type_vec3.hpp>
#include <GLM/glm/core/type_vec4.hpp>
#include <GLM/glm/core/type_mat4x4.hpp> 
#include <GLM/glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
////#include <SDL/include/SDL_opengl.h>

#include "camera.h"
#include "Sprite.h"
#include "map.h"
#include "Unit.h"
#include "Hud.h"
#include "LTexture.h"
#include "Skills.h"
#include "Shader.h"
#include "Bouton.h"

//Frames comptés, unité de temps du jeu(global)
int countedFrames = 0;

//
//global
//
SDL_Surface* surface_explosion_grenade = IMG_Load("grenade_explosion.png");
SDL_Surface* surface_grenade = IMG_Load("bombe1.bmp");	

//Unités
std::vector<Unit*> units;
std::vector<Unit*> allyunits;
std::vector<Unit*> neutralunits;
std::vector<Unit*> ennemyunits;
std::vector<Unit*> selectedallies;
std::vector<Unit*> pre_selectedunits;

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//sert a rien
const int LEVEL_WIDTH = 2000;
const int LEVEL_HEIGHT = 2000;

//Initialisation des objets SDL
SDL_Rect rectToDraw = { 100, 100, 100, 100 };
SDL_Surface *ecran = NULL, *rectangle = NULL, *rectangle2 = NULL, *hud = NULL,
*obstacle = NULL, *fpscount = NULL, *surface_fogofwar2 = NULL;
SDL_Texture *texture = NULL, *texture2 = NULL, *hudtex = NULL, *obstacletex = NULL;
//Starts up SDL and creates window
bool init();

int Gl_init();

//Loads media
//bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gFPSTextTexture;

//hud text
LTexture hudtext;

//The application time based timer

// TODO : Faire une classe séparé
class LTimer
{
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}

void input_handle(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;
		}
	}
}

void UpdateEvents(SDL_Surface* ecran)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_RESIZED:
			break;
		}
	}
}

// -------------- CLASS INPUT

class Input
{
protected:
	int mousex, mousey;
	int mousexrel, mouseyrel;
	bool key[SDL_NUM_SCANCODES];
	char mousebuttons[8];
	char quit;
public:
	Input();
	~Input(){}
	void Update();
	bool Key(bool key){ return key; }
	inline int MouseX(){ return mousex; }
	inline int MouseY(){ return mousey; }
	inline int MouseXrel(){ return mousexrel; }
	inline int MouseYrel(){ return mouseyrel; }
	inline int MouseButton(int i){ return mousebuttons[i]; }
	inline int Quit(){ return quit; }

	bool getTouche(const SDL_Scancode touche) const;
};

Input::Input()
{
	SDL_memset(this, 0, sizeof(*this));
	for (int i(0); i < SDL_NUM_SCANCODES; i++)
		key[i] = false;

}

void Input::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			key[event.key.keysym.scancode] = true;
			break;
		case SDL_KEYUP:
			key[event.key.keysym.scancode] = false;
			break;
		case SDL_MOUSEMOTION:
			mousex = event.motion.x;
			mousey = event.motion.y;
			mousexrel = event.motion.xrel;
			mouseyrel = event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mousebuttons[event.button.button] = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button && event.button.button)
				mousebuttons[event.button.button] = 0;
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_WINDOWEVENT_RESIZED:
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		default:
			break;
		}
	}
}

bool Input::getTouche(const SDL_Scancode touche) const
{
	return key[touche];
}


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		ecran = SDL_GetWindowSurface(gWindow);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	//gFPSTextTexture.free();


	//Destroy window	
//	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	float CameraX = 200;
	float CameraY = 300;
	float hudposx = 0;
	float hudposy = 0;

	int frameTicks = 0;
	bool game_game_quit = false;
	bool level_quit = false;


	//ColorKey des surfaces globales
	SDL_SetColorKey(surface_explosion_grenade, SDL_TRUE, SDL_MapRGB(surface_explosion_grenade->format, 0xFF, 0xFF, 0xFF));
	SDL_SetColorKey(surface_grenade, SDL_TRUE, SDL_MapRGB(surface_grenade->format, 0xFF, 0xFF, 0xFF));

	bool menu_loaded = false;
	bool menu_finish = false;
	std::vector<Bouton*> menu_vector_boutons;
	int menu_action = 0;
	LTexture menu_loading_text;
	menu_loading_text.setFont("Xenotron.ttf", 18);
	std::stringstream menu_stringstream_loadingtext;
	std::string menu_string_loadingtext;
	SDL_Surface *menu_textsurface = NULL;



	//On a tous besoin d'un i
	int i = 0;
	
	//Rect de l'écran
	SDL_Rect screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


	//Brouillard de guerre
	SDL_Rect rect_fogofwar;
	SDL_Color color_fogofwar = { 0, 0, 0, 100 };

	//C'est dans le nom connard
	Input input;

	//Pareil boloss crépu
	bool rightclick = false;
	bool leftclick = false;
	bool leftclick_release = false;
	bool touche_tab = false;

	//coordonées de la souris a envoyer aux fonctions (seulement pour moveunit pour l'instant)
	int mousexy[2];
	int startmousexy[2];

	//Sert a dessiner le carré de séléction
	SDL_Rect mouserect;
	bool drawselect = false;

	//de la merde 
	bool clickcube2 = false;

	//skills
	std::vector<int> desskills;	

	desskills.push_back(1);
	desskills.push_back(0);
	desskills.push_back(2);
	desskills.push_back(3);
	desskills.push_back(4);
	desskills.push_back(5);//Vecteur de numéros de skills correspondant à la liste de skills d'une unitée à envoyer à une Unit
	//desskills.push_back(1);								
	//desskills.push_back(2);								
	//desskills.push_back(3);								
	int skill = 0;										//Le skill utilisé à envoyer à use_skill()
	bool selectedskill = false;							
	bool selectedskill_mouse = false;					//Si la souris doit être en mode skill
	std::vector<Skills> active_skills;					//Vecteur des skills actifs créés au lancement et détruits à la fin de l'effet
	int skills_mousexy[2];
	bool skill_no_reset = false;

	int priorityunit_index = 0;
	//units.clear();
	//int unselected = 0;
	bool unitselected = false;
	bool allyselected = false;
	bool selection = false;		//Sert a voir si on vient de sélectionner
	bool unitpriority = false;
	//Hud
	bool hud_leftclick = false;
	bool hud_noselect = false;

	//Start up SDL and create window
	if (init() == -1)
	{
		printf("Failed to initialize !\n");
	}
	else
	{
		//Load media
		if (!gFPSTextTexture.setFont("Xenotron.ttf", 28))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 20, 255 };

			////Activer l'alpha
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

			//The frames per second timer
			LTimer fpsTimer;

			//The frames per second cap timer
			LTimer capTimer;

			//In memory text stream
			std::stringstream timeText;

			while (!game_game_quit)
			{
				Sprite *sprite_doge = new Sprite(gRenderer, "dogesimple.jpg", 100, 100, 800, 500, &hudposx, &hudposy);

				Sprite *sprite_boutongo = new Sprite(gRenderer, "bouton_go.jpg", 400, 300, 100, 100, &hudposx, &hudposy);
				Bouton *bouton_golol = new Bouton(sprite_boutongo, 1);
				menu_vector_boutons.push_back(bouton_golol);

				while (!menu_finish)
				{

					capTimer.start();

					//
					// Gestion des inputs
					//
					input.Update();


					//
					//Actions du click gauche avec impossibilitée de laisser appuyer
					//
					if (input.MouseButton(SDL_BUTTON_LEFT) && leftclick == false)
					{
						leftclick = true;
						startmousexy[0] = input.MouseX();
						startmousexy[1] = input.MouseY();
					}
					//
					//Relâchement du clic gauche
					//
					if (input.MouseButton(SDL_BUTTON_LEFT) == false)
					{
						if (leftclick)
						{
							leftclick_release = true;
						}
						leftclick = false;
					}


					if (leftclick == true && selectedskill_mouse == false)
					{
						mouserect.x = startmousexy[0];
						mouserect.y = startmousexy[1];
						mouserect.w = input.MouseX() - startmousexy[0];
						mouserect.h = input.MouseY() - startmousexy[1];

						for (auto &aze : menu_vector_boutons)
						{
							if (mouserect.x < aze->bouton_getsprite()->GetPosition().x + aze->bouton_getsprite()->GetWidth() && mouserect.x > aze->bouton_getsprite()->GetPosition().x)
							{
								if (mouserect.y < aze->bouton_getsprite()->GetPosition().y + aze->bouton_getsprite()->GetHeight() && mouserect.y + mouserect.h > aze->bouton_getsprite()->GetPosition().y)
								{
									aze->bouton_setmouseover(true);
									if (leftclick)
									{
										aze->bouton_setclick(true);
									}
								}
								else if (mouserect.y > aze->bouton_getsprite()->GetPosition().y && mouserect.y + mouserect.h < aze->bouton_getsprite()->GetPosition().y + aze->bouton_getsprite()->GetHeight())
								{
									aze->bouton_setmouseover(true);
									if (leftclick)
									{
										aze->bouton_setclick(true);
									}
								}
								else
								{
									aze->bouton_setmouseover(false);
									aze->bouton_setclick(false);
								}
							}
							else if (mouserect.x < aze->bouton_getsprite()->GetPosition().x + aze->bouton_getsprite()->GetWidth() && mouserect.x + mouserect.w > aze->bouton_getsprite()->GetPosition().x)
							{
								if (mouserect.y < mouserect.y + mouserect.h)
								{
									if (mouserect.y < aze->bouton_getsprite()->GetPosition().y + aze->bouton_getsprite()->GetHeight() && mouserect.y + mouserect.h > aze->bouton_getsprite()->GetPosition().y)
									{
										aze->bouton_setmouseover(true);
										if (leftclick)
										{
											aze->bouton_setclick(true);
										}
									}
									else
									{
										aze->bouton_setmouseover(false);
										aze->bouton_setclick(false);
									}
								}
								else if (mouserect.y > mouserect.y + mouserect.h)
								{
									if (mouserect.y > aze->bouton_getsprite()->GetPosition().y && mouserect.y + mouserect.h < aze->bouton_getsprite()->GetPosition().y + aze->bouton_getsprite()->GetHeight())
									{
										aze->bouton_setmouseover(true);
										if (leftclick)
										{
											aze->bouton_setclick(true);
										}
									}
									else
									{
										aze->bouton_setmouseover(false);
										aze->bouton_setclick(false);
									}
								}
							}
							else if (mouserect.x > aze->bouton_getsprite()->GetPosition().x && mouserect.x + mouserect.w < aze->bouton_getsprite()->GetPosition().x + aze->bouton_getsprite()->GetWidth())
							{
								if (mouserect.y < mouserect.y + mouserect.h)
								{
									if (mouserect.y < aze->bouton_getsprite()->GetPosition().y + aze->bouton_getsprite()->GetHeight() && mouserect.y + mouserect.h > aze->bouton_getsprite()->GetPosition().y)
									{
										aze->bouton_setmouseover(true);
										if (leftclick)
										{
											aze->bouton_setclick(true);
										}
									}
									else
									{
										aze->bouton_setmouseover(false);
										aze->bouton_setclick(false);
									}
								}
								else if (mouserect.y > mouserect.y + mouserect.h)
								{
									if (mouserect.y > aze->bouton_getsprite()->GetPosition().y && mouserect.y + mouserect.h < aze->bouton_getsprite()->GetPosition().y + aze->bouton_getsprite()->GetHeight())
									{
										aze->bouton_setmouseover(true);
										if (leftclick)
										{
											aze->bouton_setclick(true);
										}
									}
									else
									{
										aze->bouton_setmouseover(false);
										aze->bouton_setclick(false);
									}
								}
							}
							else
							{
								aze->bouton_setmouseover(false);
								aze->bouton_setclick(false);
							}
						}
					}

					for (auto &aze : menu_vector_boutons)
					{
						if (leftclick_release)
						{
							if (aze->bouton_getclick())
							{
								menu_action = aze->bouton_getaction();
							}
						}
					}


					switch (menu_action)
					{
					case 0:
						printf("BITE");
						break;
					case 1:
						menu_finish = true;
						break;
					default:
						printf("MESCOUEILSLSEILSEFL");
						break;
					}

					//		//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
					SDL_RenderClear(gRenderer);



					if (!menu_finish)
					{
						sprite_boutongo->Draw();
					}
					else
					{
						for (auto &aze : menu_vector_boutons)
						{
							aze->~Bouton();
						}
						sprite_doge->Draw();
						menu_stringstream_loadingtext.str("");
						menu_stringstream_loadingtext << "Wow \n Such chargement \n Very long \n Many variables" ;

						menu_string_loadingtext = menu_stringstream_loadingtext.str().c_str();
						//Render text
						if (!gFPSTextTexture.loadFromRenderedText(gRenderer, menu_textsurface, menu_string_loadingtext, textColor))
						{
							printf("Unable to render FPS texture!\n");
						}
						//Render textures
						gFPSTextTexture.render(gRenderer, 200, 600);
					}

					//
					//Afficher le tout
					//
					SDL_RenderPresent(gRenderer);

					//If frame finished early
					frameTicks = capTimer.getTicks();
					if (frameTicks < SCREEN_TICK_PER_FRAME)
					{
						//Wait remaining time
						SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
					}

				}
				////////////////////////
				//Fin de la boucle menu


				//
				//Chargement des sprites
				//
				Sprite *defaultsprite = new Sprite(gRenderer, "herbe.jpg", 100, 100, 100, 100, &CameraX, &CameraY);

				Sprite *defaulthudsprite = new Sprite(gRenderer, "herbe.jpg", 300, 660, 100, 100, &hudposx, &hudposy);

				Sprite *bobombe = new Sprite(gRenderer, "animunit.bmp", 100, 100, 100, 100, &CameraX, &CameraY);
				bobombe->SetUpAnimation(3, 3);
				Sprite *bobombehud = new Sprite(gRenderer, "animunit.bmp", 300, 660, 100, 100, &hudposx, &hudposy);

				Sprite *unit2 = new Sprite(gRenderer, "animunit.bmp", 100, 100, 10, 100, &CameraX, &CameraY);
				unit2->SetUpAnimation(3, 3);
				Sprite *hud3 = new Sprite(gRenderer, "hud.bmp", -2, 590, 1026, 180, &hudposx, &hudposy);

				Sprite *hud2 = new Sprite(gRenderer, "bombe1.bmp", 400, 400, 200, 200, &CameraX, &CameraY);

				Sprite *rog1 = new Sprite(gRenderer, "bombe1.bmp", 400, 400, 200, 200, &CameraX, &CameraY);

				Sprite *rog2 = new Sprite(gRenderer, "bombe1.bmp", 400, 400, 200, 200, &CameraX, &CameraY);

				Sprite *rog3 = new Sprite(gRenderer, "bombe1.bmp", 400, 400, 200, 200, &CameraX, &CameraY);

				Sprite *rog4 = new Sprite(gRenderer, "bombe1.bmp", 400, 400, 200, 200, &CameraX, &CameraY);

				Sprite *rog5 = new Sprite(gRenderer, "bombe1.bmp", 400, 400, 200, 200, &CameraX, &CameraY);

				Sprite *rog6 = new Sprite(gRenderer, "bombe1.bmp", 400, 400, 200, 200, &CameraX, &CameraY);

				Sprite *hudhud2 = new Sprite(gRenderer, "bombe1.bmp", 300, 660, 100, 100, &hudposx, &hudposy);

				Sprite *minimap = new Sprite(gRenderer, "herbe.jpg", 0, 0, 200, 200, &hudposx, &hudposy);

				Sprite *zerglingsprite = new Sprite(gRenderer, "zergling.jpg", 400, 200, 100, 100, &CameraX, &CameraY);

				Sprite *zerglinghudsprite = new Sprite(gRenderer, "zergling.jpg", 300, 660, 100, 100, &hudposx, &hudposy);

				//Sprites de la souris
				Sprite *mouse_sprite = new Sprite(gRenderer, "cursor.bmp", 0, 0, 30, 30, &hudposx, &hudposy);

				Sprite *mouse_sprite_skill = new Sprite(gRenderer, "crosshair.bmp", 0, 0, 30, 30, &hudposx, &hudposy);


				//Sprite du chemin des unités
				Sprite* pathsprite = new Sprite(gRenderer, "path_green_animated_highquality.bmp", 0, 0, 30, 10, &hudposx, &hudposy);
				pathsprite->SetUpAnimation(1, 3);

				Sprite* destination_circle = new Sprite(gRenderer, "destination_circle.png", 0, 0, 20, 20, &CameraX, &CameraY);
				destination_circle->SetUpAnimation(8, 1);

				Sprite* destination_circle_skills = new Sprite(gRenderer, "destination_circle_skills.png", 0, 0, 20, 20, &CameraX, &CameraY);
				destination_circle_skills->SetUpAnimation(8, 1);

				//
				//Chargement des unités
				//
				Unit *defaultunit = new Unit(defaultsprite, "default", 0, 0, 0, 0, 0, 0, desskills, defaulthudsprite, 1);

				Unit *bob = new Unit(bobombe, "bob", 100, 50, 10, 1, 2, 2, desskills, bobombehud, 0);
				units.push_back(bob);
				bob->setanim(true);
				bob->setpath_sprite(pathsprite);
		//		bob->skills_pushback(1);
		//		bob->skills_pushback(0);
		//		bob->skills_pushback(2);
		//		bob->skills_pushback(3);
		//		bob->skills_pushback(4);
		//		bob->skills_pushback(5);
				//bob->setanimrow_run_right(0);
				//bob->setanimrow_stand_1(1);
				//bob->setanimrow_skill_1(2);

				Unit *roger = new Unit(hud2, "roger", 200, 38, 50, 1, 2, 2, desskills, hudhud2, 0);
				units.push_back(roger);
				roger->setpath_sprite(pathsprite);
		//		roger->skills_pushback(1);
		//		roger->skills_pushback(0);
		//		roger->skills_pushback(2);
		//		roger->skills_pushback(3);
		//		roger->skills_pushback(4);
		//		roger->skills_pushback(5);

				//Unit *roge = new Unit(rog1, "roger", 200, 38, 20, 1, 2, 2, desskills, hudhud2, 0);
				//units.push_back(roge);
				//
				//Unit *rog = new Unit(rog2, "roger", 200, 38, 20, 1, 2, 2, desskills, hudhud2, 0);
				//units.push_back(rog);
				//
				//Unit *ro = new Unit(rog3, "roger", 200, 38, 20, 1, 2, 2, desskills, hudhud2, 0);
				//units.push_back(ro);
				//
				//Unit *rogr = new Unit(rog4, "roger", 200, 38, 20, 1, 2, 2, desskills, hudhud2, 0);
				//units.push_back(rogr);
				//
				//Unit *ror = new Unit(rog5, "roger", 200, 38, 20, 1, 2, 2, desskills, hudhud2, 0);
				//units.push_back(ror);
				//
				//Unit *roer = new Unit(rog6, "roger", 200, 38, 20, 1, 2, 2, desskills, hudhud2, 0);
				//units.push_back(roer);
				//
				Unit *zergling = new Unit(zerglingsprite, "zergling", 100, 100, 20, 1, 2, 2, desskills, zerglinghudsprite, 2);
				units.push_back(zergling);


				Unit* priorityunit = defaultunit;
				//
				//faut vraiment que j'explique tout?
				//
				Map *carte = new Map(gRenderer, "lazy.map", &CameraX, &CameraY);
				std::vector<Sprite*> tiles = carte->getTiles();
				carte->cleartiles();


				//
				//gnagnagna hud
				//
				Hud *hud = new Hud(gRenderer, hud3, bob, minimap);
				hud->setunit(defaultunit);

				//Start counting frames per second
				fpsTimer.start();

				//Remplissage des vecteurs d'unités par type
				// TODO : Faire une variable pour ne tester que les unités affichier à l'écran 
				for (auto &aze : units) // access by reference to avoid copying
				{
					if (aze->getfriendneutralfoe() == 0)
					{
						allyunits.push_back(aze);
					}
					if (aze->getfriendneutralfoe() == 1)
					{
						neutralunits.push_back(aze);
					}
					if (aze->getfriendneutralfoe() == 2)
					{
						ennemyunits.push_back(aze);
					}
				}

				//Efface le curseur windows
				SDL_ShowCursor(SDL_DISABLE);

				CameraX = 100;
				CameraY = 100;


				//
				//Boucle principale
				//
				while (!quit && !game_game_quit)
				{

					// TODO FAIRE UNE CLASSE CAMERA POUR CEtte fonction

					//	if (countedFrames > 120)
					//	{
					//		countedFrames = 0;
					//		fpsTimer.start();
					//	}

					//Start cap timer
					capTimer.start();

					//
					// Gestion des inputs
					//
					input.Update();
					leftclick_release = false;

					if (input.getTouche(SDL_SCANCODE_ESCAPE))
					{
						game_game_quit = true;
					}

					//
					//Deplacements de la camera
					//
					//au clavier
					if (input.getTouche(SDL_SCANCODE_UP))
					{
						CameraY += 2;
					}if (input.getTouche(SDL_SCANCODE_DOWN))
					{
						CameraY -= 2;
					}if (input.getTouche(SDL_SCANCODE_LEFT))
					{
						CameraX += 2;
					}if (input.getTouche(SDL_SCANCODE_RIGHT))
					{
						CameraX -= 2;
					}

					//a la souris et max camera
					if (input.MouseX() < 100)
					{
						CameraX += 2;
						if (CameraX > 1000)
						{
							CameraX = 1000;
						}

					}if (input.MouseX() > SCREEN_WIDTH - 100)
					{
						CameraX -= 2;

						if (CameraX < -1000)
						{
							CameraX = -1000;
						}
					}if (input.MouseY() < 100)
					{
						CameraY += 2;
						if (CameraY > 400)
						{
							CameraY = 400;
						}
					}if (input.MouseY() > SCREEN_HEIGHT - 100)
					{
						CameraY -= 2;
						if (CameraY < -1000)
						{
							CameraY = -1000;
						}
					}

					//Position du brouillard de guerre
					//		map_rect.x = -200 + CameraX;
					//		map_rect.y = -200 + CameraY;

					////////if (skills_mousexy[1] > 600 && selection == false) ///ATEJ
					////////{
					////////	if (leftclick)
					////////	{
					////////		hud_leftclick = true;
					////////	}
					////////	else
					////////	{
					////////		hud_leftclick = false;
					////////	}
					////////}



					//
					//Actions du click gauche avec impossibilitée de laisser appuyer
					//
					if (input.MouseButton(SDL_BUTTON_LEFT) && leftclick == false)
					{
						leftclick = true;
						startmousexy[0] = input.MouseX();
						startmousexy[1] = input.MouseY();
						if (startmousexy[1] > 600)
						{
							hud_leftclick = true;
						}

						/*
						for (auto &aze : units) // access by reference to avoid copying
						{
						if (input.MouseX() > aze->getsprite()->GetPosition().x && input.MouseX() < aze->getsprite()->GetPosition().x + aze->getsprite()->GetWidth() && input.MouseY() > aze->getsprite()->GetPosition().y && input.MouseY() < aze->getsprite()->GetPosition().y + aze->getsprite()->GetHeight())
						{
						aze->setselect(true);
						}
						else
						{
						aze->setselect(false);
						}
						}*/


						//!!!!!!!!!!!!!!!!!!!
						//!!!!!!!!!!!!!!!!
						//
						//Faudra pouvoir detruire les skills lancés quand reset
						// eventuellement faire le active_skills.push_back au moment du action
						//conclusion : C CHO
						//
						if ((selectedskill == true) && (hud_leftclick == false))
						{
							selectedskill = false;
							skills_mousexy[0] = input.MouseX() - CameraX;
							skills_mousexy[1] = input.MouseY() - CameraY;

							for (auto &aze : allyunits)
							{
								if (aze->getselectedskill())
								{
									//skill_no_reset = false;
									if (input.getTouche(SDL_SCANCODE_LSHIFT) == false)
									{										
										aze->action_clearlist();
										aze->setreset_action();
										aze->mousexylist_clear();
										aze->setaction_over(true);										
									}
									aze->action_pushback(aze->skills_at(aze->getusedskill()));
									aze->mousexy_pushback(skills_mousexy[0], skills_mousexy[1]);
									aze->setselectedskill(false);
								}
							}
						}
					}

					//
					//Actions du clic gauche en laissant appuyer(séléction)
					//
					if ((leftclick == true) && (selectedskill_mouse == false))
					{
						if (hud_leftclick == false)
						{
							mouserect.x = startmousexy[0];
							mouserect.y = startmousexy[1];
							mouserect.w = input.MouseX() - startmousexy[0];
							mouserect.h = input.MouseY() - startmousexy[1];
							unitselected = false;
							if (input.getTouche(SDL_SCANCODE_LSHIFT) == false)
							{
								allyselected = false;
							}
							selection = true;
							drawselect = true;
							for (auto &aze : units)
							{
								if (mouserect.x < aze->getsprite()->GetPosition().x + aze->getsprite()->GetWidth() && mouserect.x > aze->getsprite()->GetPosition().x)
								{
									if (mouserect.y < aze->getsprite()->GetPosition().y + aze->getsprite()->GetHeight() && mouserect.y + mouserect.h > aze->getsprite()->GetPosition().y)
									{
										unitselected = true;
										aze->setpre_select(true);
										if (aze->getfriendneutralfoe() == 0)
										{
											allyselected = true;
										}
									}
									else if (mouserect.y > aze->getsprite()->GetPosition().y && mouserect.y + mouserect.h < aze->getsprite()->GetPosition().y + aze->getsprite()->GetHeight())
									{
										unitselected = true;
										aze->setpre_select(true);
										if (aze->getfriendneutralfoe() == 0)
										{
											allyselected = true;
										}
									}
									else
									{
										aze->setpre_select(false);
									}
								}
								else if (mouserect.x < aze->getsprite()->GetPosition().x + aze->getsprite()->GetWidth() && mouserect.x + mouserect.w > aze->getsprite()->GetPosition().x)
								{
									if (mouserect.y < mouserect.y + mouserect.h)
									{
										if (mouserect.y < aze->getsprite()->GetPosition().y + aze->getsprite()->GetHeight() && mouserect.y + mouserect.h > aze->getsprite()->GetPosition().y)
										{
											unitselected = true;
											aze->setpre_select(true);
											if (aze->getfriendneutralfoe() == 0)
											{
												allyselected = true;
											}
										}
										else
										{
											aze->setpre_select(false);
										}
									}
									else if (mouserect.y > mouserect.y + mouserect.h)
									{
										if (mouserect.y > aze->getsprite()->GetPosition().y && mouserect.y + mouserect.h < aze->getsprite()->GetPosition().y + aze->getsprite()->GetHeight())
										{
											unitselected = true;
											aze->setpre_select(true);
											if (aze->getfriendneutralfoe() == 0)
											{
												allyselected = true;
											}
										}
										else
										{
											aze->setpre_select(false);
										}
									}
								}
								else if (mouserect.x > aze->getsprite()->GetPosition().x && mouserect.x + mouserect.w < aze->getsprite()->GetPosition().x + aze->getsprite()->GetWidth())
								{
									if (mouserect.y < mouserect.y + mouserect.h)
									{
										if (mouserect.y < aze->getsprite()->GetPosition().y + aze->getsprite()->GetHeight() && mouserect.y + mouserect.h > aze->getsprite()->GetPosition().y)
										{
											unitselected = true;
											aze->setpre_select(true);
											if (aze->getfriendneutralfoe() == 0)
											{
												allyselected = true;
											}
										}
										else
										{
											aze->setpre_select(false);
										}
									}
									else if (mouserect.y > mouserect.y + mouserect.h)
									{
										if (mouserect.y > aze->getsprite()->GetPosition().y && mouserect.y + mouserect.h < aze->getsprite()->GetPosition().y + aze->getsprite()->GetHeight())
										{
											unitselected = true;
											aze->setpre_select(true);
											if (aze->getfriendneutralfoe() == 0)
											{
												allyselected = true;
											}
										}
										else
										{
											aze->setpre_select(false);
										}
									}
								}
								else
								{
									aze->setpre_select(false);
								}
							}
						}
					}
					//
					//Relâchement du clic gauche
					//
					if (input.MouseButton(SDL_BUTTON_LEFT) == false)
					{
						if (leftclick)
						{
							leftclick_release = true;
						}

						leftclick = false;
						hud_leftclick = false;

						if (!selectedskill)
						{
							selectedskill_mouse = false;
						}
					}

					//
					//Actions du clic droit(déplacements)
					//
					if (input.MouseButton(SDL_BUTTON_RIGHT) && rightclick == false) // deuxième condition pour empècher de laisse appuyer
					{
						rightclick = true;

						if (selectedskill)
						{
							selectedskill = false;
							priorityunit->setselectedskill(false);
						}
						else
						{
							//Si l'unité alliée est selectionée, confirmer le deplacement
							for (auto &aze : allyunits)
							{
								if (aze->getselect())
								{
									if (input.getTouche(SDL_SCANCODE_LSHIFT) == false)
									{
										//if (aze->action_getlist().size() == 0)
										//{
										//	aze->setaction_over(true);
										//}
										aze->action_clearlist();
										aze->setreset_action();
										aze->mousexylist_clear();
										aze->setaction_over(true);
										//aze->setaction_over(true);
										//aze->skills_bool_clear();
										//aze->setskill_number(0);
										//aze->setskill_over(true);
										//aze->setnumber_of_skill_used(0);
										//aze->setskill_reset(true);
									}

									aze->action_pushback(1);
									//aze->setaction_to_do(1);
									//aze->setmove(true);
									mousexy[0] = input.MouseX() - CameraX;
									mousexy[1] = input.MouseY() - CameraY;

									//aze->get_mousexylist().push_back({ mousexy[0], mousexy[1] });
									//aze->get_mousexylist().push_back({1,1 });
									aze->mousexy_pushback(mousexy[0], mousexy[1]);
									//aze->setaction_start(true);

									//aze->setmousexy(mousexy);
								}
							}
						}

					}

					//
					//Relâchement du clic droit
					//
					if (input.MouseButton(SDL_BUTTON_RIGHT) == false)
					{
						rightclick = false;
					}

					//
					//Deplacement et selection de toutes les units (final)
					//
					//for (auto &aze : units)
					//{
					//	if (aze->getmove())
					//	{
					//		aze->moveunit();
					//	}
					//	if (aze->getmoving())
					//	{
					//		aze->moveunit();
					//	}
					//}

					//
					//Actions des unités
					//
					for (auto &aze : units)
					{
						aze->action();
						if (aze->getskill_go())
						{
							active_skills.push_back(Skills(gRenderer, aze, NULL, aze->getskill_to_go(), &CameraX, &CameraY, aze->getmousex(), aze->getmousey()));
							aze->setskill_go(false);
						}
					}

					//
					//Mise a jour du hud et du vecteur d'unitées séléctionnées lorsque le click gauche est relaché
					//
					if (leftclick == false && selection == true)
					{
						//La préséléction se fait pendant la séléction avec clic gauche. Au relâchement on fait le test
						if (allyselected)
						{
							for (auto &aze : neutralunits)
							{
								aze->setpre_select(false);
							}
							for (auto &aze : ennemyunits)
							{
								aze->setpre_select(false);
							}
						}

						for (auto &aze : units)
						{
							if (aze->getpre_select())
							{
								aze->setselect(true);
							}
							else
							{
								if (input.getTouche(SDL_SCANCODE_LSHIFT) == false)
								{
									aze->setselect(false);
								}
							}
						}

						//Les unités séléctionnées sont placées dans le vecteur d'unités du hud
						hud->hudunitsclear();
						selectedallies.clear();
						selection = false;
						priorityunit_index = 0;
						for (auto &aze : units)
						{
							if (aze->getselect())
							{
								hud->setunit(aze);
								hud->addunit(aze);
							}
						}

						//Si aucune unité n'est selectionée
						if (hud->hudunitsempty())
						{
							hud->setunit(defaultunit);
						}


						//Remplissage du vecteur d'unités séléctionnées
						for (auto &aze : allyunits)
						{
							if (aze->getselect())
							{
								selectedallies.push_back(aze);
							}
						}
					}

					//Si tab est relâché
					if (input.getTouche(SDL_SCANCODE_TAB) == false)
					{
						touche_tab = false;
					}

					//Changement d'unité prioritaire avec TAB
					if (selectedallies.size() > 0)
					{
						if (input.getTouche(SDL_SCANCODE_TAB) && touche_tab == false)
						{
							priorityunit->setselectedskill(false);
							selectedskill = false;
							selectedskill_mouse = false;
							touche_tab = true;
							priorityunit->setpriority(false);
							if (priorityunit_index + 1 < selectedallies.size())
							{
								++priorityunit_index;
							}
							else
							{
								priorityunit_index = 0;
							}
						}
					}
					else
					{
						priorityunit = defaultunit;
					}

					//
					//Gestion des skills
					//
					if (allyselected)
					{
						if (selectedallies.size() > 0)
						{
							priorityunit = selectedallies[priorityunit_index];
							priorityunit->setpriority(true);
						}
						for (auto &aze : selectedallies)
						{
							if (aze->getstop_skill())
							{
								if (input.getTouche(SDL_SCANCODE_S) == false)
								{
									aze->setstop_skill(false);
									aze->setselectedskill(false);
								}
							}
							if (aze->gethold_skill())
							{
								if (input.getTouche(SDL_SCANCODE_H) == false)
								{
									aze->sethold_skill(false);
									aze->setselectedskill(false);
								}
							}
						}
						if (input.getTouche(SDL_SCANCODE_C))
						{
							for (auto &aze : selectedallies)
							{
								aze->setusedskill(0);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
								aze->setselectedskill(true);
							}
							selectedskill = true;
							selectedskill_mouse = true;
						}
						else if (input.getTouche(SDL_SCANCODE_S))
						{
							selectedskill = false;
							for (auto &aze : selectedallies)
							{
								aze->setusedskill(1);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
								if (input.getTouche(SDL_SCANCODE_LSHIFT) == false)
								{
									aze->action_clearlist();
									aze->setreset_action();
									aze->mousexylist_clear();
									aze->setaction_over(true);
								}
								aze->action_pushback(0);
								aze->mousexy_pushback(0, 0);
								aze->setselectedskill(true);
								aze->setstop_skill(true);
							}
						}
						else if (input.getTouche(SDL_SCANCODE_H))
						{
							for (auto &aze : selectedallies)
							{
								aze->setusedskill(2);
								if (input.getTouche(SDL_SCANCODE_LSHIFT) == false)
								{
									aze->action_clearlist();
									aze->setreset_action();
									aze->mousexylist_clear();
									aze->setaction_over(true);
								}
								aze->action_pushback(2);
								aze->mousexy_pushback(0, 0);
								aze->setselectedskill(true);
								aze->sethold_skill(true);
							}
						}
						else if (input.getTouche(SDL_SCANCODE_P))
						{
							for (auto &aze : selectedallies)
							{
								aze->setusedskill(3);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
								aze->setselectedskill(true);
							}
							selectedskill = true;
							selectedskill_mouse = true;
						}
						else if (input.getTouche(SDL_SCANCODE_A))
						{
							for (auto &aze : selectedallies)
							{
								aze->setusedskill(4);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
								aze->setselectedskill(true);
							}
							selectedskill = true;
							selectedskill_mouse = true;
						}
						else if (input.getTouche(SDL_SCANCODE_Q))
						{
							priorityunit->setusedskill(5);   // Pointe sur le vecteur de skill de l'unité, si on pointe trop loin crash du jeu					
							priorityunit->setselectedskill(true);
							selectedskill = true;
							selectedskill_mouse = true;
						}
					}



					printf("%d", active_skills.size());



					//Calculate and correct fps
					float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
					if (avgFPS > 2000000)
					{
						avgFPS = 0;
					}





					/////////////////////////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////////////////////////Affichage
					/////////////////////////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////////////////////////

					//		//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
					SDL_RenderClear(gRenderer);

					//
					//Je laisse ces deux là au cas où c'est plus opti mais a priori non.....hum...
					//
					/*for (i = 0; i < tiles.size(); i++)
					{
					tiles[i].Draw();
					printf("lol");
					}
					for (auto a = tiles.begin(); a != tiles.end(); a++)
					{
					Sprite->Draw();
					}*/

					//
					//Dessiner la map
					//
					for (auto &tile : tiles) // access by reference to avoid copying
					{
						tile->Draw();
					}

					//
					//Dessiner les unités
					//
					roger->getsprite()->Draw();

					//bob->getsprite()->PlayAnimation(0, 2, 0, 100);
					//bob->getsprite()->Draw();
					bob->draw();
					zergling->getsprite()->Draw();

					
					for (auto &ally : allyunits)
					{
						int i = 0;
						if (ally->getselect())
						{
							if (ally->action_size() > 0)
							{
								if (ally->getcurrent_action() == 1)
								{
									destination_circle->SetX(ally->getmousex() - destination_circle->GetWidth() / 2);
									destination_circle->SetY(ally->getmousey() - destination_circle->GetHeight() / 2);
									destination_circle->Draw();
								}

								for (auto &action : ally->action_getlist())
								{
									if (action == 1)
									{
										destination_circle->SetX(ally->mousex_at(i) - destination_circle->GetWidth() / 2);
										destination_circle->SetY(ally->mousey_at(i) - destination_circle->GetHeight() / 2);
										destination_circle->Draw();
									}
									else if (action >= 5)
									{
										destination_circle_skills->SetX(ally->mousex_at(i) - destination_circle_skills->GetWidth() / 2);
										destination_circle_skills->SetY(ally->mousey_at(i) - destination_circle->GetHeight() / 2);
										destination_circle_skills->Draw();
									}
									++i;
								}
							}
						}

					}
					destination_circle->PlayAnimation(0, 7, 0, 6, NULL);
					destination_circle_skills->PlayAnimation(0, 7, 0, 6, NULL);

					for (auto &aze : units) // access by reference to avoid copying
					{
						if (aze->getselect() || aze->getpre_select())
						{
							aze->drawlife(gRenderer, CameraX, CameraY);
							////////if (aze->getselectedskill())
							////////{
							////////	//dessiner le pré-skill
							////////}

						}
					}

					//
					//Gestion et dessins des skills
					//
					if (active_skills.size() > 0)
					{
						for (auto &aze : active_skills)
						{
							aze.use_skill();
							//aze.hand_grenade();
							//aze->use_skill();
							/*
							if (aze.getfinish_skill())
							{
							aze.~Skills();
							}
							*/
						}
						//for (auto &aze : allyunits)
						//{
						//	aze->setskill_reset(false);
						//}
						//skill_no_reset = false;

						//
						//Destruction des skills
						//

						auto i = std::begin(active_skills);

						while (i != std::end(active_skills))
						{
							// do some stuff.
							if (i->getfinish_skill())
							{

								//i->gethand_grenade_sprite()->~Sprite();
								i->~Skills();
								i = active_skills.erase(i);
							}
							else
							{
								++i;
							}
						}


					}

					//Dessiner le carré de sélection
					if (drawselect)
					{
						SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(gRenderer, &mouserect);
						SDL_SetRenderDrawColor(gRenderer, 0, 255, 100, 50);
						SDL_RenderFillRect(gRenderer, &mouserect);
						drawselect = false;
					}

					//Dessiner le Hud
					hud->gethudstyle()->Draw();
					hud->drawtext();
					hud->getminimap()->Draw();
					hud->drawminimap(units);
					hud->drawskills(priorityunit);

					//if (selection)
					//{
					//	hud_noselect = true;
					//}
					if (allyselected && hud_noselect == false)
					{
						skills_mousexy[0] = input.MouseX();
						skills_mousexy[1] = input.MouseY();
						if (selection == false)
						{
							if (hud->skill_mouse_actions(priorityunit, skills_mousexy[0], skills_mousexy[1], leftclick, leftclick_release, input.getTouche(SDL_SCANCODE_LSHIFT)))
							{
								selectedskill = true;
								selectedskill_mouse = true;
							}
						}
					}
					//hud_noselect = false;

					//Dessiner la souris

					if (selectedskill)
					{
						mouse_sprite_skill->SetX(input.MouseX() - 15);
						mouse_sprite_skill->SetY(input.MouseY() - 15);
						mouse_sprite_skill->Draw();
					}
					else
					{
						mouse_sprite->SetX(input.MouseX() - 6);
						mouse_sprite->SetY(input.MouseY());
						mouse_sprite->Draw();
					}

					
					//Set text to be rendered
					timeText.str("");
					timeText << "Average Frames Per Second (With Cap) " << avgFPS;

					//Render text
					if (!gFPSTextTexture.loadFromRenderedText(gRenderer, fpscount, timeText.str().c_str(), textColor))
					{
						printf("Unable to render FPS texture!\n");
					}


					//Render textures
					gFPSTextTexture.render(gRenderer, (SCREEN_WIDTH - gFPSTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gFPSTextTexture.getHeight()) / 16);


					//
					//Afficher le tout
					//
					SDL_RenderPresent(gRenderer);
					//glClearColor(0.0, 0.0, 0.0, 1.0);
					//SDL_GL_SwapWindow(gWindow);

					// +1 frame
					++countedFrames;


					//If frame finished early
					frameTicks = capTimer.getTicks();
					if (frameTicks < SCREEN_TICK_PER_FRAME)
					{
						//Wait remaining time
						SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
					}

				}
			}
		}
	}


	//SDL_DestroyTexture(hudtex);
	//Free resources and close SDL

	//Free global font
	TTF_CloseFont(gFPSTextTexture.getFont());
	//gFPSTextTexture.gFont = NULL;

	close();

	return 0;
}
