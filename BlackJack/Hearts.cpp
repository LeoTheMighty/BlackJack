#include <iostream>
#include <sstream>
#include <random>
#include <ctime>
#include "Hearts.h"
#include <SDL.h>

Hearts::Hearts()
{
	
}

void Hearts::init(Deck deck)
{
	_deck = deck;
	heartsScreen.init();

	for (int i = 0; i < _deck.getSize(); i++) {
		int id = heartsScreen.addTexture(_deck.lookAtCard(i).getImageFileName());

		if (id >= 0) {
			_deck.setCardID(i, id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for cards! SDL Error: " << SDL_GetError() << "\n";

	}

	{
		int id = heartsScreen.addTexture(_deck.getBackImageFileName());

		if (id >= 0) {
			_deck.setBackID(id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for back! SDL Error: " << SDL_GetError() << "\n";

	}

	std::stringstream ss;
	ss << "MainMenu/GamblingTable.png";

	{
		int id = heartsScreen.addTexture(ss.str());

		if (id >= 0) {
			tableID = (id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for background! SDL Error: " << SDL_GetError() << "\n";

	}

	ss.str("");

	//Load all of the textures
	std::vector <AnimateObject> animateObjects;

	//Dealing animations
	for (int j = 0; j < _deck.getSize(); j++) {
		double beginX = 50, beginY = 50, endX = 50, endY = 50;
		double theta;

		switch (j % 4) {
		case NORTH:
			theta = 180;
			endY = 15;
			break;
		case WEST:
			theta = 90;
			endX = 10;
			break;
		case SOUTH:
			theta = 0;
			endY = 85;
			break;
		case EAST:
			theta = 270;
			endX = 90;
			break;
		}

		animateObjects.push_back(createAnimateObject(beginX, beginY, endX, endY, theta, 750, CENTER));

		if (j == 0)
			dealingAnimationID[START] = dealingAnimationID[END] = addAnimation(animateObjects);
		else
			dealingAnimationID[END] = addAnimation(animateObjects);

		animateObjects.clear();
	}

	//Moving card to passing

	//Load all of the animations

	for (int i = 0; i < 4; i++)
		players.push_back({});
	restart();
}

Hearts::~Hearts()
{
}

//restart or regular start the game
void Hearts::restart() {
	
}

//give everyone cards
//Maybe you can animate it too??
void Hearts::dealCards() {
	//Start of the animation
	screen = DEALING;
	heartsScreen.addFullStartTime();
	bool ifFinished = false;
	//std::vector <int> textureIDForTimes;
	//std::vector <bool> textureIDForFinished;
	//Static Game has background and sometimes words?
	//drawStaticGame();
	//to finish the card movement
	double totalTime = 750;
	int waitTime = 0, differenceInWaitTimes = 100;
	int i = 0;
	while (!ifFinished) {
		if (unsigned(waitTime) < heartsScreen.getFullElapsedTime() && _deck.getSize() != unsigned(0)) {
			//std::vector <AnimateObject> animateObjects;
			/*

			//texture id of the object in question
			int id;
			//animation id
			int animationID;
			double beginX, beginY;
			double endX, endY;
			double theta;
			//what id it is in the startTimes vector
			//this is -1 if it hasn't started yet
			double startTimeID = -1;
			//Time it will take to complete the animation for this specific object
			double totalTime;
			bool ifFinished = false;
			bool ifHighlighted = false;
			//What allignment is being used when drawing the textures
			int allignment;

			*/

			double cardWidth = 7.5, cardHeight = 7.5;

			//createAnimateObject(0, 50, 50, 50, 75, 0, totalTime, CENTER);

			int playerIndex = i % players.size();
			players[playerIndex].hand.push_back(_deck.takeCardFromTop());
			//Start the animation for the card to go to the players hand
			//heartsScreen.addStartTime();
			//textureIDForTimes.push_back(players[playerIndex].hand.back().getID());
			//In dealing addAnimation, you still need to define the textureID, startTime, width and height Divisors
			if (i + dealingAnimationID[START] <= dealingAnimationID[END]) {
				//Starts the vector of animations of the particular set of animations
				for (unsigned j = 0; j < animateObjectsList[dealingAnimationID[START] + i].size(); j++) {
					//Loops through each part of the vector animation and defines the undefined parts
					animateObjectsList[dealingAnimationID[START] + i][j].id = players[playerIndex].hand.back().getID();
					animateObjectsList[dealingAnimationID[START] + i][j].heightDivisor = cardHeight;
					animateObjectsList[dealingAnimationID[START] + i][j].widthDivisor = cardWidth;
				}
				startAnimation(i + dealingAnimationID[START]);
				//uses the startAnimation function
			}
			//textureIDForFinished.push_back(false);

			i += 1;
			waitTime += differenceInWaitTimes;
		}
		//update card movement

		heartsScreen.resetRenderer();
		drawStaticGame();

		//First draw the static cards
		if (_deck.getSize() != 0)
			heartsScreen.drawTexture(50, 50, 7, 7, _deck.getBackID(), CENTER, 0, false);

		//ifFinished = true;

		/*
		for (unsigned j = 0; j < textureIDForTimes.size(); j++) {
			double beginX = 50, beginY = 50, endX = 50, endY = 50;
			double theta;

			switch (j % 4) {
			case NORTH:
				theta = 180;
				endY = 15;
				break;
			case WEST:
				theta = 90;
				endX = 10;
				break;
			case SOUTH:
				theta = 0;
				endY = 85;
				break;
			case EAST:
				theta = 270;
				endX = 90;
				break;
			}

			double Xdist = endX - beginX;
			double Ydist = endY - beginY;

			double x = beginX + (Xdist * (heartsScreen.getElapsedTime(j) / totalTime));
			double y = beginY + (Ydist * (heartsScreen.getElapsedTime(j) / totalTime));

			if (abs(x - beginX) > abs(Xdist)) {
				x = endX;
				textureIDForFinished[j] = true;
			}
			else ifFinished = false;

			if (abs(y - beginY) > abs(Ydist)) {
				y = endY;
				textureIDForFinished[j] = true;
			}
			else ifFinished = false;

			heartsScreen.drawTexture(x, y, 7, 7, _deck.getBackID(), CENTER, theta, false);
		}
		*/

		renderAnimations();

		ifFinished = ifGroupAnimationFinished(dealingAnimationID[START], dealingAnimationID[END]);
		/*
		int numFinished = 0;
		for (unsigned i = 0; i < textureIDForFinished.size(); i++) {
			if (textureIDForFinished[i])
				numFinished += 1;
		}
		if (numFinished == textureIDForFinished.size())
			ifFinished = true;
			*/

		heartsScreen.update();
	}

	//Cards are all dynamic
	/*

	              Card Card

     Card             Deck          Card
	 Card                           Card

	              Card Card


	Velocity = DeltaX / DeltaT ( heartsScreen.getElapsedTime() )
	Distance = sqrt(x^2 + y^2)

	time it should take: t
	beginX, beginY, endX, endY

	Xdist = endX - beginX
	Ydist = endY - beginY

	x(t) = beginX + Xdist (timeElapsed/TimeItShouldTake);
	y(t) = beginY + Ydist (timeElapsed/TimeItShouldTake);


	*/
	//heartsScreen.clearStartTimes();
	heartsScreen.wait(500);
}
//The passing round
void Hearts::passTheCards() {
	//Background and words
	//drawStaticGame();
	//Draw other hands (which are also static)
	//Draw your hand (centered at the bottom)
	//You should be able to click on them, so you should use the highlight function you made
	//all handled in the user input
	//the actual logic change should also be handled in userInput
	//Each individual card will be chosen in one cycle of userInput/computerInput
	//South is always userInput others are always computer
	screen = PASSING;
	if (passingRound != TONONE) {
		for (int i = 0; i < 4; i++) {
			while (players[i].chosenCards.size() < 3) {
				int response;
				if (i == 0)
					response = userInput("pass");
				else
					response = computerInput("play");

				bool ifRepeat = false;
				//The chosenCards will climb on top of the hand indecies
				if (unsigned(response) >= players[i].hand.size()) {
					response -= players[i].hand.size();
					ifRepeat = true;
				}

				if (!ifRepeat) {
					players[i].chosenCards.push_back(players[i].hand[response]);
					players[i].hand[response] = players[i].hand.back();
					players[i].hand.pop_back();
				}
				else {
					players[i].hand.push_back(players[i].chosenCards[response]);
					players[i].chosenCards[response] = players[i].chosenCards.back();
					players[i].chosenCards.pop_back();
				}

				organizeHands();
				//The response numbers goes from 0-12

				//Draw the card things
				//Maybe animate the card to the center of the screen?
				//So show which cards you want to use
				//There are 13 objects you need to deal with (one for each card
				if (i == 0) {
					std::vector <AnimateObject> animateObject;
					for (int j = 0; j < players[i].hand.size() + players[i].chosenCards.size(); j++) {
						double totalTime = 500;
						bool ifChosenCard = false;
						int index = j;
						if (index >= players[i].hand.size()) {
							ifChosenCard = true;
							index -= players[i].hand.size();
						}

						double cardWidth = 7, cardHeight = 7;
						int beginX, beginY;
						double endX, endY;
						int id;
						
						if (!ifChosenCard)
							id = players[i].hand[index].getID();
						else
							id = players[i].chosenCards[index].getID();

						heartsScreen.getTexturePosition(beginX, beginY, id);

						endX = 0;
						endY = 0;

						animateObject.push_back(createAnimateObject(beginX, beginY, endX, endY, 0, totalTime, UPLEFT, id, cardWidth, cardHeight));
					}

					int id = addAnimation(animateObject);

					animateObject.clear();

					startAnimation(id);
					while (!ifAnimationFinished(id)) {
						heartsScreen.resetRenderer();
						drawStaticGame();
						renderAnimations();
						heartsScreen.update();
					}
					stopAnimation(id);
				}
				/*double cardWidth = 7, cardHeight = 7;

			heartsScreen.resetRenderer();
			drawStaticGame();

			double theta = 0, radians;
			double centerX, centerY, cardSeparation = 35;

			heartsScreen.highlightColor = { 255, 255, 0, 200 };

			for (unsigned i = 0; i < players.size(); i++) {
				//switch statement
				switch (i) {
				case 0:
					centerX = 0;
					centerY = 300;
					break;
				case 2:
					centerY = -300;
					centerX = 0;
					break;
				case 1:
					centerY = 0;
					centerX = -650;
					break;
				case 3:
					centerY = 0;
					centerX = 650;
					break;

				}
				radians = theta * M_PI / 180;
				double drawX = centerX - ((players[i].hand.size() - 1) * cardSeparation * cos(radians) / 2);
				double drawY = centerY - ((players[i].hand.size() - 1) * cardSeparation * sin(radians) / 2);

				for (unsigned j = 0; j < players[i].hand.size(); j++) {
					bool ifHighlighted = false;
					if (j == highlightedID && i == 0) ifHighlighted = true;
					int id;
					if (i == 0) id = players[i].hand[j].getID();
					else id = _deck.getBackID();
					heartsScreen.drawTexture(drawX, drawY, cardWidth, cardHeight, id, ROTATABLE, theta, ifHighlighted);
					drawX += cardSeparation * cos(radians);
					drawY += cardSeparation * sin(radians);
				}

				if (i == 0) {
					double cardSeparation = 100;
					centerY -= 300;
					drawX = centerX - ((players[i].chosenCards.size() - 1) * cardSeparation * cos(radians) / 2);
					drawY = centerY - ((players[i].chosenCards.size() - 1) * cardSeparation * sin(radians) / 2);
					for (unsigned j = 0; j < players[i].chosenCards.size(); j++) {
						bool ifHighlighted = false;
						if (j + players[i].hand.size() == highlightedID && i == 0) ifHighlighted = true;
						int id;
						if (i == 0) id = players[i].chosenCards[j].getID();
						else id = _deck.getBackID();
						heartsScreen.drawTexture(drawX, drawY, cardWidth, cardHeight, id, ROTATABLE, theta, ifHighlighted);
						drawX += cardSeparation * cos(radians);
						drawY += cardSeparation * sin(radians);
					}
				}
				theta += 90;
				*/
			}

			for (unsigned j = 0; j < players[i].chosenCards.size(); j++) {
				for (unsigned k = 0; k < players[i].hand.size(); k++) {
					if (players[i].chosenCards[j].getID() == players[i].hand[k].getID()) {
						players[i].hand.back() = players[i].hand[k];
						players[i].hand.pop_back();
					}
				}
			}

			int passingPlayerIndex;
			switch (passingRound) {
			case TOLEFT:
				passingPlayerIndex = (i + 1) % 4;
				break;
			case TORIGHT:
				passingPlayerIndex = (i + 3) % 4;
				break;
			case TOSTRAIGHT:
				passingPlayerIndex = (i + 2) % 4;
				break;
			}

			for (unsigned j = 0; j < players[i].chosenCards.size(); j++)
				players[passingPlayerIndex].cardsToBePassed.push_back(players[i].chosenCards[j]);

			players[i].chosenCards.clear();
		}
	}

	for (unsigned i = 0; i < players.size(); i++) {
		for (unsigned j = 0; j < players[i].cardsToBePassed.size(); j++)
			players[i].hand.push_back(players[i].cardsToBePassed[j]);

		players[i].cardsToBePassed.clear();
	}

	passingRound = (passingRound + 1) % 4;
}
//Play hand
void Hearts::playHand() {
	for (int i = 0; i < 4; i++) {
		int response;
		//prompt for the card to put forth
		if (i == 0)
			response = userInput("play");
		else
			response = computerInput("play");

		//There are certain cards you can't chose
		//For instance, the first card has to be 2 of clubs
		//also you can't do a card first if the hearts weren't broken

		//also animate the putting card forward?


	}
	//play all of the cards
}

//play entire round
void Hearts::playRound() {
	//play all of the hands
	screen = PLAYING;
	ifHeartsBroken = true;
	while (players[0].hand.size() != 0) {
		playHand();
	}
}

//play game
void Hearts::playGame() {
	bool ifWinner = false;
	//Pass the cards (or don't)
	//play all of the rounds
	while (!ifWinner) {
		dealCards();
		organizeHands();
		passTheCards();
		playRound();
		//This
		rankPlayers();
		_deck.shuffle();
	}
	// 0  is false, 1 is true
	//to the question, continue playing?
	int response = userInput("continueGame");
	//Then you'll use the rankings to show who won or whatever
}

//Will return the id of the card in the hand that the players wishes to select
int Hearts::userInput(std::string move) {
	int chosenID = -1;
	int highlightedID = -1;
	bool quit = false;

	SDL_Event e;

	if (move == "pass" || move == "play") {
	// -1 is no cards highlighted
		while (!quit) {

			int x, y;
			SDL_GetMouseState(&x, &y);

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0) {

				//User requests quit
				if (e.type == SDL_QUIT)
					quit = true;
				//User unpresses
				if (e.type == SDL_MOUSEBUTTONUP) {
					for (unsigned i = 0; i < players[0].hand.size(); i++) {
						if (heartsScreen.ifInsideTexture(x, y, players[0].hand[i].getID())) {
							chosenID = i;
							quit = true;
						}
					}
					for (unsigned i = 0; i < players[0].chosenCards.size(); i++) {
						if (heartsScreen.ifInsideTexture(x, y, players[0].chosenCards[i].getID())) {
							chosenID = i + players[0].hand.size();
							quit = true;
						}
					}

				}
			}

			highlightedID = -1;
			for (unsigned i = 0; i < players[0].hand.size(); i++) {
				if (heartsScreen.ifInsideTexture(x, y, players[0].hand[i].getID()))
					highlightedID = i;
			}
			for (unsigned i = 0; i < players[0].chosenCards.size(); i++) {
				if (heartsScreen.ifInsideTexture(x, y, players[0].chosenCards[i].getID()))
					highlightedID = i + players[0].hand.size();
			}

			double cardWidth = 7, cardHeight = 7;

			heartsScreen.resetRenderer();
			drawStaticGame();

			double theta = 0, radians;
			double centerX, centerY, cardSeparation = 35;

			heartsScreen.highlightColor = { 255, 255, 0, 200 };

			for (unsigned i = 0; i < players.size(); i++) {
				//switch statement
				switch (i) {
				case 0:
					centerX = 0;
					centerY = 300;
					break;
				case 2:
					centerY = -300;
					centerX = 0;
					break;
				case 1:
					centerY = 0;
					centerX = -650;
					break;
				case 3:
					centerY = 0;
					centerX = 650;
					break;

				}
				radians = theta * M_PI / 180;
				double drawX = centerX - ((players[i].hand.size() - 1) * cardSeparation * cos(radians) / 2);
				double drawY = centerY - ((players[i].hand.size() - 1) * cardSeparation * sin(radians) / 2);

				for (unsigned j = 0; j < players[i].hand.size(); j++) {
					bool ifHighlighted = false;
					if (j == highlightedID && i == 0) ifHighlighted = true;
					int id;
					if (i == 0) id = players[i].hand[j].getID();
					else id = _deck.getBackID();
					heartsScreen.drawTexture(drawX, drawY, cardWidth, cardHeight, id, ROTATABLE, theta, ifHighlighted);
					drawX += cardSeparation * cos(radians);
					drawY += cardSeparation * sin(radians);
				}

				if (i == 0) {
					double cardSeparation = 100;
					centerY -= 300;
					drawX = centerX - ((players[i].chosenCards.size() - 1) * cardSeparation * cos(radians) / 2);
					drawY = centerY - ((players[i].chosenCards.size() - 1) * cardSeparation * sin(radians) / 2);
					for (unsigned j = 0; j < players[i].chosenCards.size(); j++) {
						bool ifHighlighted = false;
						if (j + players[i].hand.size() == highlightedID && i == 0) ifHighlighted = true;
						int id;
						if (i == 0) id = players[i].chosenCards[j].getID();
						else id = _deck.getBackID();
						heartsScreen.drawTexture(drawX, drawY, cardWidth, cardHeight, id, ROTATABLE, theta, ifHighlighted);
						drawX += cardSeparation * cos(radians);
						drawY += cardSeparation * sin(radians);
					}
				}
				theta += 90;
			}
			heartsScreen.update();
		}
		//You'll be able to select any of the cards all the time
		//treat them like buttons
		if (move == "pass") {

		}
		else if (move == "play") {

		}
	}
	else if (move == "continueRound") {
		screen = RANKING;
		//Just show the rankings long enough for the user to want to continue
		//use a single continue button
		//Create a box to use as a prompt box
		//Box and rankings can be handled in the drawStaticGame function
	}
	else if (move == "continueGame") {
		screen = WINNING;
		//show winner ( with drawStaticGame() ) 
		//Ask the player if he/she wants to play again
	}

	return chosenID;
}

//Will return id of the card
int Hearts::computerInput(std::string move) {
	int chosenID = 0;
	if (move == "pass") {

	}
	else if (move == "play") {

	}
	return chosenID;
}

//rank the players
//This includes adding the hearts to the players and then checking the order in which they are winning
//It will return whether or not the max player has exceeded the limit
bool Hearts::rankPlayers() {
	bool ifFinished = false;
	std::vector <int> numHeartsList;
	//check the takenCards for hearts 
	for (unsigned i = 0; i < players.size(); i++) {
		int numHearts = 0;
		for (unsigned j = 0; j < players[i].takenCards.size(); j++) {

			std::string suit = players[i].takenCards[j].getSuit();
			int rank = players[i].takenCards[j].getRank();

			if (suit == "hearts")
				numHearts += 1;
			else if (suit == "spades" && rank == 12)
				numHearts += 13;
			else if (suit == "diamonds" && rank == 11)
				numHearts -= 10;

			_deck.putCardOnBottom(players[i].takenCards[j]);
		}

		players[i].numHearts += numHearts;
		//The index of this vector will match the players vector
		numHeartsList.push_back(players[i].numHearts);
		players[i].takenCards.clear();
	}
	for (unsigned j = 0; j < players.size(); j++) {
		int max = 0;
		int maxID = -1;
		for (unsigned i = 0; i < numHeartsList.size(); i++) {
			if (max < numHeartsList[i]) {
				maxID = i;
				max = numHeartsList[i];
				if (max >= maxHeartValue)
					ifFinished = true;
			}
		}
		players[maxID].ranking = numHeartsList.size();
		numHeartsList.back() = numHeartsList[maxID];
		numHeartsList.pop_back();
	}

	screen = RANKING;
	userInput("continueRound");

	return ifFinished;
}

//Draw the game that is not moving 
void Hearts::drawStaticGame() {
	heartsScreen.drawBackground(255, 0, 0, tableID);
	//use draw text to make this look nicer
	std::stringstream ss;

	switch (screen) {
	case DEALING:
		//Just random text while the cards are being dealt
		ss << "Dealing . . .";
		break;
	case PASSING:
		//Shows which you are passing to
		ss << "Pass three cards ";
		if (passingRound == TOLEFT)
			ss << "to the left";
		else if (passingRound == TORIGHT)
			ss << "to the right";
		else if (passingRound == TOSTRAIGHT)
			ss << "straight ahead";
		else if (passingRound == TONONE)
			ss << "to nobody";
		break;
	case PLAYING:
		//Text during the actual game
		//Including what cards you can or cannot do?
		//Also showing whether or not hearts are broken
		break;
	case RANKING:
		//This means it'll show who is in the lead and how many points everyone has
		break;
	case WINNING:
		break;
	}
	
	heartsScreen.highlightColor = { 0, 255, 0, 0 };
	heartsScreen.drawText(ss.str(), { 0, 0, 0 }, 0, 50, 65, 10, 10, CENTER, 0, false);
}

void Hearts::organizeHands() {
	for (unsigned i = 0; i < players.size(); i++) {
		std::vector <Card> holdingHand = players[i].hand;
		players[i].hand.clear();

		//First Clubs, then Diamonds, then Spades, then Hearts
		for (int suitIndex = 0; suitIndex < 4; suitIndex++) {
			std::string suit;
			switch (suitIndex) { //Switch for all of the suits
			case 0:
				suit = "clubs";
				break;
			case 1:
				suit = "diamonds";
				break;
			case 2:
				suit = "spades";
				break;
			case 3:
				suit = "hearts";
				break;
			}
			int numOtherCards = -1;

			while (numOtherCards != 0) {

				numOtherCards = -1;
				int min = 15;
				int minIndex = -1;
				for (unsigned j = 0; j < holdingHand.size(); j++) {
					if (holdingHand[j].getSuit() == suit) {
						numOtherCards += 1;

						int rank = holdingHand[j].getRank();
						if (rank == 1) rank = 14;

						if (rank < min) {
							minIndex = j;
							min = rank;
						}
					}
				}
				if (numOtherCards == -1)
					numOtherCards = 0;
				else {
					players[i].hand.push_back(holdingHand[minIndex]);
					holdingHand[minIndex] = holdingHand.back();
					holdingHand.pop_back();
				}
			}
		}
	}
}

int Hearts::addAnimation(std::vector <AnimateObject> animateObjects) {
	int animationID = animateObjectsList.size();
	animateObjectsList.push_back(animateObjects);

	for (unsigned i = 0; i < animateObjectsList[animationID].size(); i++)
		animateObjectsList[animationID][i].animationID = animationID;

	return animationID;
}

void Hearts::startAnimation(int animationID) {
	//animateObjectsList[animationID] the list of animations you are using
	int startTimeID = heartsScreen.addStartTime();
	for (unsigned i = 0; i < animateObjectsList[animationID].size(); i++)
		animateObjectsList[animationID][i].startTimeID = startTimeID;

}

void Hearts::stopAnimation(int animationID) {
	//resets startTimeID to initial value
	for (unsigned i = 0; i < animateObjectsList[animationID].size(); i++)
		animateObjectsList[animationID][i].startTimeID = -1;
}

void Hearts::renderAnimations() {
	for (unsigned i = 0; i < animateObjectsList.size(); i++) {
		for (unsigned j = 0; j < animateObjectsList[i].size(); j++) {
			if (animateObjectsList[i][j].startTimeID != -1) {
				double beginX = animateObjectsList[i][j].beginX, beginY = animateObjectsList[i][j].beginY;
				double endX = animateObjectsList[i][j].endX, endY = animateObjectsList[i][j].endY;
				int startTimeID = animateObjectsList[i][j].startTimeID;
				double totalTime = animateObjectsList[i][j].totalTime;

				double Xdist = endX - beginX;
				double Ydist = endY - beginY;

				double x = beginX + (Xdist * (heartsScreen.getElapsedTime(startTimeID) / totalTime));
				double y = beginY + (Ydist * (heartsScreen.getElapsedTime(startTimeID) / totalTime));

				if (abs(x - beginX) >= abs(Xdist)) {
					x = endX;
					animateObjectsList[i][j].ifFinished = true;
				}
				else 
					animateObjectsList[i][j].ifFinished = false;

				if (abs(y - beginY) >= abs(Ydist)) {
					y = endY;
					animateObjectsList[i][j].ifFinished = true;
				}
				else
					animateObjectsList[i][j].ifFinished = false;

				heartsScreen.drawTexture(x, y, 7, 7, animateObjectsList[i][j].id, CENTER, animateObjectsList[i][j].theta, animateObjectsList[i][j].ifHighlighted);
			}
		}
	}
}

/*struct AnimateObject {
		//texture id of the object in question
		int id;
		//animation id
		int animationID;
		double beginX, beginY;
		double endX, endY;
		double theta;
		//Time it will take to complete the animation for this specific object
		double totalTime;
		//What allignment is being used when drawing the textures
		int allignment;
		//what id it is in the startTimes vector
		//this is -1 if it hasn't started yet
		int startTimeID = -1;
		bool ifFinished = false;
		bool ifHighlighted = false;
	};*/

//create an animateObject (the easier way)
Hearts::AnimateObject Hearts::createAnimateObject(double beginX, double beginY, double endX, double endY, double theta, double totalTime, int allignment) {
	AnimateObject animateObject = {};
	animateObject.beginX = beginX;
	animateObject.beginY = beginY;
	animateObject.endX = endX;
	animateObject.endY = endY;
	animateObject.theta = theta;
	animateObject.totalTime = totalTime;
	animateObject.allignment = allignment;

	return animateObject;
}

Hearts::AnimateObject Hearts::createAnimateObject(double beginX, double beginY, double endX, double endY, double theta, double totalTime, int allignment, int id, double widthDivisor, double heightDivisor) {
	AnimateObject animateObject = {};
	animateObject.beginX = beginX;
	animateObject.beginY = beginY;
	animateObject.endX = endX;
	animateObject.endY = endY;
	animateObject.theta = theta;
	animateObject.totalTime = totalTime;
	animateObject.allignment = allignment;
	animateObject.id = id;
	animateObject.widthDivisor = widthDivisor;
	animateObject.heightDivisor = heightDivisor;

	return animateObject;
}

bool Hearts::ifAnimationFinished(int animationID) {
	bool ifFinished = true;
	for (int i = 0; i < animateObjectsList[animationID].size(); i++) {
		if (!animateObjectsList[animationID][i].ifFinished)
			ifFinished = false;
	}

	return ifFinished;
}


bool Hearts::ifGroupAnimationFinished(int startAnimationID, int endAnimationID) {
	bool ifFinished = true;
	for (int i = startAnimationID; i <= endAnimationID; i++) {
		if (!ifAnimationFinished(i))
			ifFinished = false;
	}
	return ifFinished;
}
