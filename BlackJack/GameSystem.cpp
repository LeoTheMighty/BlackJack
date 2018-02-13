#include <random>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include "GameSystem.h"

GameSystem::GameSystem()
{
	_gameName = "";
}

void GameSystem::init(std::string gameName, int numPlayers) {
	_gameName = gameName;
	_deck.shuffle();

	if (gameName == "BlackJack")
		_blackJack.init(numPlayers, _deck);

	else if (gameName == "Hearts")
		_hearts.init(_deck);

	else if (gameName == "Poker")
		_poker.init(numPlayers, _deck);

	else {
		printf("ERROR, could not initialize gamesystem");
		system("PAUSE");
	}
}

GameSystem::~GameSystem()
{

}

void GameSystem::playGame() {
	/*
	//Is this a bad idea?

	switch (_gameName[0]) {
	case 'B':
		playBlackJackGame();
		break;
	case 'H':
		playHeartsGame();
		break;
	case 'P':
		playPokerGame();
		break;
	}
	*/

	if (_gameName == "Poker")
		playPokerGame();
	else if (_gameName == "BlackJack")
		playBlackJackGame();
	else if (_gameName == "Hearts")
		playHeartsGame();

}

void GameSystem::playBlackJackGame()
{
	//loop for each turn that happens
	bool ifPlaying = true;
	while (ifPlaying) {
		ifPlaying = _blackJack.playTurn();
	}
}

void GameSystem::playPokerGame()
{
	int blind = 20;
	int counter = 0;
	bool ifPlayAgain = true;
	while (ifPlayAgain) {
		ifPlayAgain = _poker.playGame(blind);
		if (counter == 5) {
			counter = 0;
			blind += 20;
		}
		counter += 1;
	}
}
void GameSystem::playHeartsGame()
{
	_hearts.playGame();
}