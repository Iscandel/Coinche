#ifndef H__TEAM_311220131438__H
#define H__TEAM_311220131438__H

//#include "Player.h"

#include <boost/smart_ptr.hpp>

#include <vector>

class Team
{
public:
	Team(int id = -1);
	~Team();

	void setTeam(int id) {myId = id;}
	int getTeamId() const {return myId;}

	void addCurrentPlayScore(int points) {myCurrentPlayScore = points;}
	int getCurrentPlayScore() const {return myCurrentPlayScore;}
	void resetCurrentPlayScore() {myCurrentPlayScore = 0;}

	void addGlobalScore(int points) {myScore += points;}
	int getGlobalScore() const {return myScore;}
	void resetGlobalScore() {myScore = 0;}

	void addPlayer(int id);
	bool removePlayerId(int id);
	bool isPlayerIdPresent(int id);
	const std::vector<int>& getPlayerIds() const {return myPlayerIds;}

	void addCurrentScorePlayToGlobal();

	bool mustBeDestroyed() {return myToDestroy;}

protected:
	int myScore;
	int myCurrentPlayScore;
	int myId;
	std::vector<int> myPlayerIds;
	bool myToDestroy;
};

typedef boost::shared_ptr<Team> PtrTeam;

#endif