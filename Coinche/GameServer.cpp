#include "GameServer.h"

#include "tools/Logger.h"


GameServer::GameServer(void)
{
}


GameServer::~GameServer(void)
{
}

bool GameServer::addLocalId(int id)
{
	return _addId(myLocalIds, id);
}

bool GameServer::removeLocalId(int id) 
{
	return _removeId(myLocalIds, id);
	//for(unsigned int i = 0; i < myLocalIds.size(); i++)
	//{
	//	if(myLocalIds[i] == id) 
	//		myLocalIds.erase(myLocalIds.begin() + id);
	//}
}

bool GameServer::addNotLocalId(int id)
{
	return _addId(myNotLocalIds, id);
}

bool GameServer::removeNotLocalId(int id)
{
	return _removeId(myNotLocalIds, id);
	//for(unsigned int i = 0; i < myNotLocalIds.size(); i++)
	//{
	//	if(myNotLocalIds[i] == id) 
	//		myNotLocalIds.erase(myNotLocalIds.begin() + id);
	//}
}

bool GameServer::_removeId(std::vector<int>& vec, int id)
{
	for(unsigned int i = 0; i < vec.size(); i++)
	{
		if(vec[i] == id) 
		{
			vec.erase(vec.begin() + i);
			return true;
		}
	}

	return false;
}

bool GameServer::removeId(int id)
{
	int res = 0;
	res += removeLocalId(id);
	res += removeNotLocalId(id);
	res += removeIAId(id);
	return (bool) res;
}

bool GameServer::isLocalId(int id) const
{
	for(unsigned int i = 0; i < myLocalIds.size(); i++)
	{
		if(myLocalIds[i] == id) 
			return true;
	}

	return false;
}

bool GameServer::isNotLocalId(int id) const
{
	return !isLocalId(id);
}

void GameServer::addPlayerReady(int id)
{
	for(unsigned int i = 0; i < myPlayersReady.size(); i++)
	{
		if(myPlayersReady[i] == id)
		{
			ILogger::log(ILogger::ALL) << "Player " << id << " was already ready.\n";
			return;
		}
	}

	for(unsigned int i = 0; i < myNotLocalIds.size(); i++)
	{
		if(myNotLocalIds[i] == id)
		{
			myPlayersReady.push_back(id);
			ILogger::log(ILogger::ALL) << "Player " << id << " is ready.\n";
			return;
		}
	}
	
}

void GameServer::removePlayerReady(int id)
{
	for(unsigned int i = 0; i < myPlayersReady.size(); i++)
	{
		if(myPlayersReady[i] == id)
		{
			myPlayersReady.erase(myPlayersReady.begin() + i);
			return;
		}
	}
}
	
void GameServer::resetPlayersReady()
{
	myPlayersReady.clear();
}

bool GameServer::allPlayersReady()
{
	if(myPlayersReady.size() == myNotLocalIds.size())
		return true;
	return false;
}

bool GameServer::addIAId(int id)
{
	_addId(myLocalIds, id);
	return _addId(myIAIds, id);
}
	
bool GameServer::removeIAId(int id)
{
	_removeId(myLocalIds, id);
	return _removeId(myIAIds, id);

}

bool GameServer::_addId(std::vector<int>& vec, int id)
{
	for(unsigned int i = 0; i < vec.size(); i++)
	{
		if(vec[i] == id) 
			return false;
	}

	vec.push_back(id);
	return true;
}

int GameServer::getNumberHumanPlayers()
{
	return myLocalIds.size() - myIAIds.size();
}


//Dupliquer les données du client ?