#include <random>
#include <ctime>
#include "Deck.h"

Deck::Deck()
{
	init();
	_backImageFileName = "PlayingCards/back_of_card.png";
}


Deck::~Deck()
{

}

void Deck::init()
{
	_cards.clear();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			_cards.push_back(Card(j + 1, i + 1));
		}
	}
}

void Deck::shuffle()
{
	std::vector<Card> holderDeck = _cards;
	_cards.clear();

	static std::default_random_engine randomEngine((unsigned int)time(NULL));
	int max = holderDeck.size();
	for (int i = 0; i < max; i++) {
		std::uniform_int_distribution<int> roll(0, holderDeck.size() - 1);
		int index = roll(randomEngine);

		_cards.push_back(holderDeck[index]);
		holderDeck.erase(holderDeck.begin() + index);
	}
}

Card Deck::takeCardFromTop()
{
	Card card = _cards[0];
	_cards.erase(_cards.begin());
	return card;
}

void Deck::putCardOnBottom(Card card) {
	_cards.push_back(card);
	_cards.back().hide();
}