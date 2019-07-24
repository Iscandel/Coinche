#ifndef H_GAMESERVER_290420141134_H
#define H_GAMESERVER_290420141134_H

#include <vector>

class GameServer
{
public:
	GameServer(void);
	~GameServer(void);

	bool addLocalId(int id);
	bool removeLocalId(int id);

	bool addNotLocalId(int id);
	bool removeNotLocalId(int id);

	bool removeId(int id);

	bool addIAId(int id);
	bool removeIAId(int id);
	const std::vector<int>& getIAIds() const {return myIAIds;}

	const std::vector<int>& getLocalIds() const {return myLocalIds;}

	void addPlayerReady(int id);
	void removePlayerReady(int id);
	void resetPlayersReady();

	bool isLocalId(int id) const;
	bool isNotLocalId(int id) const;

	bool allPlayersReady();
	
	//void setNumberHumanPlayers(int nb) {myNumberHumanPlayers = nb;}
	int getNumberHumanPlayers();// {return myNumberHumanPlayers;}

protected:
	bool _removeId(std::vector<int>& vec, int id);
	bool _addId(std::vector<int>& vec, int id);
		
protected:
	//int myNumberHumanPlayers;
	std::vector<int> myLocalIds;
	std::vector<int> myNotLocalIds;
	std::vector<int> myPlayersReady;
	std::vector<int> myIAIds;
};

#endif
