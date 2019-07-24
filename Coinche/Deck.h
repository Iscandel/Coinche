#ifndef H__DECK_311220131507__H
#define H__DECK_311220131507__H

#include "CardHandler.h"

class Deck : public CardHandler<Deck>
{
public:
	Deck(void);
	~Deck(void);

	PtrCard getTop();

	bool isEmpty() {return (myCards.size() == 0);}
};

#endif