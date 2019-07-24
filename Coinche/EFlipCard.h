#pragma once
#include "EntityState.h"
#include <SFML\System.hpp>

class EFlipCard : public EntityState<Card>
{
public:
	EFlipCard(void);
	~EFlipCard(void);

	void init(Card& parent);
	virtual void free(Card& parent);
	virtual void update(Card& parent, const Game& game, unsigned int elapsedTime);

protected:
	double myInitialSizeX;
	double myInitialSizeY;

	double myInitialX;

	bool myIsFirstPhase;
	sf::Clock myClock;
	sf::Clock myClock2;
};

