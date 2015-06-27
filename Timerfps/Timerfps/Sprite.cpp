#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite()
{}

Sprite::Sprite(SDL_Renderer* passed_renderer, std::string FilePath, int x, int y, int w, int h, float *passed_CameraX, float *passed_CameraY)
{
	CameraX = passed_CameraX;
	CameraY = passed_CameraY;

	renderer = passed_renderer;
	//image = IMG_LoadTexture(renderer, FilePath.c_str());
	//image = NULL;
	surfaceimage = IMG_Load(FilePath.c_str());


	if (surfaceimage == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", FilePath.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(surfaceimage, SDL_TRUE, SDL_MapRGB(surfaceimage->format, 0xFF, 0xFF, 0xFF));
	

	image = SDL_CreateTextureFromSurface(renderer, surfaceimage);
	if (image == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", FilePath.c_str(), SDL_GetError());
	}
	else
	{
		//Get image dimensions
		img_width = surfaceimage->w;
		img_height = surfaceimage->h;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surfaceimage);
	}



	if (image == NULL)
	{
		std::cout << "Couldn't Load " << FilePath.c_str() << std::endl;
	}

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_QueryTexture(image, NULL, NULL, &img_width, &img_height);

	crop.x = 0;
	crop.y = 0;
	crop.w = img_width;
	crop.h = img_height;

	X_pos = x;
	Y_pos = y;

	Orgin_X = 0;
	Orgin_Y = 0;

	CurrentFrame = 0;
	Amount_Frame_X = 0;
	Amount_Frame_X = 0;
}

Sprite::Sprite(SDL_Renderer* passed_renderer,SDL_Surface* surface, int x, int y, int w, int h, float *passed_CameraX, float *passed_CameraY)
{
	CameraX = passed_CameraX;
	CameraY = passed_CameraY;

	renderer = passed_renderer;
	//image = IMG_LoadTexture(renderer, FilePath.c_str());
	//image = NULL;
	//surfaceimage = surface;

	if (surface == NULL)
	{
		printf("Unable to load image ! Probleme avec la surface\n");
	}
	else
	{
		//Color key image
		//SDL_SetColorKey(surfaceimage, SDL_TRUE, SDL_MapRGB(surfaceimage->format, 0xFF, 0xFF, 0xFF));


		image = SDL_CreateTextureFromSurface(renderer, surface);
		if (image == NULL)
		{
			printf("Unable to create texture from surface");
		}
		else
		{
			//Get image dimensions
			img_width = surface->w;
			img_height = surface->h;
		}
	}



	if (image == NULL)
	{
		std::cout << "Couldn't Load ";
	}

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_QueryTexture(image, NULL, NULL, &img_width, &img_height);

	crop.x = 0;
	crop.y = 0;
	crop.w = img_width;
	crop.h = img_height;

	X_pos = x;
	Y_pos = y;

	Orgin_X = 0;
	Orgin_Y = 0;

	CurrentFrame = 0;
	Amount_Frame_X = 0;
	Amount_Frame_X = 0;
}

void Sprite::SetUpAnimation(int passed_Amount_X, int passed_Amount_Y)
{
	Amount_Frame_X = passed_Amount_X;
	Amount_Frame_Y = passed_Amount_Y;
}

void Sprite::PlayAnimation(int BeginFrame, int EndFrame, int Row, float Speed, bool allrow)
{
	if (allrow)
	{
		if (animationDelay + Speed < countedFrames)
		{
			if (EndFrame <= CurrentFrame)
			{
				CurrentFrame = BeginFrame;
				allrow_row++;
			}
			else
			{
				CurrentFrame++;
			}

			crop.x = CurrentFrame * (img_width / Amount_Frame_X);
			crop.y = allrow_row * (img_height / Amount_Frame_Y);
			crop.w = img_width / Amount_Frame_X;
			crop.h = img_height / Amount_Frame_Y;

			animationDelay = countedFrames;
		}
	}
	else
	{
		if (animationDelay + Speed < countedFrames)
		{
			if (EndFrame <= CurrentFrame)
			{
				CurrentFrame = BeginFrame;
			}
			else
			{
				CurrentFrame++;
			}

			crop.x = CurrentFrame * (img_width / Amount_Frame_X);
			crop.y = Row * (img_height / Amount_Frame_Y);
			crop.w = img_width / Amount_Frame_X;
			crop.h = img_height / Amount_Frame_Y;

			animationDelay = countedFrames;
		}
	}
}

Sprite::~Sprite(void)
{
	if (image != nullptr)
	SDL_DestroyTexture(image);
}

void Sprite::Draw()
{
	Position.x = rect.x + *CameraX;
	Position.y = rect.y + *CameraY;
	Position.w = rect.w;
	Position.h = rect.h;

	SDL_RenderCopyEx(renderer, image, &crop, &Position, angle, NULL, flipType);
}

void Sprite::SetX(float X)
{
	X_pos = X;

	rect.x = int(X_pos - Orgin_X);
}

void Sprite::SetY(float Y)
{
	Y_pos = Y;

	rect.y = int(Y_pos - Orgin_Y);
}

void Sprite::SetPosition(float X, float Y)
{
	X_pos = X;
	Y_pos = Y;
	
	rect.x = int(X_pos - Orgin_X);
	rect.y = int(Y_pos - Orgin_Y);
}

float Sprite::GetX()
{
	return X_pos;
}

float Sprite::GetY()
{
	return Y_pos;
}

void Sprite::SetOrgin(float X, float Y)
{	
	Orgin_X = X;
	Orgin_Y = Y;

	SetPosition(GetX(), GetY());
}

void Sprite::SetWidth(int W)
{
	rect.w = W;
}

void Sprite::SetHeight(int H)
{
	rect.h = H;
}

int Sprite::GetWidth()
{
	return rect.w;
}

int Sprite::GetHeight()
{
	return rect.h;
}

int Sprite::GetAngle()
{
	return angle;
}

void Sprite::SetAngle(int passed_angle)
{
	angle = passed_angle;
}

void Sprite::setfliptype(SDL_RendererFlip passed_fliptype)
{
	flipType = passed_fliptype;
}

SDL_RendererFlip Sprite::getfliptype()
{
	return flipType;
}