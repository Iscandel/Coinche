#pragma once
#include "GameState.h"
#include "Player.h"

#include <string>


class SLobby :
	public GameState
{
public:
	SLobby(void);
	~SLobby(void);

	void init();

	///////////////////////////////////////////////////////////////////////////
	/// Pauses the game state.
	///////////////////////////////////////////////////////////////////////////
	virtual void pause() {}

	///////////////////////////////////////////////////////////////////////////
	/// Resumes the game state.
	///////////////////////////////////////////////////////////////////////////
	virtual void resume() {}

	///////////////////////////////////////////////////////////////////////////
	/// Updates the game state.
	///////////////////////////////////////////////////////////////////////////
	virtual void update();

	virtual void processMessage(const Message& msg);

	bool catchEvent(const sf::Event& ev);

	PtrPlayer addPlayer(int id, int team, const std::string& nickname);
	PtrPlayer addPlayer();
	PtrPlayer addIA(int id, int team, const std::string& nickname = "");
	PtrPlayer addIA();
	void removeEntity(int id);
	bool isNewId(int id);

	std::string formatPlayerList();

	void changeTeam(PtrPlayer player, int idNewTeam);//const Team& newTeam);
};

