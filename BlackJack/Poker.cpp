#include <random>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "Poker.h"

Poker::Poker()
{
	_numPlayers = 0;
}

Poker::~Poker()
{
}

void Poker::init(int numPlayers, Deck deck) {

	_deck = deck;
	_numPlayers = numPlayers;
	//initialize the poker screen
	pokerScreen.init();

	pokerScreen.highlightColor = { 255, 255, 0 };

	for (int i = 0; i < _deck.getSize(); i++) {
		int id = pokerScreen.addTexture(_deck.lookAtCard(i).getImageFileName());

		if (id >= 0) {
			_deck.setCardID(i, id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for cards! SDL Error: " << SDL_GetError() << "\n";

	}

	{
		int id = pokerScreen.addTexture(_deck.getBackImageFileName());

		if (id >= 0) {
			_deck.setBackID(id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for back! SDL Error: " << SDL_GetError() << "\n";

	}

	std::stringstream ss;
	ss << "MainMenu/GamblingTable.png";

	{
		int id = pokerScreen.addTexture(ss.str());

		if (id >= 0) {
			tableID = (id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for background! SDL Error: " << SDL_GetError() << "\n";

	}
	ss.str("");
	ss << "GameButtons/seat.png";

	{
		int id = pokerScreen.addTexture(ss.str());

		if (id >= 0) {
			seatID = (id);
		}
		else
			std::cout << "Failed to initialize SDL Textures for seats! SDL Error: " << SDL_GetError() << "\n";

	}
	ss.str("");

	{
		int id = pokerScreen.addTexture("GameButtons/smallBlind.png");
		int id2 = pokerScreen.addTexture("GameButtons/bigBlind.png");
		int id3 = pokerScreen.addTexture("GameButtons/dealer.png");

		if (id >= 0 && id2 >= 0 && id3 >= 0) {
			smallBlindID = (id);
			bigBlindID = id2;
			dealerID = id3;
		}
		else
			std::cout << "Failed to initialize SDL Textures for blinds! SDL Error: " << SDL_GetError() << "\n";

	}

	//Buttons for check, bet, raise, and fold
	ss.str("");
	for (int i = 0; i < 30; i++) {
		ss << "GameButtons/poker";

		if (i < 3) {
			ss << "Check";
		}

		else if (i < 6) {
			ss << "Bet";
		}

		else if (i < 9) {
			ss << "Raise";
		}

		else if (i < 12) {
			ss << "Fold";
		}
		else if (i < 15) {
			ss << "Plus";
		}
		else if (i < 18) {
			ss << "Call";
		}
		else if (i < 21) {
			ss << "Allin";
		}
		else if (i < 24) {
			ss << "Minus";
		}
		else if (i < 27) {
			ss << "Yes";
		}
		else if (i < 30) {
			ss << "No";
		}

		switch (i % 3) {
		case 1:
			ss << "Selected";
			break;
		case 2:
			ss << "Highlighted";
			break;
		}

		ss << ".png";

		int id = pokerScreen.addTexture(ss.str());
		if (id >= 0) {
			switch (i) {
			case 0:
				checkID = id;
				break;
			case 1:
				selectCheckID = id;
				break;
			case 2:
				highlightCheckID = id;
				break;
			case 3:
				betID = id;
				break;
			case 4:
				selectBetID = id;
				break;
			case 5:
				highlightBetID = id;
				break;
			case 6:
				raiseID = id;
				break;
			case 7:
				selectRaiseID = id;
				break;
			case 8:
				highlightRaiseID = id;
				break;
			case 9:
				foldID = id;
				break;
			case 10:
				selectFoldID = id;
				break;
			case 11:
				highlightFoldID = id;
				break;
			case 12:
				plusID = id;
				break;
			case 13:
				selectPlusID = id;
				break;
			case 14:
				highlightPlusID = id;
				break;
			case 15:
				callID = id;
				break;
			case 16:
				selectCallID = id;
				break;
			case 17:
				highlightCallID = id;
				break;
			case 18:
				allinID = id;
				break;
			case 19:
				selectAllinID = id;
				break;
			case 20:
				highlightAllinID = id;
				break;
			case 21:
				minusID = id;
				break;
			case 22:
				selectMinusID = id;
				break;
			case 23:
				highlightMinusID = id;
				break;
			case 24:
				yesID = id;
				break;
			case 25:
				selectYesID = id;
				break;
			case 26:
				highlightYesID = id;
				break;
			case 27:
				noID = id;
				break;
			case 28:
				selectNoID = id;
				break;
			case 29:
				highlightNoID = id;
				break;
			default:
				std::cout << "ERROR";
				break;
			}
		}
		else
			std::cout << "Failed to initalize SDL textures for the buttons! SDL Error: " << SDL_GetError() << "\n";
		ss.str("");
	}

	//Then initialize all of the textures

	//Initialize the actual game
	pot = 0;

	static std::default_random_engine randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> roll(0, numPlayers - 1);
	int flag = roll(randomEngine);

	for (int i = 0; i < numPlayers; i++) {
		Player player;
		players.push_back(player);
	}

	players[flag].blindFlag = DEALER;
	players[(flag + 1) % numPlayers].blindFlag = SMALLBLIND;
	if (numPlayers == 2)
		players[(flag + 2) % numPlayers].blindFlag = DEALERANDBIGBLIND;
	else
		players[(flag + 2) % numPlayers].blindFlag = BIGBLIND;

	//dealCards();

}

void Poker::restartHand() {
	for (unsigned i = 0; i < communityCards.size(); i++)
		_deck.putCardOnBottom(communityCards[i]);
	communityCards.clear();

	topBet = 0;
	shownBet = 0;
	pot = 0;
	//round = HOLE;
	int flag;
	for (unsigned i = 0; i < players.size(); i++) {
		//Clear everything except for money
		for (unsigned j = 0; j < players[i].hand.size(); j++)
			_deck.putCardOnBottom(players[i].hand[j]);
		players[i].hand.clear();
		players[i].bestHand.clear();
		if (players[i].blindFlag == DEALER) {
			if (roundNumber != 1)
				flag = (i + 1) % players.size();
			else
				flag = i;
		}
		players[i].blindFlag = REGULAR;
		players[i].currentBet = 0;
		players[i].handName = "";
		std::fill(players[i].handRanking, players[i].handRanking + 5, -1);
		players[i].handValue = -1;
		players[i].ranking = 0;
		players[i].ifPlayingRound = true;
		players[i].ifAllIn = false;
	}
	
	players[flag].blindFlag = DEALER;
	players[(flag + 1) % players.size()].blindFlag = SMALLBLIND;
	if (players.size() == 2)
		players[(flag + 2) % players.size()].blindFlag = DEALERANDBIGBLIND;
	else
		players[(flag + 2) % players.size()].blindFlag = BIGBLIND;

	_deck.shuffle();
}

void Poker::dealCards() {
	for (int i = 0; i < 2; i++) {
		for (unsigned i = 0; i < players.size(); i++) {
			players[i].hand.push_back(_deck.takeCardFromTop());
			if (i == playerID)
				players[i].hand.back().reveal();
		}
	}
}

void Poker::removePlayer(int index) {
	if (index != 0)
		players.erase(players.begin() + index);
	else {
		//handle the actual "you lose" screen and stuff
	}
}

bool Poker::playGame(int givenBlind) {
	bool ifPlaying = true;
	round = HOLE;

	blind = givenBlind;

	//Play the setup round
	playRound();
	bool ifContinue = true;

	while (ifPlaying) {
		//The index that the iteration ends at
		//If a new person bets, then it has to go all the way around before ending the round
		int endPlayerIndex = 0;
		//This should actually start with the person to the left of the dealer
		int i = 0;

		//randomizer for time
		static std::default_random_engine randomEngine((unsigned int)time(NULL));
		std::uniform_int_distribution<int> roll(0, 1000);

		for (unsigned index = 0; index < players.size(); index++) {
			if (players[index].blindFlag == SMALLBLIND)
				i = endPlayerIndex = index;
		}
		
		//If you can check or not, basically
		
		//So the betting can start with the guy right after the BB
		bool ifAfterBlind = false;

		bool ifBet = true;
		//While the round continues
		bool ifRoundContinues = true;
		while (ifRoundContinues) {
			
			//i is the player playing now
			bool ifTurnDone = false;
			if (players[i].ifPlayingRound) {
				while (!ifTurnDone) {

					if (ifAfterBlind) {
						ifAfterBlind = false;
						endPlayerIndex = i;
					}
				
					std::string response = "";
					//Force blinds to pay
					if (round == HOLE && (players[i].blindFlag == BIGBLIND || players[i].blindFlag == DEALERANDBIGBLIND) && ifBet) {
						topBet = blind;
						players[i].currentBet = blind;
						if (blind > players[i].money)
							removePlayer(i);
						else
							players[i].money -= players[i].currentBet;

						ifAfterBlind = true;
						response = "";
						ifBet = false;
						ifTurnDone = true;

					}
					else if (round == HOLE && players[i].blindFlag == SMALLBLIND && ifBet) {
						response = "";
						players[i].currentBet = blind / 2;
						if (blind / 2 > players[i].money)
							removePlayer(i);
						else
							players[i].money -= players[i].currentBet;
						ifTurnDone = true;
					}
					else if (i == 0 && !players[i].ifAllIn) {
						//Add a new thing for choosing whether to play again or naht.
						if (round == REVEAL)
							response = userInput("continue");
						else {
							if (ifBet)
								response = userInput("bet");
							else
								response = userInput("call");
						}
					}

					else if (!players[i].ifAllIn) {
						if (round != REVEAL) {
							if (ifBet)
								response = computerInput("bet", i);
							else
								response = computerInput("call", i);
						}
					}
					else if (players[i].ifAllIn && i == 0 && round == REVEAL)
						response = userInput("continue");

					if (response == "plus") {
						if (players[i].potentialBet != players[i].money)
							players[i].potentialBet += 10;
					}
					else if (response == "minus") {
						if (players[i].potentialBet != 0)
							players[i].potentialBet -= 10;
					}
					else if (response == "check") {
						players[i].ifChecked = true;
						ifTurnDone = true;
						//playRound();
					}
					else if (response == "call") {
						if (players[i].money > topBet) {
							players[i].money -= (topBet - players[i].currentBet);
							players[i].currentBet = topBet;
						}

						else {
							players[i].currentBet = players[i].money + players[i].currentBet;
							players[i].ifAllIn = true;
						}

						ifTurnDone = true;
					}
					else if (response == "raise") {
						if (topBet + players[i].potentialBet < players[i].money) {
							players[i].money -= (topBet + players[i].potentialBet - players[i].currentBet);
							players[i].currentBet = topBet + players[i].potentialBet;
							topBet = players[i].currentBet;
							endPlayerIndex = i;
							ifBet = false;
							for (unsigned i = 0; i < players.size(); i++)
								players[i].ifChecked = false;
						}
						else {
							players[i].currentBet = players[i].money;
							players[i].ifAllIn = true;
						}
						ifTurnDone = true;
						players[i].potentialBet = 0;
					}
					else if (response == "allin") {
						players[i].currentBet = players[i].money + players[i].currentBet;
						if (players[i].currentBet > topBet) {
							topBet = players[i].currentBet;
							endPlayerIndex = i;
							ifBet = false;
							for (unsigned i = 0; i < players.size(); i++)
								players[i].ifChecked = false;
						}
						players[i].ifAllIn = true;
						ifTurnDone = true;
					}
					else if (response == "fold") {
						//Lose your hand and stop playing
						players[i].ifPlayingRound = false;
						ifTurnDone = true;
					}
					else if (response == "bet") {
						//Secure the bet
						players[i].currentBet = players[i].potentialBet;
						players[i].potentialBet = 0;
						players[i].money -= players[i].currentBet;
						topBet = players[i].currentBet;
						endPlayerIndex = i;
						for (unsigned i = 0; i < players.size(); i++)
							players[i].ifChecked = false;
						ifBet = false;
						ifTurnDone = true;
					}
					else if (response == "yes") {
						ifTurnDone = true;
					}
					else if (response == "no") {
						ifContinue = false;
						ifTurnDone = true;
					}
					else
						ifTurnDone = true;

					if (i == 0)
						shownBet = players[i].potentialBet;

					if (players[i].ifAllIn && round != REVEAL)
						players[i].money = 0;

					pokerScreen.resetRenderer();
					drawGame();
					if (ifTurnDone)
						pokerScreen.update();

				}
			}
			else
				ifTurnDone = true;

				if (round != REVEAL && !players[i].ifAllIn)
					pokerScreen.wait(roll(randomEngine));
				else
					pokerScreen.wait(100);

			int nextIndex = (i + 1) % players.size();
			if (nextIndex == endPlayerIndex)
				ifRoundContinues = false;
			else
				i = nextIndex;
		}

		

		for (unsigned i = 0; i < players.size(); i++) {
			pot += players[i].currentBet;
			players[i].totalBet += players[i].currentBet;
			players[i].currentBet = 0;
			players[i].ifChecked = false;
		}

		if (round == REVEAL)
			ifPlaying = false;

		round = (round + 1) % 5;

		playRound();

		
	}
	//system("PAUSE");
	roundNumber += 1;
	return ifContinue;
}

bool Poker::playRound() {

	bool ifContinue = true;

	if (round == HOLE) {
		restartHand();
		for (unsigned i = 0; i < players.size(); i++) {
			if (players[i].money <= 0)
				removePlayer(i);
		}
		dealCards();
		//First round of betting
		//round = FLOP;
	}

	else if (round == FLOP) {
		communityCards.push_back(_deck.takeCardFromTop());
		communityCards.back().reveal();
		communityCards.push_back(_deck.takeCardFromTop());
		communityCards.back().reveal();
		communityCards.push_back(_deck.takeCardFromTop());
		communityCards.back().reveal();
		updateValues();
		updateRankings();
		//Second round of betting
		//round = TURN;
	}

	else if (round == TURN) {
		communityCards.push_back(_deck.takeCardFromTop());
		communityCards.back().reveal();
		updateValues();
		updateRankings();
		//Third round of betting
		//round = RIVER;
	}

	else if (round == RIVER) {
		communityCards.push_back(_deck.takeCardFromTop());
		communityCards.back().reveal();
		updateValues();
		updateRankings();
		//Last round of betting
		//round = REVEAL;
	}

	else if (round == REVEAL) {
		//Show winning hand(s) and divie up the pot
		for (unsigned i = 0; i < players.size(); i++) {
			if (players[i].ranking == 1) {
				players[i].money += pot;
				for (unsigned j = 0; j < players[i].hand.size(); j++)
					players[i].hand[j].reveal();
			}
		}
		//removePlayer(1);
		//round = HOLE;
		//Say "press any button to continue";
	}

	return ifContinue;
}

std::string Poker::userInput(std::string move) {

	bool ifPromptContinue = false;
	if (move == "continue")
		ifPromptContinue = true;
	bool ifFirst = true;
	if (move == "bet")
		ifFirst = true;
	if (move == "call")
		ifFirst = false;
	
	bool quit = false;

	bool ifMouseIsPressedDown = false;

	std::string response = "";

	//Event handler
	SDL_Event e;
	enum events { NONE, CHECKSELECT, CHECKHIGHLIGHT, BETSELECT, BETHIGHLIGHT, RAISESELECT, RAISEHIGHLIGHT, FOLDSELECT, 
				FOLDHIGHLIGHT, PLUSSELECT, PLUSHIGHLIGHT, CALLSELECT, CALLHIGHLIGHT, ALLINSELECT, ALLINHIGHLIGHT,
				MINUSSELECT, MINUSHIGHLIGHT, YESSELECT, YESHIGHLIGHT, NOSELECT, NOHIGHLIGHT };
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
			if (e.type == SDL_MOUSEBUTTONUP) {

				if (ifPromptContinue) {
					if (pokerScreen.ifInsideTexture(x, y, yesID)) {
						response = "yes";
						quit = true;
					}
					if (pokerScreen.ifInsideTexture(x, y, noID)) {
						response = "no";
						quit = true;
					}
				}
				else {

					if (pokerScreen.ifInsideTexture(x, y, allinID)) {
						response = "allin";
						quit = true;
					}
					if (pokerScreen.ifInsideTexture(x, y, foldID)) {
						response = "fold";
						quit = true;
					}
					if (pokerScreen.ifInsideTexture(x, y, plusID)) {
						response = "plus";
						quit = true;
					}
					if (pokerScreen.ifInsideTexture(x, y, minusID)) {
						response = "minus";
						quit = true;
					}

					if (ifFirst) {
						if (pokerScreen.ifInsideTexture(x, y, checkID)) {
							response = "check";
							quit = true;
						}
						if (pokerScreen.ifInsideTexture(x, y, betID)) {
							response = "bet";
							quit = true;
						}
					}
					else {
						if (pokerScreen.ifInsideTexture(x, y, raiseID)) {
							response = "raise";
							quit = true;
						}
						if (pokerScreen.ifInsideTexture(x, y, callID)) {
							response = "call";
							quit = true;
						}
					}
				}
			}
		}

		if (ifPromptContinue) {
			if (pokerScreen.ifInsideTexture(x, y, yesID)) {
				if (ifMouseIsPressedDown)
					currentEvent = YESHIGHLIGHT;
				else
					currentEvent = YESSELECT;
			}
			else if (pokerScreen.ifInsideTexture(x, y, noID)) {
				if (ifMouseIsPressedDown)
					currentEvent = NOHIGHLIGHT;
				else
					currentEvent = NOSELECT;
			}
			else
				currentEvent = NONE;
		}
		else {
			if (pokerScreen.ifInsideTexture(x, y, checkID) && ifFirst) {
				if (ifMouseIsPressedDown)
					currentEvent = CHECKHIGHLIGHT;
				else
					currentEvent = CHECKSELECT;
			}

			else if (pokerScreen.ifInsideTexture(x, y, betID) && ifFirst) {
				if (ifMouseIsPressedDown)
					currentEvent = BETHIGHLIGHT;
				else
					currentEvent = BETSELECT;
			}

			else if (pokerScreen.ifInsideTexture(x, y, raiseID) && !ifFirst) {
				if (ifMouseIsPressedDown)
					currentEvent = RAISEHIGHLIGHT;
				else
					currentEvent = RAISESELECT;
			}

			else if (pokerScreen.ifInsideTexture(x, y, foldID)) {
				if (ifMouseIsPressedDown)
					currentEvent = FOLDHIGHLIGHT;
				else
					currentEvent = FOLDSELECT;
			}

			else if (pokerScreen.ifInsideTexture(x, y, plusID)) {
				if (ifMouseIsPressedDown)
					currentEvent = PLUSHIGHLIGHT;
				else
					currentEvent = PLUSSELECT;
			}

			else if (pokerScreen.ifInsideTexture(x, y, minusID)) {
				if (ifMouseIsPressedDown)
					currentEvent = MINUSHIGHLIGHT;
				else
					currentEvent = MINUSSELECT;
			}

			else if (pokerScreen.ifInsideTexture(x, y, callID) && !ifFirst) {
				if (ifMouseIsPressedDown)
					currentEvent = CALLHIGHLIGHT;
				else
					currentEvent = CALLSELECT;
			}

			else if (pokerScreen.ifInsideTexture(x, y, allinID)) {
				if (ifMouseIsPressedDown)
					currentEvent = ALLINHIGHLIGHT;
				else
					currentEvent = ALLINSELECT;
			}

			else
				currentEvent = NONE;
		}
		//pokerScreen.drawTexture(95, 70, 10, 10, hitID, UPLEFT);
		//pokerScreen.drawTexture(95, 90, 10, 10, stayID, UPLEFT);

		//resets renderer
		pokerScreen.resetRenderer();

		//draws background and everything
		drawGame();

		if (ifPromptContinue) {
			if (currentEvent == YESSELECT) pokerScreen.drawTexture(25, 90, 10, 10, selectYesID, UPLEFT, 0, false);
			else if (currentEvent == YESHIGHLIGHT) pokerScreen.drawTexture(25, 90, 10, 10, highlightYesID, UPLEFT, 0, false);
			else pokerScreen.drawTexture(25, 90, 10, 10, yesID, UPLEFT, 0, false);

			if (currentEvent == NOSELECT) pokerScreen.drawTexture(75, 90, 10, 10, selectNoID, UPLEFT, 0, false);
			else if (currentEvent == NOHIGHLIGHT) pokerScreen.drawTexture(75, 90, 10, 10, highlightNoID, UPLEFT, 0, false);
			else pokerScreen.drawTexture(75, 90, 10, 10, noID, UPLEFT, 0, false);
		}
		else {
			if (currentEvent == FOLDSELECT) pokerScreen.drawTexture(100, 90, 10, 10, selectFoldID, UPLEFT, 0, false);
			else if (currentEvent == FOLDHIGHLIGHT) pokerScreen.drawTexture(100, 90, 10, 10, highlightFoldID, UPLEFT, 0, false);
			else pokerScreen.drawTexture(100, 90, 10, 10, foldID, UPLEFT, 0, false);

			if (currentEvent == ALLINSELECT) pokerScreen.drawTexture(0, 90, 10, 10, selectAllinID, UPLEFT, 0, false);
			else if (currentEvent == ALLINHIGHLIGHT) pokerScreen.drawTexture(0, 90, 10, 10, highlightAllinID, UPLEFT, 0, false);
			else pokerScreen.drawTexture(0, 90, 10, 10, allinID, UPLEFT, 0, false);

			if (currentEvent == PLUSSELECT) pokerScreen.drawTexture(17.5, 75, 10, 10, selectPlusID, UPLEFT, 0, false);
			else if (currentEvent == PLUSHIGHLIGHT) pokerScreen.drawTexture(17.5, 75, 10, 10, highlightPlusID, UPLEFT, 0, false);
			else pokerScreen.drawTexture(17.5, 75, 10, 10, plusID, UPLEFT, 0, false);

			if (currentEvent == MINUSSELECT) pokerScreen.drawTexture(36.5, 75, 10, 10, selectMinusID, UPLEFT, 0, false);
			else if (currentEvent == MINUSHIGHLIGHT) pokerScreen.drawTexture(36.5, 75, 10, 10, highlightMinusID, UPLEFT, 0, false);
			else pokerScreen.drawTexture(36.5, 75, 10, 10, minusID, UPLEFT, 0, false);

			if (ifFirst) {
				if (currentEvent == CHECKSELECT) pokerScreen.drawTexture(75, 90, 10, 10, selectCheckID, UPLEFT, 0, false);
				else if (currentEvent == CHECKHIGHLIGHT) pokerScreen.drawTexture(75, 90, 10, 10, highlightCheckID, UPLEFT, 0, false);
				else pokerScreen.drawTexture(75, 90, 10, 10, checkID, UPLEFT, 0, false);

				if (currentEvent == BETSELECT) pokerScreen.drawTexture(25, 90, 10, 10, selectBetID, UPLEFT, 0, false);
				else if (currentEvent == BETHIGHLIGHT) pokerScreen.drawTexture(25, 90, 10, 10, highlightBetID, UPLEFT, 0, false);
				else pokerScreen.drawTexture(25, 90, 10, 10, betID, UPLEFT, 0, false);
			}

			else {
				if (currentEvent == RAISESELECT) pokerScreen.drawTexture(25, 90, 10, 10, selectRaiseID, UPLEFT, 0, false);
				else if (currentEvent == RAISEHIGHLIGHT) pokerScreen.drawTexture(25, 90, 10, 10, highlightRaiseID, UPLEFT, 0, false);
				else pokerScreen.drawTexture(25, 90, 10, 10, raiseID, UPLEFT, 0, false);

				if (currentEvent == CALLSELECT) pokerScreen.drawTexture(75, 90, 10, 10, selectCallID, UPLEFT, 0, false);
				else if (currentEvent == CALLHIGHLIGHT) pokerScreen.drawTexture(75, 90, 10, 10, highlightCallID, UPLEFT, 0, false);
				else pokerScreen.drawTexture(75, 90, 10, 10, callID, UPLEFT, 0, false);
			}
			std::stringstream ss;
			SDL_Color color = { 255, 255, 255 };
			ss << shownBet;
			pokerScreen.drawText(ss.str(), color, 0, 25, 76.5, 10, 10, UPLEFT, 0, false);
			ss.str("");
		}
		/*
		if (winner != -1) {
			SDL_Color color = { 255, 255, 255 };
			if (winner == PLAYER)
				pokerScreen.drawText("Player won!", color, 0, 50, 50, 15, 15, CENTER);
			else if (winner == DEALER)
				pokerScreen.drawText("Dealer won!", color, 0, 50, 50, 15, 15, CENTER);
		}
		*/

		pokerScreen.update();
	}

	//using cin to gather data for turn
	//OR you could totally use SDL
	return response;
}

std::string Poker::computerInput(std::string move, int player) {
	//players[player] is who this is working for
	std::string response;
	//This will determine the confidence in the hand
	int pokerValue = 0;
	//if round == FLOP, then this means that you CANNOT use updateValues values
	if (round == FLOP) {
		int rank1 = players[player].hand[0].getRank();
		int rank2 = players[player].hand[1].getRank();

		if (abs(rank1 - rank2) < 5 && rank1 != rank2) {
			//10 though 25
			pokerValue += 5 * (6 - abs(rank1 - rank2));
		}

		if (rank1 == 1) rank1 = 14; if (rank2 == 1) rank2 = 14;

		if (abs(rank1 - rank2) < 5 && rank1 != rank2) {
			//10 though 25
			pokerValue += 5 * (6 - abs(rank1 - rank2));
		}
		//max is 30
		if (players[player].hand[0].getSuit() == players[player].hand[0].getSuit())
			pokerValue += 30;

		//Max is 75
		if (rank1 == rank2)
			pokerValue += 75;

		//Max is 28
		pokerValue += 2 * rank2;
		pokerValue += 2 * rank1;

		//pokerValue max is 183
		//pokerValue min is 9

	}
	else {

	}
	if (move == "bet") {
		//Either "allin", "bet", "check", or "fold";
		//Probably will never fold at this point though
		if (round == REVEAL)
			response = "";
		else
			response = "check";
	}

	else if (move == "call") {
		//Either "allin", "raise", "call", or "fold";
		response = "call";
	}

	return response;
}

//Returns the ID of the player who won the hand in the end
int Poker::whoWon() {
	return 0;
}

void Poker::updateValues() {
	for (unsigned i = 0; i < players.size(); i++) {
		if (players[i].ifPlayingRound) {

			players[i].bestHand.clear();
			std::vector <Card> bestHand;
			std::vector <Card> wholeHand = communityCards;
			wholeHand.push_back(players[i].hand[0]);
			wholeHand.push_back(players[i].hand[1]);
			organizeHand(wholeHand);

			/*
			//Get the highest card
			int max = 0;
			for (unsigned j = 0; j < wholeHand.size(); j++) {
				int rank = wholeHand[j].getRank();
				if (rank == 1)
					rank = 14;
				//remember that this is only not the case in a low straight!!!
				if(max < rank)
					max = rank;
			}
			players[i].handValue[HIGHCARD] = max;
			*/

			//Figure out the hand
			//enum handHierarchy { STRAIGHTFLUSH, FOUROFAKIND, FULLHOUSE, FLUSH, STRAIGHT, THREEOFAKIND, TWOPAIR, PAIR, HIGHCARD };
			int potentialHands[9] = { 0 };
			//Holds the index of the cards in the wholeHand for what could be the final hand
			int potentialHandIndex[5] = { -1 };
			//If there is a potential for a hand

			//Get the highest card
			int max = 0;
			for (unsigned j = 0; j < wholeHand.size(); j++) {
				int rank = wholeHand[j].getRank();
				if (rank == 1)
					rank = 14;
				//remember that this is only not the case in a low straight!!!
				if (max < rank)
					max = rank;
			}
			potentialHands[HIGHCARD] = max;
			max = 0;

			//This will account for ace as a one and as a 14
			int numCards[14] = { 0 };
			int numSuits[4] = { 0 };
			enum suits { SPADES, HEARTS, DIAMONDS, CLUBS };

			for (unsigned j = 0; j < wholeHand.size(); j++) {
				if (wholeHand[j].getRank() != 1)
					numCards[wholeHand[j].getRank() - 1] += 1;
				else {
					numCards[0] += 1;
					numCards[13] += 1;
				}

				if (wholeHand[j].getSuit() == "spades")
					numSuits[SPADES] += 1;
				if (wholeHand[j].getSuit() == "hearts")
					numSuits[HEARTS] += 1;
				if (wholeHand[j].getSuit() == "diamonds")
					numSuits[DIAMONDS] += 1;
				if (wholeHand[j].getSuit() == "clubs")
					numSuits[CLUBS] += 1;
			}

			//Check for a flush (specifically a 
			for (int j = 0; j < 4; j++) {
				if (numSuits[j] >= 5) {
					potentialHands[FLUSH] = j;
					//there is a flush 
				}
			}
			//Check for a straight
			//Only 10 straights there can be...
			// A-5, 2-6, 3-7, 4-8, 5-9, 6-10, 7-J, 8-Q, 9-K, 10-A
			for (int j = 0; j < 10; j++) {
				if (numCards[j] > 0 && numCards[j + 1] > 0 && numCards[j + 2] > 0 && numCards[j + 3] > 0 && numCards[j + 4] > 0) {
					if (j + 1 > potentialHands[STRAIGHT])
						potentialHands[STRAIGHT] = j + 1;
					//Then there is a straight
				}
			}
			//Check for repeated ranks
			for (int j = 0; j < 13; j++) {
				int rank = j;
				if (rank == 0)
					rank = 13;

				if (numCards[rank] == 4) {
					potentialHands[FOUROFAKIND] = rank + 1;
					//Four of a kind
				}

				else if (numCards[rank] == 3) {
					if (potentialHands[THREEOFAKIND] < rank + 1)
						potentialHands[THREEOFAKIND] = rank + 1;
					//Three of a kind
				}

				//Checks for pairs and two pairs
				//PAIR and TWOPAIR always have the two highest pairs such that PAIR > TWOPAIR
				else if (numCards[rank] == 2) {
					if (potentialHands[PAIR] != 0) {
						if (potentialHands[TWOPAIR] != 0) {
							if (rank + 1 > potentialHands[PAIR]) {
								potentialHands[TWOPAIR] = potentialHands[PAIR];
								potentialHands[PAIR] = rank + 1;
							}
							else if (rank + 1 > potentialHands[TWOPAIR]) {
								potentialHands[TWOPAIR] = rank + 1;
							}
							else {
								//nothing
							}
						}
						else {
							if (potentialHands[PAIR] > rank + 1)
								potentialHands[TWOPAIR] = rank + 1;
							else {
								potentialHands[TWOPAIR] = potentialHands[PAIR];
								potentialHands[PAIR] = rank + 1;
							}
						}
					}
					else
						potentialHands[PAIR] = rank + 1;
					//Pair
				}
			}

			if (potentialHands[PAIR] != 0 && potentialHands[THREEOFAKIND] != 0) {
				potentialHands[FULLHOUSE] = 1;
				//Full house
			}

			if (potentialHands[FLUSH] != 0 && potentialHands[STRAIGHT] != 0) {
				potentialHands[STRAIGHTFLUSH] = true;
				//This almost definitely is not true most of the time by the way
				//But it shows that it is a possibility
			}

			//Check for each combination individually, going downwards
			//enum handHierarchy { STRAIGHTFLUSH, FOUROFAKIND, FULLHOUSE, FLUSH, STRAIGHT, THREEOFAKIND, TWOPAIR, PAIR, HIGHCARD };
			int numFillCards = 0;
			//number of cards needed to complete the hand (along with the hand of course)

			players[i].handValue = -1;
			//Straight flush
			int handChecker = 0;
			//I think there's a problem in this code
			if (potentialHands[STRAIGHTFLUSH] != 0) {
				for (int cardNumber = 0; cardNumber < 5; cardNumber++) {
					for (unsigned j = 0; j < wholeHand.size(); j++) {
						int rank = wholeHand[j].getRank();
						if (potentialHands[STRAIGHT] > 1 && rank == 1)
							rank = 14;

						int suit = -1;
						if (wholeHand[j].getSuit() == "spades")
							suit = SPADES;
						if (wholeHand[j].getSuit() == "hearts")
							suit = HEARTS;
						if (wholeHand[j].getSuit() == "diamonds")
							suit = DIAMONDS;
						if (wholeHand[j].getSuit() == "clubs")
							suit = CLUBS;

						if (potentialHands[STRAIGHT] + cardNumber == rank && potentialHands[FLUSH] == suit) {
							potentialHandIndex[4 - cardNumber] = j;
							handChecker += 1;
						}
					}
				}
			}

			if (handChecker == 5) {
				for (int j = 0; j < 5; j++) {
					int rank = wholeHand[potentialHandIndex[j]].getRank();
					if (potentialHands[STRAIGHT] > 1 && rank == 1)
						rank = 14;
					players[i].handRanking[j] = rank;
					bestHand.push_back(wholeHand[potentialHandIndex[j]]);
				}
				players[i].handValue = STRAIGHTFLUSH;
				//This completes the hand just fine
				numFillCards = 0;
				//STRAIGHT FLUSH HAS OCCURED, HOLY SHIET
				//construct the hand
			}

			handChecker = 0;

			//Don't forget to clear the potentialHandIndex array
			std::fill(potentialHandIndex, potentialHandIndex + 5, -1);

			if (players[i].handValue == -1) {
				if (potentialHands[FOUROFAKIND] != 0) {
					players[i].handValue = FOUROFAKIND;
					numFillCards = 1;
					for (int j = 0; j < 4; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							if (rank == potentialHands[FOUROFAKIND]) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
				}
			}
			//Four of a kind
			//This is already figured out for the most part, so you just need to construct the hand with the highest other card

			if (players[i].handValue == -1) {
				if (potentialHands[FULLHOUSE] != 0) {
					players[i].handValue = FULLHOUSE;
					numFillCards = 0;
					for (int j = 0; j < 3; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							if (rank == potentialHands[THREEOFAKIND]) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
					for (int j = 0; j < 2; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							if (rank == potentialHands[PAIR]) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
				}
			}

			//Full house
			//This is already figured out, just construct with PAIR and THREEOFAKIND, they are already supposed to be ordered
			//a full house is ranked first by its triplet, then its pair

			if (players[i].handValue == -1) {
				if (potentialHands[FLUSH] != 0) {
					players[i].handValue = FLUSH;
					numFillCards = 0;
					for (int j = 0; j < 5; j++) {
						max = 0;
						int maxID = 0;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							int suit = -1;
							if (wholeHand[k].getSuit() == "spades")
								suit = SPADES;
							else if (wholeHand[k].getSuit() == "hearts")
								suit = HEARTS;
							else if (wholeHand[k].getSuit() == "diamonds")
								suit = DIAMONDS;
							else if (wholeHand[k].getSuit() == "clubs")
								suit = CLUBS;

							if (max < rank && suit == potentialHands[FLUSH]) {
								max = rank;
								maxID = k;
							}
						}

						bestHand.push_back(wholeHand[maxID]);
						wholeHand.erase(wholeHand.begin() + maxID);
					}
				}
			}

			//Flush
			//No checking really required here, just construct in order of greatness

			if (players[i].handValue == -1) {
				if (potentialHands[STRAIGHT] != 0) {
					players[i].handValue = STRAIGHT;
					numFillCards = 0;
					for (int j = 0; j < 5; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1 && potentialHands[STRAIGHT] != 1) rank = 14;
							if (rank == potentialHands[STRAIGHT] + 4 - j) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
				}
			}

			//Straight
			//No checking required, construct in order of magnitude

			if (players[i].handValue == -1) {
				if (potentialHands[THREEOFAKIND] != 0) {
					players[i].handValue = THREEOFAKIND;
					numFillCards = 2;
					for (int j = 0; j < 3; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							if (rank == potentialHands[THREEOFAKIND]) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
				}
			}

			//Three of a kind
			//no checking required, construct with 2 other highest cards

			if (players[i].handValue == -1) {
				if (potentialHands[TWOPAIR] != 0) {
					players[i].handValue = TWOPAIR;
					numFillCards = 1;
					for (int j = 0; j < 2; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							if (rank == potentialHands[PAIR]) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
					for (int j = 0; j < 2; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							if (rank == potentialHands[TWOPAIR]) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
				}
			}

			//Two pair
			//ordered already, just construct with other high card

			if (players[i].handValue == -1) {
				if (potentialHands[PAIR] != 0) {
					players[i].handValue = PAIR;
					numFillCards = 3;
					for (int j = 0; j < 2; j++) {
						int correctID;
						for (unsigned k = 0; k < wholeHand.size(); k++) {
							int rank = wholeHand[k].getRank();
							if (rank == 1) rank = 14;
							if (rank == potentialHands[PAIR]) {
								correctID = k;
								k = wholeHand.size();
							}
						}
						bestHand.push_back(wholeHand[correctID]);
						wholeHand.erase(wholeHand.begin() + correctID);
					}
				}
			}

			//pair
			//construct with 3 other high cards

			if (players[i].handValue == -1) {
				if (potentialHands[HIGHCARD] != 0) {
					players[i].handValue = HIGHCARD;
					numFillCards = 5;
				}
			}

			//High card
			//order in magnitude

			for (int j = 0; j < numFillCards; j++) {
				max = 0;
				int maxID = 0;
				for (unsigned k = 0; k < wholeHand.size(); k++) {
					int rank = wholeHand[k].getRank();
					if (rank == 1) rank = 14;
					if (max < rank) {
						max = rank;
						maxID = k;
					}
				}
				bestHand.push_back(wholeHand[maxID]);
				wholeHand.erase(wholeHand.begin() + maxID);
			}

			for (unsigned j = 0; j < bestHand.size(); j++) {
				int rank = bestHand[j].getRank();
				if (rank == 1 && potentialHands[STRAIGHT] != 1) rank = 14;
				players[i].handRanking[j] = rank;
			}

			players[i].bestHand = bestHand;

			std::string name;
			switch (players[i].handValue) {
			case STRAIGHTFLUSH:
				name = "Straight Flush";
				break;
			case FOUROFAKIND:
				name = "Four of a kind";
				break;
			case FULLHOUSE:
				name = "Full House";
				break;
			case FLUSH:
				name = "Flush";
				break;
			case STRAIGHT:
				name = "Straight";
				break;
			case THREEOFAKIND:
				name = "Three of a kind";
				break;
			case TWOPAIR:
				name = "Two Pair";
				break;
			case PAIR:
				name = "Pair";
				break;
			case HIGHCARD:
				name = "High Card";
				break;
			default:
				name = "ERROR";
				std::cout << players[i].handValue;
				break;
			}

			players[i].handName = name;

			wholeHand.clear();
			bestHand.clear();
		}
	}
}

//enum handHierarchy { STRAIGHTFLUSH, FOUROFAKIND, FULLHOUSE, FLUSH, STRAIGHT, THREEOFAKIND, TWOPAIR, PAIR, HIGHCARD };

void Poker::updateRankings() {
	std::vector <int> playerValues;
	//Outside vector is the rank at which these players are
	//Inside vector is the players inside this rank 
	//This is in case there is a tie :)
	std::vector <std::vector<int>> playerRankID;
	for (unsigned i = 0; i < players.size(); i++) {
		if (players[i].ifPlayingRound)
			playerValues.push_back(players[i].handValue);
		else
			playerValues.push_back(9);
	}

	int errorChecker = 0;
	//Sometimes this loops forever?
	//This code isn't working like it's supposed to
	//This is if all the players are checked
	bool ifFinished = false;
	while (!ifFinished) {

		errorChecker += 1;
		if (errorChecker > 100) {
			std::cout << "THERE IS ERROR IN UPDATERANKINGS";
			system("PAUSE");
		}

		int maxRank = 9;
		//This will only exceed the size of 1 if there is an intense tie
		std::vector <int> maxRankIDs;
		maxRankIDs.push_back(-1);
		for (unsigned i = 0; i < playerValues.size(); i++) {
			if (maxRank >= playerValues[i] && playerValues[i] != 9) {
				bool ifReplaceMax = true;
				if (maxRank == playerValues[i]) {
					//compare handRankings, they should be all in same order
					for (int j = 0; j < 5; j++) {
						int newHand = players[i].handRanking[j];
						int oldHand = players[maxRankIDs[0]].handRanking[j];
						if (newHand > oldHand) {
							ifReplaceMax = true;
							j = 5;
						}
						else if (newHand < oldHand) {
							ifReplaceMax = false;
							j = 5;
						}
						else {
							//Loop unless j = 4, then it's actually a tie
							if (j == 4) {
								//ACTUALLY A TIE, handle this
								maxRankIDs.push_back(i);
								ifReplaceMax = false;
							}
						}
					}
				}
				//HERE you check for ties, okay babe?
				if (ifReplaceMax) {
					maxRankIDs.clear();
					maxRankIDs.push_back(-1);
					maxRank = playerValues[i];
					maxRankIDs[0] = i;
				}
			}
		}
		//Ranking goes from 1 to the number of players
		playerRankID.push_back(maxRankIDs);
		for (unsigned j = 0; j < maxRankIDs.size(); j++) {
			players[maxRankIDs[j]].ranking = playerRankID.size();
			playerValues[maxRankIDs[j]] = 9;
		}

		//Check if all the values aren't 9
		ifFinished = true;
		for (unsigned i = 0; i < playerValues.size(); i++) {
			if (playerValues[i] != 9)
				ifFinished = false;
		}
	}
	//First check the handValue

	//If there is a tie then check the handRankings
	//Loop this for each tie there is and if there is a very, very tie then do something about it
}

bool Poker::drawGame() {
	//The x and y used in the for loops
	double x = 0, y = 0;
	//This is a misnomer, this is actually the divisor of the screen dimensions
	double cardWidth = 7, cardHeight = 6;
	//How far the cards are apart from one another
	double cardSeparation = 7;
	//The center of the cards
	double centerX = 0, centerY = 0;
	//The angle of rotation
	double theta = 0.0;
	double radians = 0.0;

	//Draw the table in the background
	pokerScreen.drawBackground(255, 119, 255, tableID);

	//This separation is the same for everybody
	cardSeparation = 115;

	for (unsigned i = 0; i < players.size(); i++) {
		//Draw all the hands

		switch (players.size()) {
		//IF TWO PLAYERS
		case 2:
			if (i == 0) {
				centerX = 0;
				centerY = 270;
				theta = 180;
			}
			else if (i == 1) {
				centerX = 0;
				centerY = -310;
				theta = 0;
			}
			break;
		//IF THREE PLAYERS
		case 3:
			if (i == 0) {
				centerX = 0;
				centerY = 270;
				theta = 180;
			}
			else if (i == 1) {
				theta = -45;
				centerX = -480;
				centerY = -250;
			}
			else if (i == 2) {
				theta = 45;
				centerX = 480;
				centerY = -250;
			}
			break;
		//IF FOUR PLAYERS
		case 4:
			if (i == 0) {
				centerX = 0;
				centerY = 270;
				theta = 180;
			}
			else if (i == 1) {
				theta = -90;
				centerX = -615;
				centerY = 0;
			}
			else if (i == 2) {
				centerX = 0;
				centerY = -310;
				theta = 0;
			}
			else if (i == 3) {
				theta = 90;
				centerX = 615;
				centerY = 0;
			}
			break;
		//IF FIVE PLAYERS
		case 5:
			if (i == 0) {
				centerX = 0;
				centerY = 270;
				theta = 180;
			}
			else if (i == 1) {
				theta = -90;
				centerX = -615;
				centerY = 0;
			}
			else if (i == 2) {
				theta = -10;
				centerX = -250;
				centerY = -300;
			}
			else if (i == 3) {
				theta = 10;
				centerX = 250;
				centerY = -300;
			}
			else if (i == 4) {
				theta = 90;
				centerX = 615;
				centerY = 0;
			}
			break;
		//IF SIX PLAYERS
		case 6:
			if (i == 0) {
				centerX = 0;
				centerY = 270;
				theta = 180;
			}
			else if (i == 1) {
				theta = -90;
				centerX = -615;
				centerY = 0;
			}
			else if (i == 2) {
				theta = -30;
				centerX = -350;
				centerY = -250;
			}
			else if (i == 3) {
				centerX = 0;
				centerY = -310;
				theta = 0;
			}
			else if (i == 4) {
				theta = 30;
				centerX = 350;
				centerY = -250;
			}
			else if (i == 5) {
				theta = 90;
				centerX = 615;
				centerY = 0;
			}
			break;
		//IF SEVEN PLAYERS
		case 7:
			if (i == 0) {
				centerX = 0;
				centerY = 270;
				theta = 180;
			}
			else if (i == 1) {
				theta = -90;
				centerX = -615;
				centerY = 0;
			}
			else if (i == 2) {
				theta = -45;
				centerX = -480;
				centerY = -250;
			}
			else if (i == 3) {
				theta = -10;
				centerX = -150;
				centerY = -300;
			}
			else if (i == 4) {
				theta = 10;
				centerX = 150;
				centerY = -300;
			}
			else if (i == 5) {
				theta = 45;
				centerX = 480;
				centerY = -250;
			}
			else if (i == 6) {
				theta = 90;
				centerX = 615;
				centerY = 0;
			}
			break;
		}

		radians = theta * M_PI / 180;
		x = centerX - ((players[i].hand.size() - 1) * cardSeparation * cos(radians) / 2);
		y = centerY - ((players[i].hand.size() - 1) * cardSeparation * sin(radians) / 2);
		if (players[i].ifPlayingRound) {
			for (unsigned j = 0; j < players[i].hand.size(); j++) {
				if (players[i].hand[j].getIfCovered())
					pokerScreen.drawTexture(x, y, cardWidth, cardHeight, _deck.getBackID(), ROTATABLE, theta, false);
				else
					pokerScreen.drawTexture(x, y, cardWidth, cardHeight, players[i].hand[j].getID(), ROTATABLE, theta, checkIfBest(players[i].hand[j], i));
				x += cardSeparation * cos(radians);
				y += cardSeparation * sin(radians);
			}
		}
		//draw seat
		x = centerX + cardSeparation * sin(radians);
		y = centerY - cardSeparation * cos(radians);
		pokerScreen.drawTexture(x, y, cardWidth * 2, cardHeight * 2, seatID, ROTATABLE, 0, false);
		//Once you feel confident enough, this could totally be chips???
		x = centerX - cardSeparation * sin(radians);
		y = centerY + cardSeparation * cos(radians);
		SDL_Color color = { 255, 255, 255 };
		std::stringstream ss;
		if (players[i].currentBet != 0) {
			ss << players[i].currentBet;
			pokerScreen.drawText(ss.str(), color, 0, x, y, cardWidth * 2, cardHeight * 2, ROTATABLE, 0, false);
		}
		else if (players[i].ifChecked) {
			ss << "check";
			if (i != 0)
				pokerScreen.drawText(ss.str(), color, 0, x, y, cardWidth * 4, cardHeight * 4, ROTATABLE, theta, false);
			else 
				pokerScreen.drawText(ss.str(), color, 0, x, y, cardWidth * 4, cardHeight * 4, ROTATABLE, 180 - theta, false);
		}
		ss.str("");
		//ss << players[i].handName;
		ss << players[i].money;
		x = centerX + cardSeparation * sin(radians);
		y = centerY - cardSeparation * cos(radians);
		if (players[i].money != -1) {
			if (i != 0)
				pokerScreen.drawText(ss.str(), color, 0, x, y, cardWidth * 4, cardHeight * 4, ROTATABLE, theta, false);
			else {
				ss.str("");
				ss << players[i].handName;
				if (ss.str() != "")
					pokerScreen.drawText(ss.str(), color, 0, x, y, cardWidth * 4, cardHeight * 4, ROTATABLE, theta - 180, false);
			}
		}
		ss.str("");

		x = centerX - cardSeparation * sin(radians) - cardSeparation * cos(radians);
		y = centerY + cardSeparation * cos(radians) - cardSeparation * sin(radians);
		if (players[i].blindFlag == SMALLBLIND)
			pokerScreen.drawTexture(x, y, cardWidth * 3, cardHeight * 3, smallBlindID, ROTATABLE, 0, false);
		else if (players[i].blindFlag == BIGBLIND)
			pokerScreen.drawTexture(x, y, cardWidth * 3, cardHeight * 3, bigBlindID, ROTATABLE, 0, false);
		else if (players[i].blindFlag == DEALER)
			pokerScreen.drawTexture(x, y, cardWidth * 3, cardHeight * 3, dealerID, ROTATABLE, 0, false);
		//ss << players[i].ranking;
		//x = centerX -  cardSeparation * sin(radians);
		//y = centerY +  cardSeparation * cos(radians);
		//if (players[i].ranking != 0)
			//pokerScreen.drawText(ss.str(), color, 0, x, y, cardWidth * 4, cardHeight * 4, ROTATABLE, 0);
	}
	
	cardSeparation = 7;
	//Be sure that you will soon be able to draw the opponent's hands rotated
	//Draw Opponent Hand

	/*
	x = 50 - ((players[1].hand.size() - 1) * cardSeparation / 2);
	for (unsigned i = 0; i < players[1].hand.size(); i++) {
		if (players[1].hand[i].getIfCovered())
			pokerScreen.drawTexture(x, 15, cardWidth, cardHeight, _deck.getBackID(), CENTER, 0);
		else
			pokerScreen.drawTexture(x, 15, cardWidth, cardHeight, players[1].hand[i].getID(), CENTER, 0);
		x += cardSeparation;
	}
	*/
	//MAX six other players
	

	//Draw Community Cards
	x = 50 - (communityCards.size() - 1) * 3.5;
	for (unsigned i = 0; i < communityCards.size(); i++) {
		if (communityCards[i].getIfCovered())
			pokerScreen.drawTexture(x, 50, cardWidth, cardHeight, _deck.getBackID(), CENTER, 0, false);
		else
			pokerScreen.drawTexture(x, 50, cardWidth, cardHeight, communityCards[i].getID(), CENTER, 0, checkIfBest(communityCards[i], 0));
		x += 7;
	}

	std::stringstream ss;
	SDL_Color color = { 255, 255, 255 };

	ss << "Money: " << players[0].money;
	pokerScreen.drawText(ss.str(), color, 0, 100, 0, 15, 15, UPLEFT, 0, false);
	ss.str("");

	ss << "Round " << roundNumber;
	pokerScreen.drawText(ss.str(), color, 0, 0, 0, 20, 20, UPLEFT, 0, false);
	ss.str("");

	ss << "Blind: " << blind;
	pokerScreen.drawText(ss.str(), color, 0, 0, 6, 20, 20, UPLEFT, 0, false);
	ss.str("");

	ss << "Pot: " << pot;
	if (pot != 0)
		pokerScreen.drawText(ss.str(), color, 0, 50, 35, 15, 15, CENTER, 0, false);
	ss.str("");

	if (round == REVEAL) {
		ss << "The winner is player ";
		int winnerIndex = -1;
		for (unsigned i = 0; i < players.size(); i++) {
			if (players[i].ranking == 1)
				winnerIndex = i + 1;
		}
		ss << winnerIndex << "!!!!";

		pokerScreen.drawText(ss.str(), color, 0, 50, 65, 30, 30, CENTER, 0, false);
		pokerScreen.drawText("Would you like to play again?", color, 0, 50, 68, 30, 30, CENTER, 0, false);
	}

	//if (players[0].handName != "")
		//pokerScreen.drawText(players[0].handName, color, 0, 50, 92, 20, 20, CENTER, 0);
	//if (players[1].handName != "")
		//pokerScreen.drawText(players[1].handName, color, 0, 50, 4, 20, 20, CENTER, 0);

	return true;
}

//Organizes hand 
void Poker::organizeHand(std::vector <Card> &playerHand) {
	std::vector <Card> tempHand = playerHand;

	playerHand.clear();
	int size = tempHand.size();
	for (int i = 0; i < size; i++) {
		int max = 0, maxID = -1; 
		for (unsigned j = 0; j < tempHand.size(); j++) {
			int rank;
			if (tempHand[j].getRank() == 1)
				rank = 14;
			else
				rank = tempHand[j].getRank();

			if (rank > max) {
				max = rank;
				maxID = j;
			}
		}
		playerHand.push_back(tempHand[maxID]);
		tempHand[maxID] = tempHand.back();
		tempHand.pop_back();
	}

	tempHand.clear();
}

bool Poker::checkIfBest(Card card, int player) {
	bool success = false;
	if (player == 0) {
		int numHighlightedCards = 0;
		switch (players[player].handValue) {
		case HIGHCARD:
			numHighlightedCards = 0;
			break;
		case PAIR:
			numHighlightedCards = 2;
			break;
		case THREEOFAKIND:
			numHighlightedCards = 3;
			break;
		case FOUROFAKIND:
		case TWOPAIR:
			numHighlightedCards = 4;
			break;
		case FULLHOUSE:
		case STRAIGHTFLUSH:
		case STRAIGHT:
		case FLUSH:
			numHighlightedCards = 5;
			break;
		}
		for (int i = 0; i < numHighlightedCards; i++) {

			if (card.getID() == players[player].bestHand[i].getID())
				success = true;
		}
	}
	return success;
}

