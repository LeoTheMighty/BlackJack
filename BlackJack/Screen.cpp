#include <iostream>
#include "Screen.h"

Screen::Screen()
{
	SCREEN_HEIGHT = SCREEN_WIDTH = 0;
	_renderer = NULL;
	_window = NULL;
	_fullscreen = true;
}

Screen::Screen(int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	_renderer = NULL;
	_window = NULL;
	_fullscreen = false;
}


Screen::~Screen()
{

}

//Starts up SDL and creates window and surface
bool Screen::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}

	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		if (_fullscreen) {
			SDL_DisplayMode DM;
			SDL_GetDesktopDisplayMode(0, &DM);
			SCREEN_WIDTH = DM.w;
			SCREEN_HEIGHT = DM.h;
		}

		//Create the window
		_window = SDL_CreateWindow("Gambling outlet for kids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (_window == NULL) {
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		else {

			//Create renderer for window
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (_renderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Get Window surface
				//gScreenSurface = SDL_GetWindowSurface(gWindow);

				//Initialize renderer color
				SDL_SetRenderDrawColor(_renderer, 0x0, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				//Open the fonts
				_fonts.push_back(TTF_OpenFont("fonts/ttf/Chunkfive.ttf", 200));
				_fonts.push_back(TTF_OpenFont("fonts/ttf/Pacifico.ttf", 200));
				_fonts.push_back(TTF_OpenFont("fonts/ttf/DejaVuSerif-Bold.ttf", 100));
				_fonts.push_back(TTF_OpenFont("fonts/ttf/STIXGeneral.ttf", 100));
				_fonts.push_back(TTF_OpenFont("fonts/ttf/cmtt10.ttf", 100));
				_fonts.push_back(TTF_OpenFont("fonts/ttf/DejaVuSans-Oblique.ttf", 100));
				_fonts.push_back(TTF_OpenFont("fonts/ttf/STIXNonUniBolIta.ttf", 100));

				for (unsigned i = 0; i < _fonts.size(); i++) {
					if (_fonts[i] == NULL)
					{
						printf("Failed to load a font! SDL_ttf Error: %s\n", TTF_GetError());
						success = false;
					}
				}

				drawText("Loading...", { 255, 255, 255 }, 0, 50, 50, 10, 10, CENTER, 0, 0);
				update();
			}
		}
	}
	return success;
}

//Frees media and shuts down SDL
void Screen::close()
{
	//Free loaded images
	for (unsigned i = 0; i < _textures.size(); i++) {
		SDL_DestroyTexture(_textures[i]);
		_textures[i] = NULL;
	}

	for (unsigned i = 0; i < textures.size(); i++) {
		SDL_DestroyTexture(textures[i].texture);
		textures[i].texture = NULL;
	}
	//SDL_FreeSurface(gPNGSurface);
	//gPNGSurface = NULL;

	//Close the fonts
	for (unsigned i = 0; i < _fonts.size(); i++) {
		TTF_CloseFont(_fonts[i]);
		_fonts[i] = NULL;
	}

	//Destroy window
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	_window = NULL;
	_renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

//Loads individual image as texture
SDL_Texture* Screen::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture form %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		//Add the dimensions to the other vectors
		_textureHeights.push_back(loadedSurface->h);
		_textureWidths.push_back(loadedSurface->w);
		textures.back().height = loadedSurface->h;
		textures.back().width = loadedSurface->w;
		_textureDrawnHeights.push_back(-1);
		_textureDrawnWidths.push_back(-1);
		_texturePositionsX.push_back(-1);
		_texturePositionsY.push_back(-1);

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

//adds a texture (returns -1 if fail otherwise the index of the texture)
int Screen::addTexture(std::string path) {
	Texture texture;
	textures.push_back(texture);
	//textures.back().texture = loadTexture(path);
	SDL_Texture* newTexture = loadTexture(path);
	int id;

	if (newTexture == NULL)
		id = -1;
	else {
		id = _textures.size();
		_textures.push_back(newTexture);
		textures.back().texture = newTexture;
	}
	return id;
}

//Draws the game
bool Screen::drawTexture(double x, double y, double widthDivisor, double heightDivisor, int id, int allignment, double rotationAngle, bool ifBordered) {
	//UPDATE THIS SO THAT IT MAINTAINS THE ASPECT RATIO OF THE CARD, SO IT BASES THE SIZE OFF OF THE SMALLEST DIMENSION (WIDTH vs HEIGHT)

	//UPLEFT is when 0-100 hits the edge of the screens
	//CENTER is when 50 is the center of the screen for the center of the texture

	//Object dimensions
	double width = _textureWidths[id];
	double height = _textureHeights[id];
	//What both dimensions will be multiplied by
	double multiplier;
	
	if (SCREEN_HEIGHT > SCREEN_WIDTH) {
		multiplier = (double(SCREEN_WIDTH) / widthDivisor) / double(width);
	}
	else {
		multiplier = (double(SCREEN_HEIGHT) / heightDivisor) / double(height);
	}

	width *= multiplier;
	height *= multiplier;

	double effectiveScreenHeight = SCREEN_HEIGHT - height;
	double effectiveScreenWidth = SCREEN_WIDTH - width;

	double adjustWidth = 0, adjustHeight = 0;

	if (allignment == CENTER || allignment == ROTATABLE) {
		adjustWidth = (width / 2);
		adjustHeight = (height / 2);
		effectiveScreenHeight += height;
		effectiveScreenWidth += width;
	}

	SDL_Rect cardRect;

	if (allignment != ROTATABLE)
		cardRect = { int(x * effectiveScreenWidth / 100.0 - adjustWidth), int(y * effectiveScreenHeight / 100.0 - adjustHeight), int(width), int(height) };
	else
		cardRect = { int(x + SCREEN_WIDTH / 2 - adjustWidth), int(y + SCREEN_HEIGHT / 2 - adjustHeight), int(width), int(height) };

	_texturePositionsX[id] = cardRect.x;
	_texturePositionsY[id] = cardRect.y;
	_textureDrawnHeights[id] = cardRect.h;
	_textureDrawnWidths[id] = cardRect.w;

	//Use the color changing function in tandem with the loaded highlight texture
	SDL_Rect highlightRect;
	if (rotationAngle == 0 || rotationAngle == 180) highlightRect = { cardRect.x - 5, cardRect.y - 5, cardRect.w + 10, cardRect.h + 10 };
	else if (rotationAngle == 90 || rotationAngle == 270) highlightRect = { cardRect.x - 5, cardRect.y + 5, cardRect.h + 10, cardRect.w + 10 };
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(_renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
	if (ifBordered)
		SDL_RenderFillRect(_renderer, &highlightRect);


	if (SDL_RenderCopyEx(_renderer, _textures[id], NULL, &cardRect, rotationAngle, NULL, SDL_FLIP_NONE) == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Screen::drawTexture(int id) {
	int allignment = textures[id].allignment;

	double x = textures[id].x;
	double y = textures[id].y;

	double rotationAngle = textures[id].theta;
	bool ifBordered = textures[id].ifBordered;
	//Object dimensions
	double width = textures[id].width;
	double height = textures[id].height;

	double widthDivisor = textures[id].widthDivisor;
	double heightDivisor = textures[id].heightDivisor;
	//What both dimensions will be multiplied by
	double multiplier;

	if (SCREEN_HEIGHT > SCREEN_WIDTH) {
		multiplier = (double(SCREEN_WIDTH) / widthDivisor) / double(width);
	}
	else {
		multiplier = (double(SCREEN_HEIGHT) / heightDivisor) / double(height);
	}

	width *= multiplier;
	height *= multiplier;

	double effectiveScreenHeight = SCREEN_HEIGHT - height;
	double effectiveScreenWidth = SCREEN_WIDTH - width;

	double adjustWidth = 0, adjustHeight = 0;

	if (allignment == CENTER || allignment == ROTATABLE) {
		adjustWidth = (width / 2);
		adjustHeight = (height / 2);
		effectiveScreenHeight += height;
		effectiveScreenWidth += width;
	}

	SDL_Rect cardRect;

	if (allignment != ROTATABLE)
		cardRect = { int(x * effectiveScreenWidth / 100.0 - adjustWidth), int(y * effectiveScreenHeight / 100.0 - adjustHeight), int(width), int(height) };
	else
		cardRect = { int(x + SCREEN_WIDTH / 2 - adjustWidth), int(y + SCREEN_HEIGHT / 2 - adjustHeight), int(width), int(height) };

	_texturePositionsX[id] = cardRect.x;
	_texturePositionsY[id] = cardRect.y;
	_textureDrawnHeights[id] = cardRect.h;
	_textureDrawnWidths[id] = cardRect.w;

	//Use the color changing function in tandem with the loaded highlight texture
	SDL_Rect highlightRect;
	if (rotationAngle == 0 || rotationAngle == 180) highlightRect = { cardRect.x - 5, cardRect.y - 5, cardRect.w + 10, cardRect.h + 10 };
	else if (rotationAngle == 90 || rotationAngle == 270) highlightRect = { cardRect.x - 5, cardRect.y + 5, cardRect.h + 10, cardRect.w + 10 };
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(_renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
	if (ifBordered)
		SDL_RenderFillRect(_renderer, &highlightRect);


	if (SDL_RenderCopyEx(_renderer, _textures[id], NULL, &cardRect, rotationAngle, NULL, SDL_FLIP_NONE) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//display text
bool Screen::drawText(std::string textureText, SDL_Color textColor, int whichFont, double x, double y, double widthDivisor, double heightDivisor, int allignment, double rotationAngle, bool ifBordered) {
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;

	//Render text surface
	textSurface = TTF_RenderText_Solid(_fonts[whichFont], textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
		if (textTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			_textureWidths.push_back(textSurface->w);
			_textureHeights.push_back(textSurface->h);
			_textureDrawnHeights.push_back(-1);
			_textureDrawnWidths.push_back(-1);
			_texturePositionsX.push_back(-1);
			_texturePositionsY.push_back(-1);


			int id = _textures.size();
			_textures.push_back(textTexture);

			drawTexture(x, y, widthDivisor, heightDivisor, id, allignment, rotationAngle, ifBordered);

			_textures.pop_back();
			_textureWidths.pop_back();
			_textureHeights.pop_back();
			_textureDrawnHeights.pop_back();
			_textureDrawnWidths.pop_back();
			_texturePositionsX.pop_back();
			_texturePositionsY.pop_back();

		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	}

	//Return success
	return textTexture != NULL;
}

//Destroys the card
bool Screen::destroyCard(int id) {
	SDL_DestroyTexture(_textures[id]);
	_textures[id] = NULL;
	return true;
}

bool Screen::drawBackground(int r, int g, int b, int id) {
	
	//Get window surface
	SDL_Surface* screenSurface = SDL_GetWindowSurface(_window);

	//Fill the surface white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, r, g, b));

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(_renderer, screenSurface);

	SDL_Rect backRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	_texturePositionsX[id] = 0;
	_texturePositionsY[id] = 0;

	if (SDL_RenderCopyEx(_renderer, newTexture, NULL, &backRect, 0, NULL, SDL_FLIP_NONE) == 0 && SDL_RenderCopyEx(_renderer, _textures[id], NULL, &backRect, 0, NULL, SDL_FLIP_NONE) == 0) {
		SDL_FreeSurface(screenSurface);
		SDL_DestroyTexture(newTexture);
		return true;
	}
	else {
		SDL_FreeSurface(screenSurface);
		SDL_DestroyTexture(newTexture);
		return false;
	}
}

//Reset the renderer
bool Screen::resetRenderer() {
	bool success = true;
	//Initialize renderer color
	SDL_SetRenderDrawColor(_renderer, 0x0, 0xFF, 0xFF, 0xFF);
	//Clear renderer
	SDL_RenderClear(_renderer);
	return success;
}

bool Screen::ifInsideTexture(int x, int y, int id) {
	int lowerBoundX = _texturePositionsX[id];
	int lowerBoundY = _texturePositionsY[id];
	int upperBoundY = lowerBoundY + _textureDrawnHeights[id];
	int upperBoundX = lowerBoundX + _textureDrawnWidths[id];

	return (x >= lowerBoundX && x <= upperBoundX && y <= upperBoundY && y >= lowerBoundY);
}

//Configures the objects in textures
void Screen::setStaticTexture(double x, double y, double widthDivisor, double heightDivisor, int id, int allignment, double rotationAngle, bool ifBordered) {
	textures[id].allignment = allignment;
	textures[id].x = x;
	textures[id].y = y;
	textures[id].heightDivisor = heightDivisor;
	textures[id].widthDivisor = widthDivisor;
	textures[id].theta = rotationAngle;
	textures[id].ifBordered = ifBordered;
	textures[id].ifDrawn = true;
}

void Screen::unsetStaticTexture(int id) {
	textures[id].x = -1;
	textures[id].y = -1;
	textures[id].heightDivisor = -1;
	textures[id].widthDivisor = -1;
	textures[id].theta = 0;
	textures[id].ifBordered = false;
	textures[id].ifDrawn = false;
}

//Eventually, you should have like an end theta and beginning theta so you can rotate them too
//Also be able to change the size of the object (though that might look funky)
//Also be able to go through a a vector of different textures to animate it (like with the flipping of a card.)
//also you should animate the highlighting by changing the alpha of it
void Screen::setAnimatedTexture(int id, double endX, double endY, int totalTime) {
	textures[id].beginX = textures[id].x;
	textures[id].beginY = textures[id].y;
	textures[id].ifAnimated = true;
	textures[id].endX = endX;
	textures[id].endY = endY;
	textures[id].timer.setStartTime();
	textures[id].timer.setEndTime(totalTime);
	textures[id].totalTime = totalTime;
}

void Screen::drawTextures() {

	for (int i = 0; i < textures.size(); i++) {

		if (textures[i].ifAnimated) {

			//Change the x and y 
			textures[i].x = textures[i].beginX + (textures[i].timer.getElapsedTime() / textures[i].totalTime)*(textures[i].endX - textures[i].beginX);
			textures[i].y = textures[i].beginY + (textures[i].timer.getElapsedTime() / textures[i].totalTime)*(textures[i].endY - textures[i].beginY);
			if (abs(textures[i].endX - textures[i].x) <= 0 && abs(textures[i].endY - textures[i].y) <= 0) {
				textures[i].x = textures[i].endX;
				textures[i].y = textures[i].endY;
				textures[i].ifAnimated = false;
			}
			else {
				int x = textures[i].x;
				int y = textures[i].y;
				double widthDivisor = textures[i].widthDivisor;
				double heightDivisor = textures[i].heightDivisor;
				int allignment = textures[i].allignment;
				double theta = textures[i].theta;
				bool ifBordered = textures[i].ifBordered;

				drawTexture(x, y, widthDivisor, heightDivisor, i, allignment, theta, ifBordered);
			}
			

			//if they're at endX and endY, then turn off the animation and make endX and endY equal x and y;

		}
		else if (textures[i].ifDrawn) {

			int x = textures[i].x;
			int y = textures[i].y;
			double widthDivisor = textures[i].widthDivisor;
			double heightDivisor = textures[i].heightDivisor;
			int allignment = textures[i].allignment;
			double theta = textures[i].theta;
			bool ifBordered = textures[i].ifBordered;

			drawTexture(x, y, widthDivisor, heightDivisor, i, allignment, theta, ifBordered);
		}
	}
}