#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cmath>
#include <string>
#include <vector>
#include "Timer.h"

class Screen
{
public:

	Screen();
	Screen(int width, int height);

	~Screen();
	//Starts up SDL and creates window and surface
	bool init();

	//Frees media and shuts down SDL
	void close();

	//adds a texture (returns -1 if fail otherwise the index of the texture)
	int addTexture(std::string path);

	//display text
	bool drawText(std::string textureText, SDL_Color textColor, int whichFont, double x, double y, double widthDivisor, double heightDivisor, int allignment, double rotationAngle, bool ifBordered);

	//Loads individual image as texture
	SDL_Texture* loadTexture(std::string path);

	//Draws the card
	bool drawTexture(double x, double y, double widthDivisor, double heightDivisor, int id, int allignment, double rotationAngle, bool ifBordered);

	bool drawTexture(int id);
	//Destroys the card
	bool destroyCard(int id);

	//Draws the background
	bool drawBackground(int r, int g, int b, int id);

	//Reset the renderer
	bool resetRenderer();

	//Updates window
	void update() { SDL_RenderPresent(_renderer); }

	//Configures the objects in textures
	void setStaticTexture(double x, double y, double widthDivisor, double heightDivisor, int id, int allignment, double rotationAngle, bool ifBordered);
	void unsetStaticTexture(int id);
	void setAnimatedTexture(int id, double endX, double endY, int totalTime);

	void drawTextures();

	//getters
	bool ifInsideTexture(int x, int y, int id);
	void getTexturePosition(int &x, int &y, int id) {
		x = _texturePositionsX[id];
		y = _texturePositionsY[id];
	}
	//setters

	//Waiting for effect
	void wait(int milliseconds) { SDL_Delay(milliseconds); }

	SDL_Color highlightColor = { 0, 0, 0 };

	//Time functions

	void addFullStartTime() { timer.addFullStartTime(); }
	int addStartTime() { return timer.addStartTime(); }
	unsigned int getElapsedTime(int id) { return timer.getElapsedTime(id); }
	unsigned int getFullElapsedTime() { return timer.getFullElapsedTime(); }
	void clearStartTimes() { timer.clearStartTimes(); }

private:
	Timer timer;

	SDL_Window* _window;
	SDL_Renderer* _renderer;
	std::vector <SDL_Texture*> _textures;
	std::vector <TTF_Font*> _fonts;

	enum allignment { UPLEFT, CENTER, ROTATABLE };

	std::vector <int> _textureHeights;
	std::vector <int> _textureWidths;

	std::vector <int> _textureDrawnHeights;
	std::vector <int> _textureDrawnWidths;

	std::vector <int> _texturePositionsX;
	std::vector <int> _texturePositionsY;

	struct Texture {
		Timer timer;
		SDL_Texture* texture = NULL;

		//maybe have a vector in here for multiple texture things
		//For instance like for a card flipping or whatever

		//For drawing
		double heightDivisor = 1;
		double widthDivisor = -1;

		double beginHeightDivisor = -1;
		double beginWidthDivisor = -1;

		double endHeightDivisor = -1;
		double endWidthDivisor = -1;

		//Height and width of the texture
		int height = 0;
		int width = 0;

		//How tall and high it is drawn on the screen
		int drawnHeight = 0;
		int drawnWidth = 0;

		//Where it is drawn at any given moment
		int x = -1;
		int y = -1;

		//Rotation angle
		double theta = 0;

		double beginTheta = 0;
		double endTheta = 0;

		//if it's highlighted, yaknow?
		bool ifBordered = false;

		//animation positions
		int endX = -1, endY = -1, beginX = -1, beginY = -1;
		int totalTime = 0;
		//false when completely stationary
		//Maybe you don't even need this though
		bool ifAnimated = false;

		bool ifDrawn = false;

		//How it is alligned
		int allignment = UPLEFT;
	};

	std::vector <Texture> textures;

	int SCREEN_HEIGHT;
	int SCREEN_WIDTH;

	bool _fullscreen;

	int highlighterID;
};

