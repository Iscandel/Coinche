#include "GameState.h"
#include "GameEngine.h"
#include "NetworkEngine.h"

//#include "Game.h"

GameState::GameState()
:myIsPaused(false)
,myIsVisible(true)
{
	init();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
GameState::~GameState(void)
{
	free();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void GameState::setGameState(PtrGameState state)
{
	getGameEngine()->setGameState(state);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//void GameState::sendMessage(const Message& m)
//{
//	if(getNetworkEngine()->isMulti() && !getNetworkEngine()->isServer())
//	{
//		getNetworkEngine()->sendMessage(m);
//	}
//	else
//	{
//		getGameEngine()->processMessage(m); //?
//	}
//			
//}

void GameState::sendMessageTCP(const Message& m, int id)
{
	//processMessage(m); ?
	if(getNetworkEngine()->isMulti() && getNetworkEngine()->isServer() && isNotLocalId(id))
	{
		getNetworkEngine()->sendMessageTCP(m, id);
	}			
}

//pas la même fonction. Cas non connecté, le serveur doit envoyer en processMsg 
//seulement si le message est explicitement destiné aux locaux. Le client du serveur, 
//qui est aussi isServer(), pose problème
void GameState::sendMessageTCP(const Message& m, bool toLocal)
{
	if(getNetworkEngine()->isMulti() && getNetworkEngine()->isServer())
	{
		getNetworkEngine()->sendMessageTCP(m);
	}			
	
	if(toLocal)
	{
		getGameEngine()->processMessage(m); //?
	}

	//if(getNetworkEngine()->isMulti()) && getNetworkEngine()->isServer())
	//{
	//	getNetworkEngine()->sendMessageTCP(m);
	//	if(toLocal)
	//	{
	//		getGameEngine()->processMessage(m); //?
	//	}
	//}			
	//else if(!getNetworkEngine()->isMulti())
	//{
	//	if(toLocal)
	//	{
	//		getGameEngine()->processMessage(m); //?
	//	}
	//}
}

void GameState::clientRequestTCP(Message& m)
{
	//Si multi et non client du serveur
	if(getNetworkEngine()->isMulti() && !getNetworkEngine()->isServer())
	{
		getNetworkEngine()->sendMessageTCP(m);
	}			
	else 
	{
		m.iData[0] = getGameEngine()->getGame().getPlayerId();
		getGameEngine()->processMessage(m); //?
	}
}

void GameState::sendMessageTCPToAllExceptOne(const Message& m, int id)
{
	if(getNetworkEngine()->isMulti() && getNetworkEngine()->isServer())
	{
		Game& game = getGameEngine()->getGame();
		for(int i = 0; i < game.getNumberPlayers(); i++)
		{
			if(game.getPlayerByPosition(i)->getId() != id && isNotLocalId(game.getPlayerByPosition(i)->getId()))
				getNetworkEngine()->sendMessageTCP(m, game.getPlayerByPosition(i)->getId());
		}
	}			
}

bool GameState::isNotLocalId(int id) 
{
	Game& game = getGameEngine()->getGame();
	const GameServer& server = game.getServerData();
	return server.isNotLocalId(id);
	//if(id != game.getPlayerId())
	//	return true;

	//return false;
}

void GameState::draw()
{
	if(myView)
		myView->draw(*this);
}


