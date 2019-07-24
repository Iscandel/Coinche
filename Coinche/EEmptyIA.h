#ifndef H_EEMPTYIA_190120152219__H
#define H_EEMPTYIA_190120152219__H

#include "EntityState.h"

class Player;

class EEmptyIA : public EntityState<Player>
{
public:
	EEmptyIA(void);
	~EEmptyIA(void);

	void init(Player&) {}
	void free(Player&) {}
	void update(Player&, const Game&, unsigned int) {}
};

#endif