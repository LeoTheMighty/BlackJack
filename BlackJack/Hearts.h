#pragma once
#include <vector>
#include "Deck.h"
#include "Screen.h"
#include "Card.h"
class Hearts
{
public:

	struct AnimateObject {
		double beginX;
		double beginY;
		double endX;
		double endY;
		double theta;
		//Time it will take to complete the animation for this specific object
		double totalTime;
		//What allignment is being used when drawing the textures
		int allignment;
		//what id it is in the startTimes vector
		//this is -1 if it hasn't started yet
		int startTimeID = -1;
		//texture id of the object in question
		int id = -1;
		//animation id
		int animationID = -1;
		//width and height divisors
		double widthDivisor = -1, heightDivisor = -1;
		bool ifFinished = false;
		bool ifHighlighted = false;
	};

	Hearts();

	void init(Deck deck);
	void restart();
	//give everyone cards
	//Maybe you can animate it too??
	void dealCards();
	//The passing round
	void passTheCards();
	//Play hand
	void playHand();
	//play entire round
	void playRound();
	//play game
	void playGame();
	int userInput(std::string move);
	int computerInput(std::string move);
	//rank the players
	bool rankPlayers();
	//Draw the game that is not moving 
	void drawStaticGame();

	//Organize the hands in order of suit and value
	void organizeHands();

	//start an animation given by the list of animateObjects
	void startAnimation(int animationID);
	//stop an animation given by the list
	void stopAnimation(int animationID);
	//render animation
	void renderAnimations();

	//add an animation (returns the index of this animation
	int addAnimation(std::vector <AnimateObject> animateObjects);

	//create an animateObject (the easier way)
	AnimateObject createAnimateObject(double beginX, double beginY, double endX, double endY, double theta, double totalTime, int allignment);
	AnimateObject createAnimateObject(double beginX, double beginY, double endX, double endY, double theta, double totalTime, int allignment, int id, double widthDivisor, double heightDivisor);

	//if a single animation finished
	bool ifAnimationFinished(int animationID);

	//If a group of animations has finished
	bool ifGroupAnimationFinished(int startAnimationID, int endAnimationID);

	~Hearts();
private:
	Screen heartsScreen;
	static const int _numPlayers = 4;
	Deck _deck;
	enum players { SOUTH, WEST, NORTH, EAST };
	enum passingRounds { TOLEFT, TORIGHT, TOSTRAIGHT, TONONE };
	enum allignmentFlag { UPLEFT, CENTER, ROTATABLE };
	enum screens { DEALING, PASSING, PLAYING, RANKING, WINNING };
	int passingRound = 0;
	int screen = 0;
	bool ifHeartsBroken = false;
	//no money
	//What the game is played until
	int maxHeartValue = 100;
	struct Player {
		std::string playerName = "";
		//should be 13 at the very beginning
		std::vector <Card> hand;
		//0 at beginning, can be up to the whole deck
		std::vector <Card> takenCards;
		//Cards they choose to pass
		std::vector <Card> chosenCards;
		//cards to be passed to the player
		std::vector <Card> cardsToBePassed;
		int numHearts = 0;
		//from 1 thru 4, depending on how low numHearts is comparatively
		int ranking = 0;
	};

	std::vector <Player> players;

	//You can have multiple animations lined up, so you can wait to start them all
	std::vector <std::vector <AnimateObject>> animateObjectsList;

	//texture id
	int tableID;


	//The slices of indexes that acts as animation segments (when multiple "animations" make up one big animation)
	enum indecies { START, END };
	//animation "IDs" (completes the first and last one, so if same, only one animation)
	int dealingAnimationID[2] = { -1, -1 };

};

