#pragma once

#include "tools/Point.h"

class Card;
class Game;

template<class T>
class EntityState
{
public:
	EntityState(void) {}
	~EntityState(void){}

	//creature::etatJoueur getNom() const {return monNom;}
	//void setNom(creature::etatJoueur e) {monNom = e;}

	virtual void init(T& parent) = 0;
	virtual void free(T& parent) = 0;
	virtual void update(T& parent, const Game& game, unsigned int elapsedTime) = 0;

	//virtual std::vector<PtrImage> getImages(Creature& parent, Skin::Position elementSkin);

protected:
	//creature::etatJoueur monNom;
};

//typedef boost::shared_ptr<EntityState<Card> > PtrEntityState;