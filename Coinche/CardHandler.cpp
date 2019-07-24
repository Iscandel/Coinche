#include "CardHandler.h"


ICardHandler::~ICardHandler(void)
{
	removeCards();
}

void ICardHandler::removeCards()
{
	for(unsigned int i = 0; i < myCards.size(); i++)
	{
		removeCard(myCards[i]);
	}
}

PtrCard ICardHandler::removeCard(PtrCard card)
{
	return removeCard(card->getId());
}