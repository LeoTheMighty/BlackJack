#include <iostream>
#include <ctime>
#include <random>
#include <sstream>
#include "BlackJack.h"

BlackJack::BlackJack()
{
	blackJackScreen = Screen();
}

void BlackJack::init(int numPlayers, Deck deck)
{
	_deck = deck;

	//Initialize the black jack screen
	blackJackScreen.init();

	//Initialize the bet
	playerBet = 0;

	//Load all the textures for the cards
	for (int i = 0; i < _deck.getSize(); i++) {
		int id = blackJackScreen.addTexture(_deck.lookAtCard(i).getImageFileName());

		if (id >= 0) {
			_deck.setCardID(i, id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for cards! SDL Error: " << SDL_GetError() << "\n";
	}

	{
		int id = blackJackScreen.addTexture(_deck.getBackImageFileName());

		if (id >= 0) {
			_deck.setBackID(id);
		}
		else 
			std::cout << "Failed to initialize SDL Textures for back! SDL Error: " << SDL_GetError() << "\n";

	}

	{
		int id = blackJackScreen.addTexture(backgroundFilePath);

		if (id >= 0) {
			tableID = (id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for background! SDL Error: " << SDL_GetError() << "\n";

	}

	{
		int id1 = blackJackScreen.addTexture(hitFilePath);
		int id2 = blackJackScreen.addTexture(selectHitFilePath);
		int id3 = blackJackScreen.addTexture(highlightHitFilePath);

		if (id1 >= 0 && id2 >= 0 && id3 >= 0) {
			hitID = id1;
			selectHitID = id2;
			highlightHitID = id3;
		}
		else
			std::cout << "Failed to initialize SDL Textures for hit button! SDL Error: " << SDL_GetError() << "\n";

	}

	{
		int id1 = blackJackScreen.addTexture(stayFilePath);
		int id2 = blackJackScreen.addTexture(selectStayFilePath);
		int id3 = blackJackScreen.addTexture(highlightStayFilePath);

		if (id1 >= 0 && id2 >= 0 && id3 >= 0) {
			stayID = id1;
			selectStayID = id2;
			highlightStayID = id3;
		}
		else
			std::cout << "Failed to initialize SDL Textures for stay button! SDL Error: " << SDL_GetError() << "\n";

	}

	{
		int id1 = blackJackScreen.addTexture(betFilePath);
		int id2 = blackJackScreen.addTexture(selectBetFilePath);
		int id3 = blackJackScreen.addTexture(highlightBetFilePath);

		if (id1 >= 0 && id2 >= 0 && id3 >= 0) {
			betID = id1;
			selectBetID = id2;
			highlightBetID = id3;
		}
		else
			std::cout << "Failed to initialize SDL Textures for bet button! SDL Error: " << SDL_GetError() << "\n";

	}

	{
		int id1 = blackJackScreen.addTexture(playFilePath);
		int id2 = blackJackScreen.addTexture(selectPlayFilePath);
		int id3 = blackJackScreen.addTexture(highlightPlayFilePath);

		if (id1 >= 0 && id2 >= 0 && id3 >= 0) {
			playID = id1;
			selectPlayID = id2;
			highlightPlayID = id3;
		}
		else
			std::cout << "Failed to initialize SDL Textures for play button! SDL Error: " << SDL_GetError() << "\n";

	}
	for (int i = 0; i < numPlayers; i++) {
		std::vector <Card> hand;
		_playerCards.push_back(hand);
		_playerMoney.push_back(500);
		_playerValue.push_back(0);
	}

	//dealCards();
}


BlackJack::~BlackJack()
{

}

void BlackJack::restartHands() {
	for (unsigned i = 0; i < _playerCards.size(); i++) {
		while (_playerCards[i].size() > 0) {
			_deck.putCardOnBottom(_playerCards[i].back());
			_playerCards[i].pop_back();
		}
	}
	_deck.shuffle();
	//dealCards();
}
bool BlackJack::playTurn()
{
	//drawGame();
	bool ifPlaying = true;
	//Handle the input
	for (int i = 0; i < getNumPlayers(); i++) {
		//handle the moves they make
		if (i == PLAYER) {
			//userInput will take in a variable about whether it's a bet or a hit/stay
			playerBet = 0;
			drawGame();

			while (bet(userInput("bet"), i)) {
				drawGame();
			}
			restartHands();
			dealCards();
			revealCards(false);
			updateValues();
			//loops until returns false, which will either be staying or busting
			while (play(userInput("play"), i)) {
				updateValues();
			}

		}
		//use input to handle move
		//loop for each "hit" "stay" or "bet"

		else {
			//So will computerInput like ^^
			bet(computerInput("bet"), i);
			//loops until returns false, which will either be staying or busting
			while (play(computerInput("play"), i));
		}
	}
	//check who actually won the hand
	updateValues();
	//Change the game
	revealCards(true);
	winner = checkWinner();
	_playerMoney[winner] += playerBet * 2;
	if (winner == PLAYER) {
	}
	else if (winner == DEALER) {

	}
	//Draw the game
	drawGame();
	//Prompt for the next input

	return ifPlaying;
}

void BlackJack::drawGame()
{
	//Draw the game
	/*
	
	DECK       Covered Card Uncovered Card


	Hit Card  Hit Card  Hit Card  Hit Card  Hit Card  Hit Card


	
	          Uncovered Card Uncovered Card
	*/

	//Draw the background
	blackJackScreen.drawBackground(255, 178, 102, tableID);
	double x = 0;
	double cardWidth = 7, cardHeight = 6;

	enum allignmentFlag { UPLEFT, CENTER, ROTATABLE };

	//Draw the player's cards
	x = 50 - (_playerCards[PLAYER].size() - 1) * 3.5;
	for (unsigned i = 0; i < _playerCards[PLAYER].size(); i++) {
		if (_playerCards[PLAYER][i].getIfCovered())
			blackJackScreen.drawTexture(x, 85, cardWidth, cardHeight, _deck.getBackID(), CENTER, 0, false);
		else 
			blackJackScreen.drawTexture(x, 85, cardWidth, cardHeight, _playerCards[PLAYER][i].getID(), CENTER, 0, false);
		x += 7;
	}

	//blackJackScreen.drawCard(100, 50, _deck.lookAtCard(0).getID(), UPLEFT);
	//blackJackScreen.drawCard(50, 50, _deck.lookAtCard(2).getID(), CENTER);
	//blackJackScreen.drawCard(0, 50, _deck.lookAtCard(1).getID(), UPLEFT);

	int j = _deck.getSize() - 1;
	for (int i = _deck.getSize() - 1; i >= 0; i--) {
		blackJackScreen.drawTexture((j - i) * 0.1, 0, cardWidth, cardHeight, _deck.getBackID(), UPLEFT, 0, false);
	}



	//Draw the dealer's cards
	x = 50 - (_playerCards[DEALER].size() - 1) * 3.5;
	for (unsigned i = 0; i < _playerCards[DEALER].size(); i++) {
		if (_playerCards[DEALER][i].getIfCovered())
			blackJackScreen.drawTexture(x, 15, cardWidth, cardHeight, _deck.getBackID(), CENTER, 0, false);
		else
			blackJackScreen.drawTexture(x, 15, cardWidth, cardHeight, _playerCards[DEALER][i].getID(), CENTER, 0, false);
		x += 7;
	}
	SDL_Color color = { 255, 255, 255 };
	std::stringstream ss;
	ss << "bet: " << playerBet;
	blackJackScreen.drawText(ss.str(), color, 0, 90, 25, 15, 15, UPLEFT, 0, false);
	ss.str("");
	//updateValues();
	ss << "Value: ";
	if (_playerValue[PLAYER] > 21)
		ss << "BUST";
	else
		 ss << _playerValue[PLAYER];
	blackJackScreen.drawText(ss.str(), color, 0, 90, 15, 15, 15, UPLEFT, 0, false);
	ss.str("");
	ss << "Money: " << _playerMoney[PLAYER];
	blackJackScreen.drawText(ss.str(), color, 0, 90, 5, 15, 15, UPLEFT, 0, false);
	ss.str("");

	ss << "Dealer's money: " << _playerMoney[DEALER];
	blackJackScreen.drawText(ss.str(), color, 0, 15, 70, 15, 15, UPLEFT, 0, false);
	ss.str("");
	/*
	ss << "Other Value: ";
	if (_playerValue[DEALER] > 21)
		ss << "BUST";
	else
		ss << _playerValue[DEALER];

	blackJackScreen.drawText(ss.str(), color, 0, 15, 50, 15, 15, UPLEFT);
	ss.str("");
	*/

	//draw the buttons
	//blackJackScreen.drawTexture(95, 70, 10, 10, hitID, UPLEFT);
	//blackJackScreen.drawTexture(95, 90, 10, 10, stayID, UPLEFT);
	//blackJackScreen.drawCard(0, 0, _deck.getBackID());
	//blackJackScreen.update();
	//system("PAUSE");
	//Maybe use some SDL??? :))
}

void BlackJack::dealCards()
{
	//give each player two cards to start with
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < getNumPlayers(); i++) {
			Card card = _deck.takeCardFromTop();
			_playerCards[i].push_back(card);
			//if (j == 0) {
				//if (i == PLAYER)
					//_playerCards[i].back().reveal();
			//}
			//else {
				//_playerCards[i].back().reveal();
			//}
		}
	}
}

std::string BlackJack::userInput(std::string move)
{
	enum allignmentFlag { UPLEFT, CENTER, ROTATABLE };

	bool quit = false;

	bool ifMouseIsPressedDown = false;

	std::string response = "";

	//Event handler
	SDL_Event e;
	enum events { NONE, HITSELECT, HITHIGHLIGHT, STAYSELECT, STAYHIGHLIGHT, BETSELECT, BETHIGHLIGHT, PLAYSELECT, PLAYHIGHLIGHT };
	int currentEvent = NONE;

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
			}

			if (move == "play") {
				if (e.type == SDL_MOUSEBUTTONUP) {
					if (blackJackScreen.ifInsideTexture(x, y, hitID)) {
						response = "hit";
						quit = true;
					}
					if (blackJackScreen.ifInsideTexture(x, y, stayID)) {
						response = "stay";
						quit = true;
					}
				}
			}

			else if (move == "bet") {
				if (e.type == SDL_MOUSEBUTTONUP) {
					if (blackJackScreen.ifInsideTexture(x, y, playID)) {
						response = "play";
						quit = true;
					}
					if (blackJackScreen.ifInsideTexture(x, y, betID)) {
						response = "bet";
						quit = true;
					}
				}
			}
		}

		if (move == "play") {
			if (blackJackScreen.ifInsideTexture(x, y, hitID)) {
				if (ifMouseIsPressedDown)
					currentEvent = HITHIGHLIGHT;
				else
					currentEvent = HITSELECT;
			}

			else if (blackJackScreen.ifInsideTexture(x, y, stayID)) {
				if (ifMouseIsPressedDown)
					currentEvent = STAYHIGHLIGHT;
				else
					currentEvent = STAYSELECT;
			}
			else
				currentEvent = NONE;
		}
		else if (move == "bet") {
			if (blackJackScreen.ifInsideTexture(x, y, playID)) {
				if (ifMouseIsPressedDown)
					currentEvent = PLAYHIGHLIGHT;
				else
					currentEvent = PLAYSELECT;
			}

			else if (blackJackScreen.ifInsideTexture(x, y, betID)) {
				if (ifMouseIsPressedDown)
					currentEvent = BETHIGHLIGHT;
				else
					currentEvent = BETSELECT;
			}
			else
				currentEvent = NONE;
		}

		//blackJackScreen.drawTexture(95, 70, 10, 10, hitID, UPLEFT);
		//blackJackScreen.drawTexture(95, 90, 10, 10, stayID, UPLEFT);

		//resets renderer
		blackJackScreen.resetRenderer();

		//draws background and everything
		drawGame();
		if (move == "play") {
			if (currentEvent == HITSELECT) blackJackScreen.drawTexture(95, 70, 10, 10, selectHitID, UPLEFT, 0, false);
			else if (currentEvent == HITHIGHLIGHT) blackJackScreen.drawTexture(95, 70, 10, 10, highlightHitID, UPLEFT, 0, false);
			else blackJackScreen.drawTexture(95, 70, 10, 10, hitID, UPLEFT, 0, false);

			if (currentEvent == STAYSELECT) blackJackScreen.drawTexture(95, 90, 10, 10, selectStayID, UPLEFT, 0, false);
			else if (currentEvent == STAYHIGHLIGHT) blackJackScreen.drawTexture(95, 90, 10, 10, highlightStayID, UPLEFT, 0, false);
			else blackJackScreen.drawTexture(95, 90, 10, 10, stayID, UPLEFT, 0, false);

		}
		else if (move == "bet") {
			if (currentEvent == PLAYSELECT) blackJackScreen.drawTexture(95, 70, 10, 10, selectPlayID, UPLEFT, 0, false);
			else if (currentEvent == PLAYHIGHLIGHT) blackJackScreen.drawTexture(95, 70, 10, 10, highlightPlayID, UPLEFT, 0, false);
			else blackJackScreen.drawTexture(95, 70, 10, 10, playID, UPLEFT, 0, false);

			if (currentEvent == BETSELECT) blackJackScreen.drawTexture(95, 90, 10, 10, selectBetID, UPLEFT, 0, false);
			else if (currentEvent == BETHIGHLIGHT) blackJackScreen.drawTexture(95, 90, 10, 10, highlightBetID, UPLEFT, 0, false);
			else blackJackScreen.drawTexture(95, 90, 10, 10, betID, UPLEFT, 0, false);

			if (winner != -1) {
				SDL_Color color = { 255, 255, 255 };
				if (winner == PLAYER)
					blackJackScreen.drawText("Player won!", color, 0, 50, 50, 15, 15, CENTER, 0, false);
				else if (winner == DEALER)
					blackJackScreen.drawText("Dealer won!", color, 0, 50, 50, 15, 15, CENTER, 0, false);
			}
		}
		blackJackScreen.update();
	}

	//using cin to gather data for turn
	//OR you could totally use SDL
	return response;
}
std::string BlackJack::computerInput(std::string move)
{
	static std::default_random_engine randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> roll(0, 1);
	int flag = roll(randomEngine);
	if (move == "bet") {

		return "play";
	}

	else if (move == "play") {
		if (flag == 0)
			return "hit";
		else
			return "stay";
	}
	return "";
}

bool BlackJack::bet(std::string input, int player)
{
	if (input == "bet") {
		_playerMoney[player] -= 10;
		playerBet += 10;
		return true;
	}

	else 
		return false;
}

bool BlackJack::play(std::string input, int player)
{
	bool ifContinue = true;
	if (input == "hit") {
		_playerCards[player].push_back(_deck.takeCardFromTop());
		_playerCards[player].back().reveal();
	}
	if (input == "stay")
		ifContinue = false;

	return ifContinue;
	//choose hit or stay (this is looped)
}


void BlackJack::updateValues()
{
	for (int i = 0; i < getNumPlayers(); i++) {
		int numAces = 0;
		int amount = 0;
		for (int j = 0; j < getNumCards(i); j++) {
			amount += _playerCards[i][j].getBlackJackValue();
			if (_playerCards[i][j].getBlackJackValue() == 1)
				numAces += 1;
		}
		while (amount > 21 && numAces > 0) {
			amount -= 10;
			numAces -= 1;
		}

		_playerValue[i] = amount;
	}
}

void BlackJack::revealCards(bool ifEnd) {
	for (int i = 0; i < getNumPlayers(); i++) {
		if (!ifEnd) {
			_playerCards[i][0].reveal();
			if (i == PLAYER)
				_playerCards[i][1].reveal();
		}
		else {
			for (unsigned j = 0; j < _playerCards[i].size(); j++) {
				_playerCards[i][j].reveal();
			}
		}
	}
}

int BlackJack::checkWinner() {
	int maxValue = 0;
	int maxValueID = -1;
	for (unsigned i = 0; i < _playerCards.size(); i++) {
		if (_playerValue[i] > maxValue && _playerValue[i] < 22) {
			maxValue = _playerValue[i];
			maxValueID = i;
		}
	}

	return maxValueID;
}