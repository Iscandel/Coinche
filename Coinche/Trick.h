#ifndef H__TRICK_250120150122__H
#define H__TRICK_250120150122__H

#include "CardHandler.h"

class Trick : public CardHandler<Trick>
{
public:
	Trick(int firstPosition, int winningPlayer);
	~Trick(void);

	int getFirstPosition() const {return myFirstPosition;}
	int getWinningPlayerId() const {return myWinningPlayerId;}

protected:
	//Position of the first player of this trick to play
	int myFirstPosition;
	int myWinningPlayerId;
};

#endif