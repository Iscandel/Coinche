#ifndef H__EMOVETOTABLE_190120151400__H
#define H__EMOVETOTABLE_190120151400__H

#include "EntityState.h"


class EMoveToTable : public EntityState<Card>
{
public:
	EMoveToTable(const Point<double>& destination);
	void init(Card& parent);
	virtual void free(Card& parent);
	virtual void update(Card& parent, const Game&, unsigned int elapsedTime);

protected:
	Point<double> myDestination;
	Point<double> speedVector;
};

#endif
