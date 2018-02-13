#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include <stdio.h>
#include "Card.h"
#include "Deck.h"
#include "Screen.h"

class BlackJack
{
public:
	BlackJack();
	void init(int numPlayers, Deck deck);

	~BlackJack();

	void restartHands();
	bool playTurn();
	void dealCards();
	void drawGame();

	//setters
	void revealCards(bool ifEnd);
	//getters
	int getNumPlayers() { return _playerCards.size(); }
	int getNumCards(int player) { return _playerCards[player].size(); }

private:
	//screen for BlackJack
	Screen blackJackScreen;

	//Using cin to get information for turn
	std::string userInput(std::string move);
	//Using logic to get information for turn
	std::string computerInput(std::string move);

	//Choose the amount of money to bet
	bool bet(std::string input, int player);
	//Choose hit or stay
	bool play(std::string input, int player);
	//update the values of the amounts
	void updateValues();
	int checkWinner();
	//enumeration so I don't mix up the players
	enum _players { PLAYER, DEALER, OPPONENTONE, OPPONENTTWO, OPPONENTTHREE, OPPONENTFOUR, OPPONENTFIVE };
	//The deck to be used for the game
	Deck _deck;
	//First vector is the players, second vector is the cards each of them has.
	std::vector<std::vector <Card>> _playerCards;
	//Everyone will start with $500
	std::vector<int> _playerMoney;
	//The amount that each player has (from 4-21 or over).
	std::vector<int> _playerValue;

	struct Player {
		std::vector <Card> hand;
		int money;
		int value;
	};
	int winner = -1;

	//Bet that you do
	int playerBet;

	//file path for gambling table image
	std::string backgroundFilePath = "MainMenu/GamblingTable.png";
	//id for the gambling table texture
	int tableID;
	//id for the hit button
	int hitID;
	int selectHitID;
	int highlightHitID;
	std::string hitFilePath = "GameButtons/hit.png";
	std::string selectHitFilePath = "GameButtons/hitSelected.png";
	std::string highlightHitFilePath = "GameButtons/hitHighlighted.png";
	//id for the stay button
	int stayID;
	int selectStayID;
	int highlightStayID;
	std::string stayFilePath = "GameButtons/stay.png";
	std::string selectStayFilePath = "GameButtons/staySelected.png";
	std::string highlightStayFilePath = "GameButtons/stayHighlighted.png";
	//id for the bet button
	int betID;
	int selectBetID;
	int highlightBetID;
	std::string betFilePath = "GameButtons/bet.png";
	std::string selectBetFilePath = "GameButtons/betSelected.png";
	std::string highlightBetFilePath = "GameButtons/betHighlighted.png";
	//id for the play button
	int playID;
	int selectPlayID;
	int highlightPlayID;
	std::string playFilePath = "GameButtons/play.png";
	std::string selectPlayFilePath = "GameButtons/playSelected.png";
	std::string highlightPlayFilePath = "GameButtons/playHighlighted.png";
};

