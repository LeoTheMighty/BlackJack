#include <sstream>
#include "Card.h"

Card::Card() : _rank(0), _suit(""), _ifCovered(true), _id(-1)
{

}

Card::Card(int rank, int suitIdentifier)
{
	std::stringstream ss;

	_rank = rank;

	switch (rank) {
	case 1:
		ss << "ace";
		break;
	case 11:
		ss << "jack";
		break;
	case 12:
		ss << "queen";
		break;
	case 13:
		ss << "king";
		break;
	default:
		ss << rank;
		break;
	}

	_rankName = ss.str();
	ss.str("");

	if (suitIdentifier == 1) _suit = "hearts";
	if (suitIdentifier == 2) _suit = "spades";
	if (suitIdentifier == 3) _suit = "clubs";
	if (suitIdentifier == 4) _suit = "diamonds";

	ss << "PlayingCards/" << _rankName << "_of_" << _suit << ".png";
	_imageFileName = ss.str();

	_id = -1;

}


Card::~Card()
{
}
