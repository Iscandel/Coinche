#pragma once
#include "Contract.h"
#include "EntityState.h"
#include "Card.h"
#include "Rules.h"
#include <map>

class Player;
class Game; 

class BasicIA : public EntityState<Player>
{
public:
	BasicIA(void);
	~BasicIA(void);

	void init(Player& parent);
	void free(Player& parent);
	void update(Player& parent, const Game& game, unsigned int elapsedTime);

protected:
	bool isTrumpRemaining(const Contract& contract);
	bool isMaster(Player& parent, const std::vector<Card::CardColor>& colorVec, std::vector<Card::CardColor>& masterVec);
	bool isMaster(Player& parent, Card::CardColor colorVec);
	bool hasCardColor(Player& parent, const std::vector<Card::CardColor>& colorVec);
	bool playMasterCard(Player& parent, const std::vector<Card::CardColor>& colorVec);
	bool playMasterCard(Player& parent, Card::CardColor colorVec);
	//bool tryPlayNotStrumpMasterCard(Player& parent, const Contract& contract);
	bool playSmallCard(Player& parent, const std::vector<Card::CardColor>& colorVec);
	bool playSmallCard(Player& parent, Card::CardColor colorVec);
	bool playGoodCard(Player& parent, const std::vector<Card::CardColor>& colorVec);
	bool playGoodCard(Player& parent, Card::CardColor colorVec);
	bool playSmallestGreaterCard(Player& parent, PtrCard referenceCard);
	bool is10SecondNonMaster(Player& player, Card::CardColor color);
	bool playCorrectTrump(Player& parent, Table& table, PtrCard holdingCard, Card::CardColor colorVec);

	std::vector<Card::CardColor> createNotTrumpVec(const Contract& contract);

	void updateRemainingCards(Game& game, Table& table);

protected:
	int mySleepingTime;

	//std::map<int, std::vector<int>> myCardsOrder;
	std::map<Card::CardColor, std::vector<CardPoint>> myCardsOrder;

};

struct CardsByPoints  
{  
    bool operator ()(PtrCard first, PtrCard second);
};  