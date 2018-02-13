#pragma once
#include <vector>
#include <string>
#include "Card.h"
class Deck
{
public:
	Deck();
	~Deck();
	//setters
	void init();
	void shuffle();
	Card takeCardFromTop();
	void putCardOnBottom(Card card);
	void setCardID(int index, int id) { _cards[index].setID(id); }
	void setBackID(int index) { backTextureID = index; }
	//getters
	int getSize() { return _cards.size(); }
	std::string getBackImageFileName() { return _backImageFileName; }
	Card lookAtCard(int index) { return _cards[index]; }
	int getBackID() { return backTextureID; }
private:
	int backTextureID;
	std::string _backImageFileName;
	std::vector <Card> _cards;
};

