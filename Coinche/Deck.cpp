#include "Deck.h"


Deck::Deck(void)
{
}


Deck::~Deck(void)
{
}

PtrCard Deck::getTop()
{
	if(myCards.size() > 0)
	{
		return myCards[0];
	}

	return PtrCard();
}