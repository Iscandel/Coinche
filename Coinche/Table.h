#ifndef H__TABLE_180120151237__H
#define H__TABLE_180120151237__H

#include "CardHandler.h"

class Game; 
class Table : public CardHandler<Table>
{
public:
	Table(void);
	~Table(void);

	void update(const Game& game, unsigned int elapsedTime);
};

#endif
