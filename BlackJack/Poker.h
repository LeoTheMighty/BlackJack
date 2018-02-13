#pragma once
#include <string>
#include <vector>
#include "Screen.h"
#include "Card.h"
#include "Deck.h"

class Poker
{
public:
	Poker();
	void init(int numPlayers, Deck deck);
	void dealCards();
	void restartHand();
	void removePlayer(int index);
	//Plays until a winner is decided in the hand
	bool playGame(int blind);
	bool playRound();

	std::string userInput(std::string move);
	std::string computerInput(std::string move, int player);

	//Returns the ID of the player who won the hand in the end
	int whoWon();

	void updateValues();
	void updateRankings();
	bool drawGame();
	//Organizes a hand by order of rank
	void organizeHand(std::vector <Card> &playerHand);
	bool checkIfBest(Card card, int player);
	~Poker();
private:
	Screen pokerScreen;
	int _numPlayers;
	Deck _deck;

	int roundNumber = 1;

	enum handHierarchy { STRAIGHTFLUSH, FOUROFAKIND, FULLHOUSE, FLUSH, STRAIGHT, THREEOFAKIND, TWOPAIR, PAIR, HIGHCARD };
	enum roundFlags { HOLE, FLOP, TURN, RIVER, REVEAL };
	enum blindFlags { DEALER, BIGBLIND, SMALLBLIND, REGULAR, DEALERANDBIGBLIND };
	enum valueFlags { HANDTYPE, HIGHSET };
	enum allignmentFlag { UPLEFT, CENTER, ROTATABLE };
	int round;
	//money that you get for winning the hand
	int pot;

	//Blind value
	int blind;
	//Small blind is half of the blind, so make sure it's divisble by 2

	//ID for the player you play as
	int playerID = 0;

	//Bet that shows up on the screen before you decide whether to actually bet it
	int shownBet = 0;
	//Bet that you need to stay in the hand
	int topBet = 0;
	//Make sure you account for if there

	std::vector <Card> communityCards;

	struct Player {
		//Just in case i'm feeling goofy :)
		std::string playerName;
		//If the person hasn't folded yet
		bool ifPlayingRound;
		//If the person is all in
		bool ifAllIn = false;
		//if the person has already checked
		bool ifChecked = false;
		//In case there is an all-in situation
		int totalBet = 0;
		//For each round betting
		int currentBet = 0;
		//For visual purposes
		int potentialBet = 0;
		//Which blind the player currently is
		int blindFlag = REGULAR;
		//Money you have currently (when you bet you lose it)
		int money = 500;
		//Value shows the type of hand using "hand hierarchy"
		int handValue = -1;
		//shows the name of the hand
		std::string handName = "";
		//Shows the ranking of the hand, using max five cards their ranks
		int handRanking[5] = { 0, 0, 0, 0, 0 };
		//Number from 1 through _numPlayers showing which order of winning you are
		int ranking = 0;
		//current hand
		std::vector <Card> hand;
		//best hand
		std::vector <Card> bestHand;
	};

	std::vector <Player> players;

	//Special texture IDs
	int tableID;
	int seatID;

	int smallBlindID;
	int bigBlindID;
	int dealerID;

	int yesID;
	int selectYesID;
	int highlightYesID;

	int noID;
	int selectNoID;
	int highlightNoID;

	int plusID;
	int selectPlusID;
	int highlightPlusID;

	int checkID;
	int selectCheckID;
	int highlightCheckID;

	int betID;
	int selectBetID;
	int highlightBetID;

	int raiseID;
	int selectRaiseID;
	int highlightRaiseID;

	int foldID;
	int selectFoldID;
	int highlightFoldID;

	int callID;
	int selectCallID;
	int highlightCallID;

	int allinID;
	int selectAllinID;
	int highlightAllinID;

	int minusID;
	int selectMinusID;
	int highlightMinusID;
};

