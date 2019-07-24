#ifndef H__GAME_171020131934__H
#define H__GAME_171020131934__H

#include "Player.h"
//#include "Card.h"
#include "Deck.h"
#include "EntityManager.h"
#include "GameServer.h"
#include "Table.h"
#include "TrickSet.h"
#include "Rules.h"

#include <string>

struct PointsAndWinning
{
	int myWinningTeamId;
	std::string myEndSentence;
};

class Game
{
public:
	Game(void);
	~Game(void);

	PtrPlayer addPlayer(PtrPlayer player);
	PtrPlayer addPlayer();
	PtrPlayer addIA(PtrPlayer ia);
	PtrPlayer addIA();
	void removePlayer(int id);
	void removePlayerByPosition(int position);
	//void setNumberPlayers(int number);

	int getNumberPlayers() const {return myPlayers.size();}
	PtrPlayer getPlayerByPosition(int i) {return myPlayers[i];}

	PtrPlayer getPlayerById(int id);

	EntityManager& getEntityManager() {return myEntityManager;}

	void distributeCards();

	//int createNewPlayerId();

	void setPlayerId(int id);
	int getPlayerId() const {return myPlayerId;}

	//void setNumberHumanPlayers(int nb) {myNumberHumanPlayers = nb;}
	//int getNumberHumanPlayers() const {return myNumberHumanPlayers;}

	Deck& getDeck() {return myDeck;}

	GameServer& getServerData() {return myGameServerData;}

	//Affects to the player the first position (0) in the players vector and 
	//shifts accordingly the others
	void affectPlayerPositionToSouth();

	void createDeck();
	void createDeck(int nbCards, const std::string& data);

	void resetDeck();

	int getDealerId() const {return myDealerId;}
	void setDealerId(int id);

	int getPlayerIdToPlay() const {return myPlayerIdToPlay;}
	//Client function only
	void setPlayerIdToPlay(int id);
	void setFirstIdToPlay(int position);
	int getFirstIdToPlay() const {return myFirstIdToPlay;}

	void nextPlayerToPlay();

	int getPlayerPosition(int id) const;

	Table& getTable() {return myTable;}

	TrickSet& getTrickSet() {return myTricks;}

	void update(unsigned int elapsedTime);

	void addTrick(boost::shared_ptr<Trick> trick);
	void endTrick(int firstPosition, Point<double>& cardTrickPos);

	const Contract& getContract() const {return myContract;}
	void setContract(const Contract& contract) {myContract = contract;}

	std::pair<int, std::string> computePoints();

	bool playersHaveNoCards();

	void addPlayerTeam(int teamId, int playerId);
	std::map<int, PtrTeam> & getTeams() {return myTeams;}
	PtrTeam getTeam(int id);

	void addCurrentScorePlayToGlobal();
protected:
	void createCard(int id, int typeId);
	std::string generateRandomName();

protected:
	//Player* myDealer;
	int myDealerId;
	int myPlayerIdToPlay;
	int myFirstIdToPlay;
	std::vector<boost::shared_ptr<Player> > myPlayers;
	//std::vector<Card> myCards;
	Deck myDeck;
	EntityManager myEntityManager;
	int myPlayerId;
	int myNumberHumanPlayers;
	GameServer myGameServerData;
	Table myTable;
	TrickSet myTricks;
	Contract myContract;
	//teamId, team
	std::map<int, PtrTeam> myTeams;
	//std::vector<int> myLocalIds;
};

#endif