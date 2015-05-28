#pragma once


class camera
{
private:
	//The X and Y offsets of the dot
	int x, y;

	//The velocity of the dot
	int xVel, yVel;
	SDL_Rect cam;

public:
	//Initializes the variables
	camera();
	~camera();
	//Takes key presses and adjusts the dot's velocity
	void handle_input();

	//Moves the dot
	void move();

	//Shows the dot on the screen
	void show();

	//Sets the camera over the dot
	void set_camera();
	
	SDL_Rect getcam();

	void setcam(SDL_Rect rect);

};
