#include<iostream>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
//#include<vector>
#include "GameSystem.h"
#include "Screen.h"

//Screen dimensions
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

int main(int argc, char *argv[]) 
{
	GameSystem gameSystem;

	Screen screen(SCREEN_WIDTH, SCREEN_HEIGHT);

	//screen.init();

	//gameSystem.init("BlackJack", 2);

	//gameSystem.playGame();

	/*
	First we use SDL_Init(SDL_INIT_VIDEO) to get the image part of it going
	Then We create a pointer to a SDL_Window and SDL_Surface
	We initialize them by creating a window with SDL_CreateWindow() and SDL_GetWindowSurface()
	Then we draw the surface (Window is the eisel, surface is the canvas) and update the surface on the window
	*/

	//Start up SDL and create window
	if (!screen.init()) {
		std::cout << "Failed to initialize!\n";
	}
	else {

		enum mainMenuTextures { BACKGROUND, TITLE, BLACKJACK, POKER, HEARTS, BLACKJACKSELECTED, POKERSELECTED, HEARTSSELECTED, BLACKJACKHIGHLIGHTED, POKERHIGHLIGHTED, HEARTSHIGHLIGHTED };
		enum allignmentFlag { UPLEFT, CENTER };

		for (int i = 0; i < 11; i++) {
			std::string path = "MainMenu/";
			if (i == 0)
				path += "background";
			else if (i == 1)
				path += "title";
			else if (i % 3 == 2)
				path += "blackJack";
			else if (i % 3 == 0)
				path += "poker";
			else if (i % 3 == 1)
				path += "hearts";

			if (i >= 5 && i <= 7)
				path += "Selected";
			if (i >= 8 && i <= 10)
				path += "Highlighted";
			path += ".png";
			screen.addTexture(path);
			
		}

		//Main loop flag
		bool quit = false;

		bool ifMouseIsPressedDown = false;

		//Event handler
		SDL_Event e;

		//While application is running
		while (!quit) {

			int x, y;
			SDL_GetMouseState(&x, &y);

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0) {
				
				//User requests quit
				if (e.type == SDL_QUIT)
					quit = true;
				//User presses down
				if (e.type == SDL_MOUSEBUTTONDOWN)
					ifMouseIsPressedDown = true;
				//User unpresses
				if (e.type == SDL_MOUSEBUTTONUP) {
					ifMouseIsPressedDown = false;
					if (screen.ifInsideTexture(x, y, BLACKJACK))
						gameSystem.init("BlackJack", 2);
					if (screen.ifInsideTexture(x, y, POKER))
						gameSystem.init("Poker", 7);
					if (screen.ifInsideTexture(x, y, HEARTS))
						gameSystem.init("Hearts", 4);
					//For some reason I can't close the main menu screen before I open the new one???
					gameSystem.playGame();
				}
			}

			//resets renderer
			screen.resetRenderer();

			screen.drawBackground(0, 0, 0, BACKGROUND);
			//screen.drawTexture(50, 20, 9, 8, TITLE, CENTER);
			screen.drawText("GAMBLING", { 0, 0, 0 }, 2, 50, 20, 7, 7, CENTER, 0, false);
			screen.highlightColor = { 255, 255, 0 };
			
			if (screen.ifInsideTexture(x, y, BLACKJACK)) {
				if (ifMouseIsPressedDown)
					screen.drawTexture(50, 40, 9, 8, BLACKJACKHIGHLIGHTED, CENTER, 0, false);
				else
					screen.drawTexture(50, 40, 9, 8, BLACKJACKSELECTED, CENTER, 0, false);
			}
			else
				screen.drawTexture(50, 40, 9, 8, BLACKJACK, CENTER, 0, false);

			if (screen.ifInsideTexture(x, y, POKER)) {
				if (ifMouseIsPressedDown)
					screen.drawTexture(50, 60, 9, 8, POKERHIGHLIGHTED, CENTER, 0, false);
				else
					screen.drawTexture(50, 60, 9, 8, POKERSELECTED, CENTER, 0, false);
			}
			else
				screen.drawTexture(50, 60, 9, 8, POKER, CENTER, 0, false);

			if (screen.ifInsideTexture(x, y, HEARTS)) {
				if (ifMouseIsPressedDown)
					screen.drawTexture(50, 80, 9, 8, HEARTSHIGHLIGHTED, CENTER, 0, false);
				else
					screen.drawTexture(50, 80, 9, 8, HEARTSSELECTED, CENTER, 0, false);
			}
			else
				screen.drawTexture(50, 80, 9, 8, HEARTS, CENTER, 0, false);

			screen.update();
		}
	}

	//Choose which game to play.

	//Hearts uses one deck and 4 players
	//BlackJack uses one deck and 2-7 players
	//Poker uses one deck and 2-7 players.

	//system("PAUSE");
	return 0;
}



