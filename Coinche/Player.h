#pragma once

#include <vector>
#include <boost/smart_ptr.hpp>

#include "Card.h"
#include "CardHandler.h"
//#include "Team.h"

class Game;

class Player : public Entity, public CardHandler<Player>
{
public:
	Player(int id, Game* game = NULL);
	~Player(void);

	void sortCards();

	//void clearHand() {myCards.clear();}
	//bool addCard(PtrCard c);
	//PtrCard removeCard(Card::CardValue value, Card::CardColor color);

	void update(const Game& game, unsigned int elapsedTime);

	void setState(boost::shared_ptr<EntityState<Player> > state);

	void setName(const std::string& name) {myName = name;}

	const std::string& getName() const {return myName;}

	void setGame(Game* game) {myGame = game;}
	const Game& getGame() const {return *myGame;}
	Game& getGame() {return *myGame;}

	//void setTeam(PtrTeam team) {myTeam = team;}

	//PtrTeam getTeam() {return myTeam;}

	//void isHuman() {return myIsHuman;}

	//void setGame(const Game& game) {myGame = game;}
	//const Game& getGame() {return myGame;}

protected:
	//PtrTeam myTeam;
	//std::vector<PtrCard> myCards;
	std::string myName;
	boost::shared_ptr<EntityState<Player> > myState;

	Game* myGame;
};

typedef boost::shared_ptr<Player> PtrPlayer;
