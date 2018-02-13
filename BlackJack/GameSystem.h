#pragma once
#include <string>
#include <vector>
#include "BlackJack.h"
#include "Poker.h"
#include "Hearts.h"
#include "Card.h"
#include "Deck.h"

class GameSystem
{
public:
	GameSystem();

	void init(std::string gameName, int numPlayers);

	~GameSystem();

	void playGame();
	
	//setters

	//getters

private:

	void playBlackJackGame();
	void playPokerGame();
	void playHeartsGame();

	//Which game is being played
	std::string _gameName;
	//Deck used for any of the games
	Deck _deck;
	//BlackJack instance
	BlackJack _blackJack;
	Poker _poker;
	Hearts _hearts;
};

