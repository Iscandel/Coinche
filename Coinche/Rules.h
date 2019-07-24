#pragma once
//#include "Player.h"
//#include "Deck.h"
#include "Card.h"
#include "Contract.h"
#include "Team.h"

#include "tools/Singleton.h"

class Table;
class Player;
class Deck;
class Trick;
class TrickSet;

namespace rules
{
	enum CardType
	{
		TRUMP,
		NORMAL,
		NO_TRUMP
	};
}



struct CardPoint
{
	CardPoint(Card::CardValue value, int point) : myValue(value), myPoint(point) {}
	Card::CardValue myValue;
	int myPoint;
};

class Rules : public Singleton<Rules>
{	
public:
	static const int DECK_SIZE;

public:
	Rules(void);
	~Rules(void);

	void onTrick();

	void distributeCards(Deck& cards, std::vector<boost::shared_ptr<Player> >& vec);//(std::vector<PtrCard>& cards, std::vector<Player*>& vec);

	std::vector<PtrCard> shuffle(Deck& vec);

	bool isRandomShuffle() const {return myIsDeckShuffledForGame;}
	bool isAllNoTrumpAllowed() const {return myIsAllNoTrumpAllowed;}
	//Rules::BiddingType getCurrentTrump() const {return myCurrentTrump;}
	bool isDeclarationsAllowed() const {return myIsDeclarationsAllowed;}
	bool isDeckShuffledForGame() const {return myIsDeckShuffledForGame;}
	int getNumberCardsPerPlayer() const {return myNumberCardsPerPlayer;}
	int getNumberCardsPerDeck() const {return myNumberCardsPerDeck;}
	int getNumberCardsPerTrick() const {return getNumberOfPlayers();}
	int getNumberOfPlayers() const {return myNumberOfPlayers;}
	//const Contract& getCurrentContract() const {return myCurrentContract;}


	void setRandomShuffle(bool shuffle) {myIsDeckShuffledForGame = shuffle;}
	void setAllNoTrumpAllowed(bool allowed) {myIsAllNoTrumpAllowed = allowed;}
	void setDeclarationAllowed(bool allowed) {myIsDeclarationsAllowed = allowed;}
	void setNumberOfPlayers(int number);
	
	//void setContract(Contract& contract) {myCurrentContract = contract;}

	template<class T>
	int getNumberCards() const;

	bool validateCardPlayed(Game& game, boost::shared_ptr<Player> player, PtrCard card);

	std::vector<Trick> lookLastTricks(const TrickSet& set);

	boost::shared_ptr<Player> getTrickHolder(Table& table, Game& game, int positionFirstPlayer, const Contract& contract, PtrCard& holdingCard);

	int sumPoints(const std::vector<PtrCard>, const Contract& contract, bool lastTrick);
	bool checkContractAndComputeScore(Game& game, const Contract& contract, std::map<int, int>& tricksPerPlayerId);
	bool isGeneral(PtrTeam team, std::map<int, int>& tricksPerPlayerId);
	bool isCapot(PtrTeam team, std::map<int, int>& tricksPerPlayerId);
	const std::map<rules::CardType, std::vector<CardPoint>>& getCardsOrder() const {return myCardsOrder;}
	
	bool cardIsGreater(PtrCard card, PtrCard reference, const Contract& contract);
	bool hasCardColor(const std::vector<PtrCard>& playerCards, Card::CardColor color);
	bool hasGreaterCards(const std::vector<PtrCard>& playerCards,PtrCard holdingCard, const Contract& contract);

protected:
	void setNumberCardsPerPlayer(int number) {myNumberCardsPerPlayer = number;}
	void setNumberCardsPerDeck(int number) {myNumberCardsPerDeck = number;}

protected:
	bool myIsAllNoTrumpAllowed;
	bool myIsDeclarationsAllowed;
	//Rules::BiddingType myCurrentTrump;
	bool myIsDeckShuffledForGame;
	int myNumberCardsPerPlayer;
	int myNumberCardsPerDeck;
	int myNumberOfPlayers;
	//int myNumberCardsPerTrick;
	Contract myCurrentContract;

	std::map<rules::CardType, std::vector<CardPoint>> myCardsOrder;

};

template<class T>
int Rules::getNumberCards() const
{
	return getNumberCardsPerPlayer();
}

template<>
inline int Rules::getNumberCards<Deck>() const
{
	return getNumberCardsPerDeck();
}

template<>
inline int Rules::getNumberCards<Table>() const
{
	return getNumberOfPlayers();
}

template<>
inline int Rules::getNumberCards<Trick>() const
{
	return getNumberOfPlayers();
}

//template<>
//inline int Rules::getNumberCards<Table>() const
//{
//	return getNumberCardsPerTrick();
//}


