#include "SLobby.h"

#include "GameEngine.h"
#include "NetworkEngine.h"
#include "Game.h"
#include "EEmptyIA.h"
#include "MessageBuilder.h"
#include "SPreBidding.h"
#include "tools/Logger.h"
#include "tools/Tools.h"
#include "Team.h"


SLobby::SLobby(void)
{
}


SLobby::~SLobby(void)
{
}

void SLobby::update()
{
}

void SLobby::processMessage(const Message& msg)
{
	switch(msg.type)
	{
	case Message::MessageType::SV_CONNECTION_RESULT:
	{
		if(msg.iData[Message::Key::SvConnectionResult::RESULT] == 1)
		{
			std::string name = "Yoo";
			//sendMessageTCP(MessageBuilder::ClConnection(name, false));//, false);
			clientRequestTCP(MessageBuilder::ClConnection(name, false));
			ILogger::log() << "CL : player " << name << " connected.\n";
		}
		else
		{
			//disconnect
		}
	}
	break;
	case Message::MessageType::CL_CONNECTION: //Pour le serveur
	{
		Game& game = getGameEngine()->getGame();
		//PtrPlayer player = addPlayer(game.createNewPlayerId()); // ?
		PtrPlayer player = addPlayer();
	
		std::string name = msg.sData[Message::Key::ClConnection::NAME];
		int tmpId = msg.iData[Message::Key::ClConnection::TMP_ID];
		
		player->setName(name);

		Message msgToSend = MessageBuilder::SvNewPlayer(player->getId(), -1, name);
		sendMessageTCPToAllExceptOne(msgToSend, player->getId());

		ILogger::log() << "SV : player " << name << " connected.\n";

		if(msg.iData[Message::Key::ClConnection::IS_LOCAL_CLIENT] == 1)
		{
			game.setPlayerId(player->getId());
		}
		else
		{			
			msgToSend = MessageBuilder::SvPlayerCreated(player->getId(), tmpId); //Pour l'asso id <-> socket
			getNetworkEngine()->processMessage(msgToSend);

			msgToSend = MessageBuilder::SvPlayerList(game.getNumberPlayers(), formatPlayerList());
			getNetworkEngine()->sendMessageTCP(msgToSend, player->getId());

			msgToSend = MessageBuilder::SvPlayerYourId(player->getId());
			getNetworkEngine()->sendMessageTCP(msgToSend, player->getId()); //pas d'id joueur encore assigné au jeu, le message est envoyé, même si on est serveur. plus valable


			//sendMessageToNetwork(msgToSend);
		}
		break;
	}
	case Message::MessageType::SV_NEW_PLAYER: //new player connected
	{
		int id = msg.iData[Message::Key::SvNewPlayer::ID];
		int team = msg.iData[Message::Key::SvNewPlayer::TEAM];
		std::string name = msg.sData[Message::Key::SvNewPlayer::NAME];

		if(isNewId(id))
		{
			PtrPlayer player = addPlayer(id, team, name);

			//changeTeam(player, team);
			//player->setName(name);
		}
	
		break;
	}

	case Message::MessageType::SV_PLAYER_LIST:
	{
		Game& game = getGameEngine()->getGame();

		int nbPlayers = msg.iData[Message::Key::SvPlayerList::NB_PLAYERS];
	
		std::string list = msg.sData[Message::Key::SvPlayerList::LIST];
		for(unsigned int i = 0; i < nbPlayers; i++)
		{ 
			//std::string id, team, name;

			//id
			int pos = list.find(' ');
			int id = tools::stringToNum<int>(list.substr(0, pos));
			list = list.substr(pos + 1, list.size() - 1);

			//Team
			pos =  list.find(' ');
			int team = tools::stringToNum<int>(list.substr(0, pos));
			list = list.substr(pos + 1, list.size() - 1);

			//Nickname length
			pos = list.find(' ');
			int nameLength = tools::stringToNum<int>(list.substr(0, pos));
			list = list.substr(pos + 1, list.size() - 1);

			std::string nickname;
			if(nameLength != 0)
			{
				//Nickname
				nickname = list.substr(0, nameLength);
				list = list.substr(nameLength + 1, list.size() - 1);
			}

			PtrPlayer player = addPlayer(id, team, nickname);
			//player->setTeam(Team(team));
			//player->setName(nickname);
			//addPlayer(player);
			
			//if(i != nbPlayers - 1)
			//	list = list.substr(1, list.size() - 1);

			//std::istringstream stream(list);
			//list >> id >> team >> name;
			//PtrPlayer player(new Player(id));
			//changeTeam(player, team);
			//player->setName(nickname);
			
		}
		break;
	}

	case Message::MessageType::SV_PLAYER_YOUR_ID:
	{
		int id = msg.iData[Message::Key::SvPlayerYourId::ID];
		 
		Game& game = getGameEngine()->getGame();
		game.setPlayerId(id);
		break;
	}

	case Message::MessageType::SV_GO_SPREBIDDING:
	{
		setGameState(PtrGameState(new SPreBidding));
		break;
	}

	case Message::MessageType::SV_CHANGE_TEAM:
	{
		int idPlayer = msg.iData[Message::Key::svChangeTeam::ID_PLAYER];
		int teamId = msg.iData[Message::Key::svChangeTeam::TEAM_ID];

		Game& game = getGameEngine()->getGame();

		if(teamId != -1)
		{
			game.addPlayerTeam(teamId, idPlayer);
		}
	}
	break;

	default:
		break;
	}
}

void SLobby::init()
{
	//Game& game = getGameEngine()->getGame();

	//PtrPlayer player = addPlayer(game.createNewPlayerId());
	//changeTeam(player, 1);

	//player = addIA(game.createNewPlayerId());
	//changeTeam(player, 1);

	//player = addIA(game.createNewPlayerId());
	//changeTeam(player, 2);

	//player = addIA(game.createNewPlayerId());
	//changeTeam(player, 2);
}

bool SLobby::catchEvent(const sf::Event& ev)
{
	switch(ev.Type)
	{
	case sf::Event::KeyPressed:
		if(ev.Key.Code == sf::Keyboard::S)
		{
			getNetworkEngine()->listen(8205);
			sf::Sleep(500);
			sendMessageToGame(MessageBuilder::ClConnection("Moi", true));
			//sendMessageToNetwork(MessageBuilder::ClConnection("Moi"));
		}
		else if(ev.Key.Code == sf::Keyboard::C)
		{
			sendMessageToNetwork(MessageBuilder::ClConnection("Yoo", false));
		}
		else if(ev.Key.Code == sf::Keyboard::N)
		{
			Game& game = getGameEngine()->getGame();
			
			if(getNetworkEngine()->isServer())
			{
				PtrPlayer player = game.getPlayerByPosition(0);
				changeTeam(player, 1);
				
				//player = addIA(game.createNewPlayerId());
				//player = game.getPlayerByPosition(1);
				//changeTeam(player, 2);

				player = addIA();
				player = addIA();

				player = game.getPlayerByPosition(1);
				changeTeam(player, 2);

				player = game.getPlayerByPosition(2);
				changeTeam(player, 1);

				//player = addIA(game.createNewPlayerId());
				
				//changeTeam(player, 2);

				if(game.getNumberPlayers() == 3)
				{
					//player = addIA(game.createNewPlayerId());
					player = addIA();
				
				}

				player = game.getPlayerByPosition(3);
				changeTeam(player, 2);

			setGameState(PtrGameState(new SPreBidding));
			}

			//MessageBuilder
		}
	break;
	}

	return true;
}

PtrPlayer SLobby::addPlayer(int id, int teamId, const std::string& nickname)
{
	Game& game = getGameEngine()->getGame();
	PtrPlayer player(new Player(id));

	changeTeam(player, teamId);
	game.addPlayer(player); //addHumanPlayer
	
	player->setName(nickname);
	//game.setNumberHumanPlayers(game.getNumberHumanPlayers() + 1); //Les joueurs humains ne sont supprimés qu'en se déconnectant
	//Vérifier si local id ?

	return player;
}

PtrPlayer SLobby::addPlayer()
{
	Game& game = getGameEngine()->getGame();
	return game.addPlayer(); //addHumanPlayer
	//game.setNumberHumanPlayers(game.getNumberHumanPlayers() + 1); //Les joueurs humains ne sont supprimés qu'en se déconnectant
	//Vérifier si local id ?
}

PtrPlayer SLobby::addIA()
{
	Game& game = getGameEngine()->getGame();
	PtrPlayer player = game.addIA(); //addIA -> server.addLocalId

	Message msg = MessageBuilder::SvNewPlayer(player->getId(), -1, player->getName());
	sendMessageTCPToAllExceptOne(msg, player->getId());

	return player;
}

PtrPlayer SLobby::addIA(int id, int teamId, const std::string& nickname)
{
	Game& game = getGameEngine()->getGame();
	PtrPlayer player(new Player(id));
	player->setState(boost::shared_ptr<EEmptyIA>(new EEmptyIA));
	changeTeam(player, teamId);
	game.addIA(player); //addIA -> server.addLocalId

	if(nickname != "")
		player->setName(nickname);

	Message msg = MessageBuilder::SvNewPlayer(player->getId(), -1, nickname);
	sendMessageTCPToAllExceptOne(msg, player->getId());

	return player;
}

void SLobby::removeEntity(int id)
{
	Game& game = getGameEngine()->getGame();
	game.removePlayer(id);
	//id ou pos ?
}

void SLobby::changeTeam(PtrPlayer player, int idNewTeam)//const Team& newTeam);
{
	//Game& game = getGameEngine()->getGame();

	//if(idNewTeam != -1)
	//{
	//	game.addPlayerTeam(idNewTeam, player->getId());
	//}

	Message msg = MessageBuilder::svChangeTeam(player->getId(), idNewTeam);
	sendMessageTCP(msg, true);

	//for(unsigned int i = 0; i < game.getNumberPlayers(); i++)
	//{
	//	PtrPlayer otherPlayer = game.getPlayerByPosition(i);
	//	PtrTeam team = otherPlayer->getTeam();
	//	if(player != otherPlayer && team->getTeamId() == idNewTeam)
	//		player->setTeam(team);
	//	else
	//		player->setTeam(PtrTeam(new Team(idNewTeam)));
	//}
}

bool SLobby::isNewId(int id)
{
	Game& game = getGameEngine()->getGame();
	int nbPlayers = game.getNumberPlayers();

	for(int i = 0; i < nbPlayers; i++)
	{
		PtrPlayer player = game.getPlayerByPosition(i);
		if(player->getId() == id)
			return false;
	}

	return true;
}

std::string SLobby::formatPlayerList()
{
	Game& game = getGameEngine()->getGame();

	std::ostringstream oss;
	//oss << game.getNumberPlayers();
	
	for(unsigned int i = 0; i <  game.getNumberPlayers(); i++)
	{ 
			//std::string id, team, name;
		PtrPlayer player = game.getPlayerByPosition(i);
		
		//int teamId = player->getTeam() ? player->getTeam()->getTeamId() : 1;
		int teamId = game.getTeam(player->getId()) ? game.getTeam(player->getId())->getTeamId() : 1;
		oss << player->getId() << " " << teamId << " " << player->getName().size() << " " << player->getName() << " ";			
	}
	
	return oss.str();
}

//Problème des anciens messages (état antérieur) qu'un client pourrait toujours envoyer -> segmenter le game server