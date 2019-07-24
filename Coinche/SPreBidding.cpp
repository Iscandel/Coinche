#include "SPreBidding.h"

#include "Game.h"
#include "GameEngine.h"
#include "GameServer.h"
#include "GraphicEngine.h"
#include "Gaia/Gaia.h"
#include "NetworkEngine.h"
#include "SBidding.h"

SPreBidding::SPreBidding(void)
{
}


SPreBidding::~SPreBidding(void)
{
}

void SPreBidding::init()
{
	//if(!getNetworkEngine()->isMulti())
	//{
	//	setGameState(PtrGameState(new SBidding));
	//}
	if(getNetworkEngine()->isMulti() && getNetworkEngine()->isServer())
	{
		Message msg;
		msg.type = Message::MessageType::SV_GO_SPREBIDDING;
		sendMessageTCP(msg, false);
	}
	else if(getNetworkEngine()->isMulti())
	{
		Message msg;
		msg.type = Message::MessageType::CL_SPREBIDDING_READY;
		//sendMessageTCP(msg);
		clientRequestTCP(msg);
	}
}
void SPreBidding::update()
{
	processQueue();

	gaia::GuiManager::getInstance()->update(getGraphicEngine()->getElapsedTime());

	Game& game = getGameEngine()->getGame();
	GameServer server = game.getServerData();

	if(server.allPlayersReady())
	{
		server.resetPlayersReady();
		setGameState(PtrGameState(new SBidding));
		//Message msg;
		//msg.type = Message::MessageType::SV_GO_SBIDDING;
		//sendMessageTCP(msg);
	}
	
	//Game& game = getGameEngine()->getGame();
	//for(int i = 0; i < game.getNumberPlayers(); i++)
	//{
	//	PtrPlayer player = game.getPlayerByPosition(i);
	//	player->update(getGraphicEngine()->getElapsedTime());
	//}
}

bool SPreBidding::catchEvent(const sf::Event& ev)
{
	Game& game = getGameEngine()->getGame();
	EntityManager entityManager = game.getEntityManager();

	if(ev.Type == sf::Event::MouseMoved)
	{
	}

	return true;
}

void SPreBidding::processMessage(const Message& msg)
{
	switch(msg.type) //Gérer déco
	{
	case Message::MessageType::CL_SPREBIDDING_READY:
	{
		int id = msg.iData[0];

		Game& game = getGameEngine()->getGame();
		GameServer server = game.getServerData();
		server.addPlayerReady(id);

		if(server.allPlayersReady())
		{
			server.resetPlayersReady();
			setGameState(PtrGameState(new SBidding));
			
			//myPlayersReady.push_back(id);

		//if(allPlayersReady())
		}
	}
	break;

	case Message::MessageType::SV_GO_SBIDDING:
	{
		setGameState(PtrGameState(new SBidding));
	}
	break;

	default:
		break;
	}
}