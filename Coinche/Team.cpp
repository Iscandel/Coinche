#include "Team.h"


Team::Team(int id)
:myId(id)
,myToDestroy(false)
,myScore(0)
,myCurrentPlayScore(0)
{
}


Team::~Team(void)
{
}

void Team::addCurrentScorePlayToGlobal()
{
	addGlobalScore(getCurrentPlayScore());
	resetCurrentPlayScore();
}

void Team::addPlayer(int id)
{
	if(!isPlayerIdPresent(id))
		myPlayerIds.push_back(id);

	myToDestroy = false;
}

bool Team::removePlayerId(int id)
{
	for(unsigned int i = 0; i < myPlayerIds.size(); i++)
	{
		if(myPlayerIds[i] == id)
		{
			myPlayerIds.erase(myPlayerIds.begin() + i);	
			if(myPlayerIds.size() == 0)
				myToDestroy = true;
			return true;
		}
	}


	
	return false;
}

bool Team::isPlayerIdPresent(int id)
{
	for(unsigned int i = 0; i < myPlayerIds.size(); i++)
	{
		if(myPlayerIds[i] == id)
		{
			return true;
		}
	}
	
	return false;
}