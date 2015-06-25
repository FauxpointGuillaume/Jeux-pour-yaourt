#include "stdafx.h"
/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

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

//Opengl context
//SDL_GLContext contexteOpenGL = NULL;

/*
//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

*/

//The application time based timer
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
/*
//Scene textures
LTexture gFPSTextTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
*/
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
/*
class Player
{

public:
	//Initializes variables
	Player();

	//Deallocates memory
	~Player();

	void drawplayer();

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;


};

*/
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


/*
bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	return success;
}
*/


void close()
{
	//Free loaded images
	gFPSTextTexture.free();


	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
/*
int Gl_init()
{
	// Initialisation de la SDL

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return -1;
	}


	// Version d'OpenGL

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


	// Double Buffer

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


	// Création de la fenêtre

	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (gWindow == 0)
	{
		std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return -1;
	}


	// Création du contexte OpenGL

	contexteOpenGL = SDL_GL_CreateContext(gWindow);

	if (contexteOpenGL == 0)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_DestroyWindow(gWindow);
		SDL_Quit();

		return -1;
	}

	// On initialise GLEW

	GLenum initialisationGLEW(glewInit());

	// Si l'initialisation a échouée :

	if (initialisationGLEW != GLEW_OK)
	{
		// On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)

		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;


		// On quitte la SDL
		SDL_GL_DeleteContext(contexteOpenGL);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();

		return -1;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return -1;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return -1;
	}


	return 1;
}
*/
int main(int argc, char* args[])
{
	float CameraX = 200;
	float CameraY = 300;
	float hudposx = 0;
	float hudposy = 0;


	//ColorKey des surfaces globales
	SDL_SetColorKey(surface_explosion_grenade, SDL_TRUE, SDL_MapRGB(surface_explosion_grenade->format, 0xFF, 0xFF, 0xFF));
	SDL_SetColorKey(surface_grenade, SDL_TRUE, SDL_MapRGB(surface_grenade->format, 0xFF, 0xFF, 0xFF));



	//On a tous besoin d'un i
	int i = 0;
	
	//Rect de l'écran
	SDL_Rect screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


	//Brouillard de guerre
	SDL_Rect rect_fogofwar;
	SDL_Color color_fogofwar = { 0, 0, 0, 100 };

	//Du brouillon pour le fogofwar
	//////////////////////////
	//surface_fogofwar = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	//SDL_Texture *texture_fogofwar;
	//SDL_Texture* auxtexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 500, 500);

	//////
//	SDL_Rect sourceRect;
//	SDL_Rect destRect;
//	
//	Uint32* destPixels;
//	Uint32* srcPixels;
//	Uint32* destPixel;
//	Uint32* srcPixel;
//	
//	unsigned char* destAlpha;
//	unsigned char* srcAlpha;
//	static bool keepFogRemoved = false;
	
	///////////////////////////// fin du brouillon


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
	std::vector<int> desskills;							//Vecteur de numéros de skills correspondant à la liste de skills d'une unitée à envoyer à une Unit
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
		if (!gFPSTextTexture.setFont("lazy.ttf", 28))
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

		//	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
		

	//		/////////////////
	//		//Opengl test
	//		glEnable(GL_TEXTURE_2D);
	//		glEnable(GL_BLEND);
	//
	//		glMatrixMode(GL_PROJECTION);
	//		glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
	//		glMatrixMode(GL_MODELVIEW);
	//
	//		glm::mat4 projection;
	//		glm::mat3 modelview;
	//
	//		projection = glm::perspective(70.0, (double)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0, 100.0);
	//		modelview = glm::mat3(1.0);
	//
	//
	//		float vertices[] = { -0.5, -0.5, 0.0, 0.5, 0.5, -0.5 };
	//		float coordTexture[] = { 0, 0, 1, 0, 1, 1,     // Triangle 1
	//			0, 0, 0, 1, 1, 1 };    // Triangle 2
	//
	//		SDL_Surface *imageSDL = IMG_Load("destination_circle_skills.png");
	//		if (imageSDL == 0)
	//		{
	//			printf("\nBTIEBTIEBTIEBTIEBTEITBBITEBITEBTIEB");
	//		}
	//		GLuint id;
	//
	//		glGenTextures(1, &id);
	//		glBindTexture(GL_TEXTURE_2D, id);
	//
	//		GLenum formatInterne(0);
	//		GLenum format(0);
	//
	//		if (imageSDL->format->BytesPerPixel == 3)
	//		{
	//			// Format interne
	//			formatInterne = GL_RGB;
	//			// Format
	//			if (imageSDL->format->Rmask == 0xff)
	//				format = GL_RGB;
	//			else
	//				format = GL_BGR;
	//		}
	//		// Détermination du format et du format interne pour les images à 4 composantes
	//		else if (imageSDL->format->BytesPerPixel == 4)
	//		{
	//			// Format interne
	//			formatInterne = GL_RGBA;
	//			// Format
	//			if (imageSDL->format->Rmask == 0xff)
	//				format = GL_RGBA;
	//			else
	//				format = GL_BGRA;
	//		}
	//		// Dans les autres cas, on arrête le chargement
	//		else
	//		{
	//			std::cout << "Erreur, format interne de l'image inconnu" << std::endl;
	//			SDL_FreeSurface(imageSDL);
	//			return false;
	//		}
	//
	//		//
	//		//
	//		//// Verrouillage
	//		//
	//		//
	//	//	glTexImage2D(GL_TEXTURE_2D, 0, 4 , imageSDL->w, imageSDL->h, 0, GL_RGBA , GL_UNSIGNED_BYTE, imageSDL->pixels);
	//		glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, imageSDL->w, imageSDL->h, 0, format, GL_UNSIGNED_BYTE, imageSDL->pixels);
	//	
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//
	//		//
	//		//// Déverrouillage
	//		//
	//		glBindTexture(GL_TEXTURE_2D, 0);
	//	
	//		SDL_FreeSurface(imageSDL);
	//
	//
	//		Shader shaderBasique("Shaders/basique_2D.vert", "Shaders/basique.frag");
	//		shaderBasique.charger();
	//		//
	//		/////////////////
	//		
	//		//glEnable(GL_TEXTURE_2D);
	//		//glShadeModel(GL_FLAT);
	//







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
			bob->skills_pushback(1);
			bob->skills_pushback(0);
			bob->skills_pushback(2);
			bob->skills_pushback(3);
			bob->skills_pushback(4);
			bob->skills_pushback(5);
			//bob->setanimrow_run_right(0);
			//bob->setanimrow_stand_1(1);
			//bob->setanimrow_skill_1(2);

			Unit *roger = new Unit(hud2, "roger", 200, 38, 50, 1, 2, 2, desskills, hudhud2, 0);
			units.push_back(roger);
			roger->setpath_sprite(pathsprite);
			roger->skills_pushback(1);
			roger->skills_pushback(0);
			roger->skills_pushback(2);
			roger->skills_pushback(3);
			roger->skills_pushback(4);
			roger->skills_pushback(5);

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



			//Taffage du fogofwar
			//
			//Sprite du brouillard de guerre
	//		Sprite *sprite_fogofwar_remove = new Sprite(gRenderer, "fogofwar_remove_circle.png", 300, 300, 100, 100, &CameraX, &CameraY);
	//
	//		SDL_Surface *surface_fogofwar_remove2 = IMG_Load("fogofwar_remove_circle.png");
	//
	//		SDL_Surface* surface_fogofwar_remove = SDL_ConvertSurfaceFormat(surface_fogofwar_remove2,
	//			SDL_PIXELFORMAT_ARGB8888,
	//			NULL);
	//
	//		Uint32* fogofwar_remove_pixels = (Uint32*)surface_fogofwar_remove->pixels;
	//
	//
	//		SDL_Rect map_rect = { 0, 0, carte->getsizex(), carte->getsizey() };
	//		//SDL_Rect fog_surface_rect
	//		SDL_Rect fogofwar_rectdeouf = { 100, 100, 200, 200 };
	//		
	//		//surface_fogofwar = SDL_CreateRGBSurface(0, map_rect.w, map_rect.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	//		surface_fogofwar2 = SDL_CreateRGBSurface(0, map_rect.w, map_rect.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	//		
	//
	//		SDL_Surface* surface_fogofwar = SDL_ConvertSurfaceFormat(surface_fogofwar2,
	//		SDL_PIXELFORMAT_ARGB8888,
	//			NULL);
	//
	//
	//
	//		void *fogofwar_pixels;
	//		int fogofwar_pitch;
	//		Uint32 transparent = SDL_MapRGBA(surface_fogofwar->format, 0, 0, 0, 0);
	//		Uint32* pixels32;
	//		int     pixelCount;
	//
	//		int lolx = 0;
	//		int loly = 0;
	//
	//		unsigned char* destAlpha;
	//		unsigned char* srcAlpha = 0;
	//
	//
	//
	//		Uint32 rgba_fogofwar = SDL_MapRGBA(surface_fogofwar->format, 0, 0, 0, 150);
	//
	//		Uint32 *fogofwar_pixel;
	//		Uint32 fogofwar_pixel2 = SDL_MapRGBA(surface_fogofwar->format, 0, 0, 0, 0);
	//
	//		Uint8* p;
	//		//SDL_Texture* texture_fogofwar = SDL_CreateTextureFromSurface(gRenderer, surface_fogofwar);
	//		//SDL_SetTextureBlendMode(texture_fogofwar, SDL_BLENDMODE_BLEND);
	//
	//		rect_fogofwar.w = carte->getsizex();
	//		rect_fogofwar.h = carte->getsizey();
	//
	//		//SDL_QueryTexture(texture_fogofwar, (Uint32*)0x16261804, NULL, &surface_fogofwar->w, &surface_fogofwar->h);
	//		SDL_Texture *texture_fogofwar = SDL_CreateTexture(gRenderer,
	//			SDL_PIXELFORMAT_ARGB8888,
	//			SDL_TEXTUREACCESS_STREAMING,
	//			surface_fogofwar->w,
	//			surface_fogofwar->h);
	//		SDL_SetTextureBlendMode(texture_fogofwar, SDL_BLENDMODE_BLEND);
	//
	//
	//		SDL_FillRect(surface_fogofwar, NULL, rgba_fogofwar);


//			SDL_LockTexture(texture_fogofwar, nullptr, reinterpret_cast<void **>(&fogofwar_pixels), &fogofwar_pitch);
//			SDL_memcpy(fogofwar_pixels, surface_fogofwar->pixels, surface_fogofwar->pitch*surface_fogofwar->h);
//			SDL_UnlockTexture(texture_fogofwar);
//


			//SDL_Surface* imageFomatted = SDL_ConvertSurfaceFormat(surface_fogofwar,
			//	SDL_PIXELFORMAT_RGBA8888,
			//	NULL);
			//
			//
			//SDL_Texture* Letexture = SDL_CreateTexture(gRenderer,
			//	SDL_PIXELFORMAT_RGBA8888,
			//	SDL_TEXTUREACCESS_STREAMING,
			//	imageFomatted->w, imageFomatted->h);
			//
			//
			//void* Lepixels = NULL;
			//int Lepitch = 0;
			//
			//int width = imageFomatted->w;
			//int height = imageFomatted->h;
			//
			//Uint32* pixels32 = (Uint32*)Lepixels;
			//int     pixelCount = (Lepitch / 4) * height;
			//
			//Uint32 colorKey = SDL_MapRGBA(imageFomatted->format, 255, 255, 255, 255);
			//Uint32 transparent = SDL_MapRGBA(imageFomatted->format, 0xFF, 0x00, 0xFF, 0x00);


			//SDL_FillRect(surface_fogofwar, NULL, rgba_fogofwar);
			//texture_fogofwar = SDL_CreateTextureFromSurface(gRenderer, surface_fogofwar);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





			//
			//
			//Fin du brouillon du fogofwar






			//
			//gnagnagna hud
			//
			Hud *hud = new Hud(gRenderer, hud3, bob, minimap);
			hud->setunit(defaultunit);

			//Start counting frames per second
			fpsTimer.start();

			




			//Remplissage des vecteurs d'unités par type
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
			while (!quit && !input.getTouche(SDL_SCANCODE_ESCAPE))
			{

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
					if (selectedskill && hud_leftclick == false)
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
									//aze->setaction_over(true);
									//aze->setaction_over(true);
									aze->action_clearlist();
									aze->setreset_action();
									aze->mousexylist_clear();
									aze->setaction_over(true);
									//aze->skills_bool_clear();
									//aze->setskill_number(0);
									//aze->setskill_over(true);
									//aze->setnumber_of_skill_used(0);
									//aze->setskill_reset(true);
									//skill_no_reset = true;
								}
								//aze->increment_number_of_skill_used();
								//aze->skills_bool_pushback(0);
								//aze->action_pushback(aze->getskills().at(aze->getusedskill()));
								aze->action_pushback(aze->skills_at(aze->getusedskill()));
								aze->mousexy_pushback(skills_mousexy[0], skills_mousexy[1]);
								//active_skills.push_back(Skills(gRenderer, aze, NULL, aze->getskills().at(aze->getusedskill()), &CameraX, &CameraY, skills_mousexy));							
								aze->setselectedskill(false);
								//aze->setaction_start(true);
							}
						}
					}
				}

				//
				//Actions du clic gauche en laissant appuyer(séléction)
				//
				if (leftclick == true && selectedskill_mouse == false)
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
					/*
					int i = 0;
					for (auto &aze : selectedunits)
					{
					if (aze->getselect() == false)
					{
					selectedunits.erase(selectedunits.begin() + i);
					}
					++i;
					}*/

					/*
					auto i = std::begin(active_skills);

					while (i != std::end(active_skills)) {
					// do some stuff.
					if (i->getfinish_skill())
					{
					i = selectedunits.erase(i);
					}
					else
					{
					++i;
					}
					}*/

					if (!selectedskill)
					{
						selectedskill_mouse = false;
					}
					//if (selection)
					//{
					//	if (allyselected)
					//	{
					//		for (auto &aze : neutralunits)
					//		{
					//			aze->setpre_select(false);
					//		}
					//		for (auto &aze : ennemyunits)
					//		{
					//			aze->setpre_select(false);
					//		}
					//	}
					//
					//	for (auto &aze : units)
					//	{
					//		if (aze->getpre_select())
					//		{
					//			aze->setselect(true);
					//		}
					//		else
					//		{
					//			aze->setselect(false);
					//		}
					//	}
					//}
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

				/*
				for (auto &aze : units) // access by reference to avoid copying
				{
				aze->getsprite()->Draw();
				}
				*/




/////////////////////////Brouillon 3 du fogofwar
/////////////////////////
/////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





				//SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				//SDL_RenderDrawRect(gRenderer, &map_rect);
				//SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
				//SDL_RenderFillRect(gRenderer, &map_rect);



				////////////change the rendering target
				//////////
				//////////SDL_SetTextureBlendMode(auxtexture, SDL_BLENDMODE_BLEND);
				//////////SDL_SetRenderTarget(gRenderer, auxtexture);
				//////////
				////////////render what we want
				//////////roger->draw(); //render my class triangle e.g
				//////////
				//////////
				////////////change the target back to the default and then render the aux
				//////////
				//////////SDL_SetRenderTarget(gRenderer, NULL); //NULL SETS TO DEFAULT
				//////////bob->draw();
				//////////SDL_DestroyTexture(auxtexture);




				////////////////////////////////////////////////////////
				//SDL_FillRect(surface_fogofwar, NULL, rgba_fogofwar);
				//
				//sourceRect = { 0 + (int)CameraX, 0 + (int)CameraY, surface_fogofwar_remove->w, surface_fogofwar_remove->h };
				//destRect = { 300 + (int)CameraX, 300 + (int)CameraY, surface_fogofwar_remove->w, surface_fogofwar_remove->h };
				//
				//	// Make sure our rects stays within bounds
				//	if (destRect.x < 0)
				//	{
				//		sourceRect.x -= destRect.x; // remove the pixels outside of the surface
				//		sourceRect.w -= sourceRect.x; // shrink to the surface, not to offset fog
				//		destRect.x = 0;
				//		destRect.w -= sourceRect.x; // shrink the width to stay within bounds
				//	}
				//	if (destRect.y < 0)
				//	{
				//		sourceRect.y -= destRect.y; // remove the pixels outside
				//		sourceRect.h -= sourceRect.y; // shrink to the surface, not to offset fog
				//		destRect.y = 0;
				//		destRect.h -= sourceRect.y; // shrink the height to stay within bounds
				//	}
				//	
				//	int xDistanceFromEdge = (destRect.x + destRect.w) - surface_fogofwar->w;
				//	if (xDistanceFromEdge > 0) // we're busting
				//	{
				//		sourceRect.w -= xDistanceFromEdge;
				//		destRect.w -= xDistanceFromEdge;
				//	}
				//	int yDistanceFromEdge = (destRect.y + destRect.h) - surface_fogofwar->h;
				//	if (yDistanceFromEdge > 0) // we're busting
				//	{
				//		sourceRect.h -= yDistanceFromEdge;
				//		destRect.h -= yDistanceFromEdge;
				//	}
				//
				//	SDL_LockSurface(surface_fogofwar);
				//
				//	destPixels = (Uint32*)surface_fogofwar->pixels;
				//	srcPixels = (Uint32*)surface_fogofwar_remove->pixels;
				//
				//
				//	for (int y = 0; y < destRect.w; ++y)
				//	{
				//		for (int x = 0; x < destRect.h; ++x)
				//		{
				//			destPixel = destPixels + (y + destRect.y) * surface_fogofwar->w + destRect.x + x;
				//			srcPixel = srcPixels + (y + sourceRect.y) * surface_fogofwar_remove->w + sourceRect.x + x;
				//
				//			destAlpha = (unsigned char*)destPixel + 3; // fetch alpha channel
				//			srcAlpha = (unsigned char*)srcPixel + 3; // fetch alpha channel
				//			if (keepFogRemoved == true && *srcAlpha > 0)
				//			{
				//				continue; // skip this pixel
				//			}
				//
				//			*destAlpha = *srcAlpha;
				//		}
				//	}
				//
				//	SDL_UnlockSurface(surface_fogofwar);


				//SDL_LockSurface(surface_fogofwar);
				//
				//for (int y = bob->getsprite()->GetY(); y < bob->getsprite()->GetY()+ 200; ++y)
				//{
				//	for (int x = bob->getsprite()->GetX(); x < bob->getsprite()->GetX()+200; ++x)
				//	{
				//		if (x >= 0 && y >= 0)
				//		{
				//			p = (Uint8 *)surface_fogofwar->pixels + y * surface_fogofwar->pitch + x * surface_fogofwar->format->BytesPerPixel;
				//			//fogofwar_pixel = (Uint32*)surface_fogofwar->pixels + y * surface_fogofwar->pitch + x;
				//			*(Uint32 *)p = fogofwar_pixel2;
				//		}
				//	}
				//}
				//
				//SDL_UnlockSurface(surface_fogofwar);



				/////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////



			//	SDL_FillRect(surface_fogofwar, NULL, rgba_fogofwar);


			//	rect_fogofwar.x = 0 + (int)CameraX;
			//	rect_fogofwar.y = 0 + (int)CameraY;
				//SDL_LockSurface(surface_fogofwar);

			//	for (int y = bob->getsprite()->GetY(); y < bob->getsprite()->GetY() + 200; ++y)
			//	{
			//		for (int x = bob->getsprite()->GetX(); x < bob->getsprite()->GetX() + 200; ++x)
			//		{
			//			if (x >= 0 && y >= 0)
			//			{
			//				p = (Uint8 *)surface_fogofwar->pixels + y * surface_fogofwar->pitch + x * surface_fogofwar->format->BytesPerPixel;
			//				//fogofwar_pixel = (Uint32*)surface_fogofwar->pixels + y * surface_fogofwar->pitch + x;
			//				*(Uint32 *)p = fogofwar_pixel2;
			//			}
			//		}
			//	}

				//SDL_UnlockSurface(surface_fogofwar);

				//texture_fogofwar = SDL_CreateTextureFromSurface(gRenderer, surface_fogofwar);

				//SDL_LockTexture(texture_fogofwar, NULL, &pixels, &pitch);
				//			
				//SDL_LockTexture(texture_fogofwar, nullptr, reinterpret_cast<void **>(&fogofwar_pixels), &fogofwar_pitch);
//				SDL_LockTexture(texture_fogofwar, nullptr, static_cast<void **>(&fogofwar_pixels), &fogofwar_pitch);
//				//SDL_LockTexture(texture_fogofwar, NULL,&fogofwar_pixels, &fogofwar_pitch);
//				for (int y = bob->getsprite()->GetY(); y < bob->getsprite()->GetY() + 500; ++y)
//				{
//					++loly;
//					for (int x = bob->getsprite()->GetX(); x < bob->getsprite()->GetX() + 500; ++x)
//					{
//						p = (Uint8 *)fogofwar_pixels + y * fogofwar_pitch + x * surface_fogofwar->format->BytesPerPixel;
//						//fogofwar_pixel = (Uint32*)surface_fogofwar->pixels + y * surface_fogofwar->pitch + x;
//						
//						//on test si le pixel est dans les limites de la texture, sinon crash (a part pour le x <1360 là on boucle sur la texture)
//						if (x >= 0 && y >= 0 && x < 1360 && y < 1520)	//1360 et 1520 représentent la taille de la texture fogofwar donc de la map
//						{
//							//destAlpha = (unsigned char*)p + 3; // fetch alpha channel
//
//							//destAlpha = srcAlpha;
//							//*(Uint32 *)p = fogofwar_remove_pixels[lolx];
//							*(Uint32 *)p = transparent;
//						}
//						++lolx;
//						
//					}
//				}
//				SDL_UnlockTexture(texture_fogofwar);
//				
//				lolx = 0;
//				loly = 0;
//





				//SDL_LockTexture(texture_fogofwar, NULL, &pixels, &pitch);
				//////////////////////////////
				//////////////////////////////Optimisation de ouf a faire en dessous la boucle for fait 2millions de tours pour tous les pixels de la map
				//////////////////////////////alors qu'il suffirait de modifier ceux autour des persos
				//////////////////////////////et/ou même uniquement ceux affichés à l'écran
				///////////////////////////////
				///////////////EDIT : le memcpy fonctionne et copie tous les pixels de la surface
				///////////////sur la texture, environ 2millions l'opti est à faire là
				///////////////
				///////////////Deux façons d'opti,1 ne changer que les pixels qu'on veut
				///////////////avec ou sans memcpy,2 ne faire les changements qu'a l'écran
				///////////////3 les deux.

		//			SDL_LockTexture(texture_fogofwar, nullptr, reinterpret_cast<void **>(&pixels), &pitch);
		//			SDL_memcpy(pixels, surface_fogofwar->pixels, surface_fogofwar->pitch*surface_fogofwar->h);
		//			//pixels32 = (Uint32*)pixels;	
		//			//pixelCount = (pitch / 4) * surface_fogofwar->h;
		//			//for (int i = 0; i < 50000 ; i++)
		//			//{				
		//			//	pixels32[i] = fogofwar_pixel2;	
		//			//}		
		//			SDL_UnlockTexture(texture_fogofwar);
		
				///////////////////////////
				///////////////////////////Updatetexture oblige a changer toute la texture
				///////////////////////////
		//		if (CameraX > 0 && CameraY > 0)
		//		{
		//		screenRect.x += CameraX;
		//		screenRect.y = CameraY;
		//		}
		//		SDL_UpdateTexture(texture_fogofwar, NULL, surface_fogofwar->pixels, surface_fogofwar->pitch);
		//		////SDL_UnlockTexture(texture_fogofwar);
		//		//
		//		SDL_RenderCopy(gRenderer, texture_fogofwar, NULL, &rect_fogofwar);
				////SDL_DestroyTexture(texture_fogofwar);
			

///////////////////////////////////////
///////////////////////////////////////DU LOURD
///////////////////////////////////////DU LOURD
///////////////////////////////////////
	//			for (row = 0; row < MOOSEPIC_H; ++row) {
	//				dst = (Uint32*)((Uint8*)pixels + row * pitch);
	//				for (col = 0; col < MOOSEPIC_W; ++col) {
	//					color = &MooseColors[*src++];
	//					*dst++ = (0xFF000000 | (color->r << 16) | (color->g << 8) | color->b);
	//					
	//				}
	//				
	//			}
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////




/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

				////void *pixels;
				////int pitch;
				//
				//
				///* Process events */
				//
				///* Modify the pixels of pictureSurface */
				//
				///*
				//* Blit 8-bit palette surface onto the window surface that's
				//* closer to the texture's format
				//*/
				////SDL_BlitSurface(pictureSurface, NULL, windowSurface, NULL);
				//
				///* Modify the texture's pixels */
				//SDL_LockTexture(texture_fogofwar, NULL, &pixels, &pitch);
				//SDL_ConvertPixels(surface_fogofwar->w, surface_fogofwar->h,
				//	surface_fogofwar->format->format,
				//	surface_fogofwar->pixels, surface_fogofwar->pitch,
				//	SDL_PIXELFORMAT_ARGB8888,
				//	pixels, pitch);
				//SDL_UnlockTexture(texture_fogofwar);
				//
				///* Make the modified texture visible by rendering it */
				//SDL_RenderCopy(gRenderer, texture_fogofwar, NULL, &rect_fogofwar);











				//SDL_LockTexture(texture_fogofwar, &surface_fogofwar->clip_rect, &pixels, &pitch);
				//
				//memcpy(pixels, surface_fogofwar->pixels, (surface_fogofwar->pitch * surface_fogofwar->h));
				//
				//int width = surface_fogofwar->w;
				//int height = surface_fogofwar->h;
				//
				//Uint32* pixels32 = (Uint32*)pixels;
				//int     pixelCount = (pitch / 4) * height;
				//
				//Uint32 colorKey = SDL_MapRGBA(surface_fogofwar->format, 0xFF, 0x00, 100, 50);
				//Uint32 transparent = SDL_MapRGBA(surface_fogofwar->format, 0, 0x00, 0xFF, 0x00);
				//
				//
				//for (int i = 0; i < pixelCount; i++) {
				//	
				//		pixels32[i] = transparent;
				//	
				//}
				//
				//SDL_UnlockTexture(texture_fogofwar);
				//
				////SDL_FreeSurface(imageFomatted);
				////SDL_FreeSurface(surface_fogofwar);
				//
				//pixels = NULL;
				//pitch = 0;
				//width = 0;
				//height = 0;
				//
				//
				//SDL_RenderCopy(gRenderer, texture_fogofwar, NULL, &map_rect);















					//SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
					//
					////Lock texture for manipulation 
					//SDL_LockTexture(newTexture, &surface_fogofwar->clip_rect);
					//
					////Copy loaded/formatted surface pixels 
					//memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
					//
					////Get image dimensions 
					//mWidth = formattedSurface->w;
					//mHeight = formattedSurface->h;
					//
					////Get pixel data in editable format 
					//Uint32* pixels = (Uint32*)mPixels;
					//int pixelCount = (mPitch / 4) * mHeight;
					//
					////Map colors 
					//Uint32 colorKey = SDL_MapRGBA(SDL_GetWindowSurface(gWindow)->format, 0, 0xFF, 0xFF, 0xFF);
					//Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(gWindow)->format, 0x00, 0xFF, 0xFF, 0x00);
					//
					////Color key pixels 
					//for (int i = 0; i < pixelCount; ++i)
					//{
					//	if (pixels[i] == colorKey)
					//	{
					//		pixels[i] = transparent;
					//	}
					//}
					//
					////Unlock texture to update 
					//SDL_UnlockTexture(newTexture);
					//mPixels = NULL;
					//
					//SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
		 














				
				////////////////////////////////////////////////////////////////


				//SDL_FillRect(surface_fogofwar, NULL, rgba_fogofwar);
				//
				//rect_fogofwar.x = -200 + CameraX;
				//rect_fogofwar.y = -200 + CameraY;
				//rect_fogofwar.w = carte->getsizex()+400;
				//rect_fogofwar.h = carte->getsizey()+400;
				//
				//texture_fogofwar = SDL_CreateTextureFromSurface(gRenderer, surface_fogofwar);
				//
				//SDL_RenderCopy(gRenderer, texture_fogofwar, NULL, &rect_fogofwar);
				////SDL_DestroyTexture(texture_fogofwar);
				//
				//surface_fogofwar->
				//
				//
//////////////////////////////////////////////////
				////Get pixel data
				//Uint32* pixels = (Uint32*)texture_fogofwar->getPixels();
				//int pixelCount = (gFooTexture.getPitch() / 4) * gFooTexture.getHeight();
				//
				////Map colors
				//Uint32 colorKey = SDL_MapRGB(SDL_GetWindowSurface(gWindow)->format, 0, 0xFF, 0xFF);
				//Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(gWindow)->format, 0xFF, 0xFF, 0xFF, 0x00);
				//
				////Color key pixels
				//for (int i = 0; i < pixelCount; ++i)
				//{
				//	if (pixels[i] == colorKey)
				//	{
				//		pixels[i] = transparent;
				//	}
				//}
				//
				////Unlock texture
				//gFooTexture.unlockTexture();
				//
				//
//////////////////////////////////////////////////
				//
				//
				////Lock texture for manipulation
				//SDL_LockTexture(texture_fogofwar, NULL, , &mPitch);
				//
				////Copy loaded/formatted surface pixels
				//memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
				//
				////Unlock texture to update
				//SDL_UnlockTexture(newTexture);
				//mPixels = NULL;
				//
				////Get image dimensions
				//mWidth = formattedSurface->w;
				//mHeight = formattedSurface->h;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////Fin du brouillon 3 du fogofwar




				//
				//Dessiner le chemin des unités
				//
				for (auto &ally : allyunits)
				{
					int i = 0;
					if (ally->getselect())
					{
						if (ally->action_size() > 0)
						{
							if (ally->getcurrent_action() == 1)
							{
								destination_circle->SetX(ally->getmousex() - destination_circle->GetWidth()/2);
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
				destination_circle->PlayAnimation(0, 7, 0, 6,NULL);
				destination_circle_skills->PlayAnimation(0, 7, 0, 6,NULL);

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


			//		for (auto &aze : active_skills) // access by reference to avoid copying
			//		{
			//			if (aze->getfinish_skill())
			//			{
			//				//i->gethand_grenade_sprite()->~Sprite();
			//				aze->~Skills();
			//				//aze = active_skills.erase(aze);
			//			}
			//		}


					/*
					std::vector<Skills*>::iterator deleteIterator = active_skills.begin();         //iterator??
					while (deleteIterator != active_skills.end())
					{
					if (deleteIterator->geth )
					deleteIterator = active_skills.erase(deleteIterator);
					}*/

					/*
					//printf("%d", active_skills.size());
					for (auto &aze : active_skills)
					{
					if (aze->getfinish_skill())
					{
					aze->gethand_grenade_sprite()->~Sprite();
					aze->~Skills();
					i = active_skills.erase(i);
					if (i < active_skills.size())
					{
					++i;
					}
					}
					}
					*/
					//Maybe opti
					/*
					active_skills.erase(std::remove_if (active_skills.begin(),active_skills.end(),[](Skills element)->bool
					{
					// Do "some stuff", then return true if element should be removed.
					return true;
					}
					),
					active_skills.end()
					);
					*/
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
					mouse_sprite_skill->SetX(input.MouseX()-15);
					mouse_sprite_skill->SetY(input.MouseY()-15);
					mouse_sprite_skill->Draw();
				}
				else
				{
					mouse_sprite->SetX(input.MouseX() - 6);
					mouse_sprite->SetY(input.MouseY());
					mouse_sprite->Draw();
				}

				////////////int centrex = SCREEN_WIDTH / 2;
				////////////int centrey = SCREEN_HEIGHT / 2;
				////////////
				////////////int rayon = 10;
				////////////
				////////////for (int j = rayon; j < (rayon + 5); ++j)
				////////////{
				////////////	for (int i = 0; i < 720; ++i)
				////////////	{
				////////////		int x;
				////////////		int y;
				////////////
				////////////		x = centrex + j * cos(i/2);
				////////////		y = centrey + j * sin(i/2);
				////////////		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0xFF);
				////////////		SDL_RenderDrawPoint(gRenderer, x, y);
				////////////
				////////////	}
				////////////}
				////////////
				////////////rayon = rayon + 5;
				////////////int trans[3] = { 0xF0, 0x0F, 0 };
				////////////
				////////////for (int j = 0; j < 3; ++j)
				////////////{
				////////////	for (int i = 0; i < 720; ++i)
				////////////	{
				////////////		int x;
				////////////		int y;
				////////////
				////////////		x = centrex + (rayon + j) * cos(i / 2);
				////////////		y = centrey + (rayon + j) * sin(i / 2);
				////////////		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, trans[j]);
				////////////		SDL_RenderDrawPoint(gRenderer, x, y);
				////////////
				////////////	}
				////////////}
				////////////
				////////////float xpers = 15, ypers = 15;
				////////////float exploX = 10, exploxY = 10;
				////////////
				////////////float norma = sqrt((exploX - xpers)*(exploX - xpers) + (exploxY - ypers)*(exploxY - ypers));
				////////////printf("Norm = %f", norma);
				////////////
				////////////rayon = 6;
				////////////
				////////////if (norma > rayon)
				////////////{
				////////////	std::cout << "FERME TA GUELE JE SUIS PAS MORT" << std::endl;
				////////////}
				////////////else
				////////////{
				////////////	std::cout << "PETIT BATARD" << std::endl;
				////////////}


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


	//			//Opengl test
	//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//			//glEnable(GL_TEXTURE_2D);
	//			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//		//	modelview = gluLookAt(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0));
	//
	//			//modelview =( glm::mat4)(glm::vec4(0, 0, 2, 0), glm::vec4(0, 0, 0, 0), glm::vec4(0, 1, 0, 0));
	//
	//		//	modelview = { glm::vec4(0, 0, 2, 0), glm::vec4(0, 0, 0, 0), glm::vec4(0, 1, 0, 0) };
	//		//
	//	//		modelview  = glm::mat3{
	//	//			glm::vec3(0, 0, 0),
	//	//			glm::vec3(0, 0, 0),
	//	//			glm::vec3(0, 0, 0)
	//	//		};
	//   
	//
	//	//	// Activation du shader
	//	//	glUseProgram(shaderBasique.getProgramID());
	//	//
	//	//	// Envoi des vertices
	//	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	//	//	glEnableVertexAttribArray(0);
	//	//
	//	//	// Envoi des coordonnées de texture
	//	//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, coordTexture);
	//	//	glEnableVertexAttribArray(2);
	//	//	
	//	//
	//	//	// Envoi des matrices
	//	//	glUniformMatrix4fv(glGetUniformLocation(shaderBasique.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//	//	glUniformMatrix4fv(glGetUniformLocation(shaderBasique.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	//
	//			//// On remplie puis on active le tableau Vertex Attrib 0
	//			//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	//			//glEnableVertexAttribArray(0);
	//
	//			// Verrouillage de la texture
	//			glBindTexture(GL_TEXTURE_2D, id);
	//
	//
	//			// Rendu
	//			//glDrawArrays(GL_TRIANGLES, 0, 6);
	//			
	//		//	glDrawArrays(GL_QUADS, 0, 4);
	//
	//			glBegin(GL_QUADS);	
	//		
	//			glTexCoord2d(50, 0);
	//			glVertex2d(50, 0);
	//			glTexCoord2d(100, 0);
	//			glVertex2d(100, 0);
	//			glTexCoord2d(100, 100);
	//			glVertex2d(100, 100);
	//			glTexCoord2d(50, 100);			
	//			glVertex2d(50, 100);
	//		
	//			glEnd();
	//
	//
	//
	//			// Déverrouillage de la texture
	//			glBindTexture(GL_TEXTURE_2D, 0);
	//
	//
	//			// Désactivation des tableaux
	//	//		glDisableVertexAttribArray(2);
	//	//		glDisableVertexAttribArray(0);
	//
	//			// Affichage du triangle
	//			//glDrawArrays(GL_TRIANGLES, 0, 3);
	//
	//			//glLoadIdentity();
	//			glTranslatef(1, 1, 0);
	//			//glDrawArrays(GL_QUADS, 0, 4);
	//
	//			// On désactive le tableau Vertex Attrib puisque l'on n'en a plus besoin
	//	//		glDisableVertexAttribArray(0);
	//
	//
	//
	//
	//			//glDisable(GL_TEXTURE_2D);
	////			glBindTexture(GL_TEXTURE_2D, 0);
	//
	//			// Désactivation du shader
	//	//		glUseProgram(0);


				//
				//Afficher le tout
				//
				SDL_RenderPresent(gRenderer);
				//glClearColor(0.0, 0.0, 0.0, 1.0);
				//SDL_GL_SwapWindow(gWindow);

				// +1 frame
				++countedFrames;


				//If frame finished early
				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICK_PER_FRAME)
				{
					//Wait remaining time
					SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
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
