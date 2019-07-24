#include "Table.h"


Table::Table(void)
{
}


Table::~Table(void)
{
}

void Table::update(const Game& game, unsigned int elapsedTime)
{
	for(int i = 0; i < myCards.size(); i++)
	{
		myCards[i]->update(game, elapsedTime);
	}
}