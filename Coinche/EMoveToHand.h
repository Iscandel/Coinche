#pragma once
#include "EntityState.h"

class EMoveToHand : public EntityState<Card>
{
public:
	EMoveToHand(const Point<double>& destination);
	void init(Card& parent);
	virtual void free(Card& parent);
	virtual void update(Card& parent, const Game& game, unsigned int elapsedTime);

protected:
	Point<double> myDestination;
	Point<double> speedVector;
};


