#pragma once
#include<string>
class Card
{
public:
	Card();
	Card(int rank, int suitIdentifier);
	~Card();
	//setters (you probably shouldn't be able to change the suit or rank of the card though.
	void reveal() { _ifCovered = false; }
	void hide() { _ifCovered = true; }
	void setID(int id) { _id = id; }
	//getters
	std::string getSuit() { return _suit; }
	int getRank() { return _rank; }
	std::string getImageFileName() { return _imageFileName; }
	int getBlackJackValue() { if (_rank < 11) return _rank; if (_rank == 1) return 11; else return 10; }
	int getID() { return _id; }
	bool getIfCovered() { return _ifCovered; }

	//The index for this object in the texture vector of the screen
	int _id;

private:
	//1-13, 1, 11, 12, 13 are ace, jack, queen, and king respectively
	int _rank;
	std::string _rankName;

	//Either hearts, spades, clubs, or diamonds
	std::string _suit;

	//Whether the card is face down or not
	bool _ifCovered;

	//The image that you will use can be found in
	std::string _imageFileName;
	//You'll probably have to use stringstreams though...

	

};

