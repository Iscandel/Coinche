#include "SGame.h"

#include "BasicIA.h"
#include "EMoveToTable.h"
#include "GameEngine.h"
#include "GameView.h"
#include "GraphicEngine.h"
#include "MessageBuilder.h"
#include "NetworkEngine.h"
#include "Rules.h"
#include "TextureManager.h"
#include "tools/Logger.h"
//#include "tools/Tools.h"

#include <boost/bind.hpp>

SGame::SGame(void)
{
}


SGame::~SGame(void)
{
}

void SGame::init()
{	
	myIsTransition = false;
	setView(PtrView(new GameView("data/gameViewPos.txt")));
	//
	Contract contract;
	contract.myBidType = rules::CLUBS;
	contract.myTeamId = 1;
	contract.myValue = 100;
	contract.myCoincheType = rules::NOT_COINCHE;
	Game& game = getGameEngine()->getGame();
	game.setContract(contract); //tmp

	if(!getNetworkEngine()->isMulti() || getNetworkEngine()->isServer())
	{
		Game& game = getGameEngine()->getGame();

		Message msg;
		msg.type = Message::MessageType::SV_GO_SGAME;
		sendMessageTCP(msg, false);

		game.setDealerId(game.getPlayerByPosition(game.getNumberPlayers() - 1)->getId());
		//game.setPlayerIdToPlay(game.getDealerId());
		msg = MessageBuilder::svFirstPlayerToPlay(game.getFirstIdToPlay());
		sendMessageTCP(msg, false);

		const GameServer& server= game.getServerData();
		const std::vector<int>& vec = server.getIAIds() ;
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			game.getPlayerById(vec[i])->setState(boost::shared_ptr<BasicIA>(new BasicIA));
		}		
	}
}

bool SGame::catchEvent(const sf::Event& ev)
{
	Game& game = getGameEngine()->getGame();
	EntityManager entityManager = game.getEntityManager();

	if(ev.Type == sf::Event::MouseMoved)
	{
		std::vector<PtrCard> allEntities = entityManager.getEntities();
		for(unsigned int i = 0; i < allEntities.size(); i++)
		{
			allEntities[i]->setSelected(false);
		}

		std::vector<PtrGraphicEntity> entities;
		//for(int i = 0; i < game.getNumberPlayers(); i++)
		//{
		//	PtrPlayer player = game.getPlayerByPosition(i);

		//	std::vector<PtrGraphicEntity> tmp = player->getHoveredEntities(
		//			Point<double>(ev.MouseMove.X / (double)getGraphicEngine()->getWindowSizeX(),
		//						  ev.MouseMove.Y / (double)getGraphicEngine()->getWindowSizeY()),
		//			Point<int>(getGraphicEngine()->getWindowSizeX(),
		//					   getGraphicEngine()->getWindowSizeY())
		//	);
		//	entities.insert(entities.end(), tmp.begin(), tmp.end());
		//}

		PtrPlayer player = game.getPlayerById(game.getPlayerId());
		std::vector<PtrGraphicEntity> tmp = player->getHoveredEntities(
					Point<double>(ev.MouseMove.X / (double)getGraphicEngine()->getWindowSizeX(),
								  ev.MouseMove.Y / (double)getGraphicEngine()->getWindowSizeY()),
					Point<int>(getGraphicEngine()->getWindowSizeX(),
							   getGraphicEngine()->getWindowSizeY())
		);
		entities.insert(entities.end(), tmp.begin(), tmp.end());

		if(entities.size() != 0)
		{
			entities.back()->setSelected(true);
			//Message msg = MessageBuilder::clCardToPlay(entities.back()->getId());
			//clientRequestTCP(msg);
		}
	}
	//if(ev.Type == sf::Event::MouseMoved)
	//{
	//	std::vector<PtrGraphicEntity> allEntities = entityManager.getEntities();
	//	for(unsigned int i = 0; i < allEntities.size(); i++)
	//	{
	//		allEntities[i]->setSelected(false);
	//	}

	//	std::vector<PtrGraphicEntity> entities = 
	//		entityManager.getHoveredEntities(
	//			Point<double>(ev.MouseMove.X / getGraphicEngine()->getWindowSizeX(), 
	//						  ev.MouseMove.Y / getGraphicEngine()->getWindowSizeY())); //A modif
	//	if(entities.size() != 0)
	//	{
	//		entities.back()->setSelected(true);
	//		Message msg = MessageBuilder::clCardToPlay(entities.back()->getId());
	//		clientRequestTCP(msg);
	//	}
	//		
	//}

	if(ev.Type == sf::Event::MouseButtonReleased)
	{
		if(ev.MouseButton.Button == sf::Mouse::Left)
		{
			if(game.getPlayerIdToPlay() == game.getPlayerId())
			{
				PtrPlayer player = game.getPlayerById(game.getPlayerId());
				std::vector<PtrCard>& cards = player->getCards();
				for(unsigned int i = 0; i < cards.size(); i++)
				{
					if(cards[i]->isSelected())
					{		
						cards[i]->setSelected(false);
						if(Rules::getInstance()->validateCardPlayed(game, player, cards[i]))
						{
							Message msg = MessageBuilder::clCardToPlay(cards[i]->getId());
							clientRequestTCP(msg);
						}
					}
				}
			}
		}
	}
	return true;
}

void SGame::update()
{
	processQueue();

	Game& game = getGameEngine()->getGame();
	game.update(getGraphicEngine()->getElapsedTime());

	std::vector<TimerFunction>::iterator it = myTimerFunctions.begin();
	for(; it != myTimerFunctions.end();)
	{

		it->update(getGraphicEngine()->getElapsedTime());
		if(it->isFinished())
			it = myTimerFunctions.erase(it);
		else
			++it;
	}

	//for(int i = 0; i < game.getNumberPlayers(); i++)
	//{
	//	PtrPlayer player = game.getPlayerByPosition(i);
	//	player->update(game, getGraphicEngine()->getElapsedTime());
	//}

	//game.getTable().update(getGraphicEngine()->getElapsedTime());
}

void SGame::processMessage(const Message& msg)
{
	switch(msg.type)
	{
	case Message::MessageType::SV_PLAYER_TO_PLAY :
	{
		int playerToPlay = msg.iData[Message::Key::SvPlayerToPlay::PLAYER_ID];
		getGameEngine()->getGame().setPlayerIdToPlay(playerToPlay);
	}
	break;

	case Message::MessageType::SV_FIRST_PLAYER_TO_PLAY :
	{
		int firstPlayerToPlay = msg.iData[Message::Key::SvFirstPlayerToPlay::FIRST_PLAYER_ID];
		getGameEngine()->getGame().setFirstIdToPlay(firstPlayerToPlay);
	}
	break;

	case Message::MessageType::CL_PLAY_CARD :
	{
		if(myIsTransition)
			return;

		int playerId = msg.iData[Message::Key::clCardToPlay::ID];
		int cardId = msg.iData[Message::Key::clCardToPlay::CARD_ID];

		Game& game = getGameEngine()->getGame();
		PtrPlayer player = game.getPlayerById(playerId);
		if(player)
		{
			PtrCard card = player->getCard(cardId);
			if(card)
			{
				if(Rules::getInstance()->validateCardPlayed(game, player, card))
				{
					myIsTransition = true;

					Message msg = MessageBuilder::svChangeCardTypeId(playerId, cardId, card->getTypeId());
					sendMessageTCP(msg, false);

					msg = MessageBuilder::svCardToTable(playerId, cardId);
					sendMessageTCP(msg, true);

					//if(!getNetworkEngine()->isMulti() || getNetworkEngine()->isServer())
					//{
					//if(game.getPlayerIdToPlay() == game.getPlayerByPosition(game.getNumberPlayers() - 1)->getId())
					//{
					//	game.endTrick();
					//}
					////}

					//game.nextPlayerToPlay();
					//msg = MessageBuilder::svPlayerToPlay(game.getPlayerIdToPlay());
					//sendMessageTCP(msg, false);
				}
			}
			else
			{
				ILogger::log(ILogger::ERRORS) << "card " << cardId << " not found to be played.\n";
			}
		}
		else
		{
			ILogger::log(ILogger::ERRORS) << "player " << playerId << " not found to play.\n";
		}

	}
	break;

	case Message::MessageType::SV_CHANGE_CARD_TYPE_ID:
	{
		int playerId = msg.iData[Message::Key::svChangeCardTypeId::PLAYER_ID];
		int cardId = msg.iData[Message::Key::svChangeCardTypeId::CARD_ID];
		int cardTypeId = msg.iData[Message::Key::svChangeCardTypeId::CARD_TYPE_ID];

		Game& game = getGameEngine()->getGame();

		PtrPlayer player = game.getPlayerById(playerId);
		PtrCard card = player->getCard(cardId);
		card->setTypeId(cardTypeId);
	}
	break;

	case Message::MessageType::SV_CARD_TO_TABLE :
	{
		int playerId = msg.iData[Message::Key::svCardToTable::PLAYER_ID];
		int cardId = msg.iData[Message::Key::svCardToTable::CARD_ID];
		Game& game = getGameEngine()->getGame();
		PtrPlayer player = game.getPlayerById(playerId);

		if(player)
		{
			int playerPos = game.getPlayerPosition(playerId);
			Point<double> tablePos = myView->getGivenPosition("4p_p" + tools::numToString(playerPos) + "Table");

			PtrCard card = player->getCard(cardId);
			if(card)
			{
				game.getTable().addCard(card);
				card->setState(boost::shared_ptr<EMoveToTable>(new EMoveToTable(tablePos)));
				card->setRotation(0.f);		
			}
		}
	}
	break;

	case Message::MessageType::CARD_ARRIVED_ON_TABLE:
	{
		if(!getNetworkEngine()->isMulti() || getNetworkEngine()->isServer())
		{
			Game& game = getGameEngine()->getGame();
			if(game.getTable().getCards().size() == Rules::getInstance()->getNumberCards<Table>())
			{
				//Point<double> cardTrickPos = myView->getGivenPosition("trickSet");
				//game.endTrick(cardTrickPos);
				//notify client	
				TimerFunction timerFunction(boost::bind(&SGame::endTrick, this), 2500);
				myTimerFunctions.push_back(timerFunction);
			}
			else
			{
				game.nextPlayerToPlay(); 
				Message msg = MessageBuilder::svPlayerToPlay(game.getPlayerIdToPlay());
				sendMessageTCP(msg, false);

				myIsTransition = false;
			}
		}		
	}
	break;

	case Message::MessageType::SV_END_TRICK:
	{
		//int firstPosition = msg.iData[Message::Key::svEndTrick::FIRST_POSITION];
		Game& game = getGameEngine()->getGame();
		//const Contract& contract = game.getContract();
		//int firstPosition = game.getPlayerPosition(game.getFirstIdToPlay());
		//PtrCard holdingCard;

		//PtrPlayer player = Rules::getInstance()->getTrickHolder(game.getTable(), game, firstPosition, contract, holdingCard);
		//game.setFirstIdToPlay(player->getId());	

		Point<double> cardTrickPos = myView->getGivenPosition("trickSet");
		game.endTrick(game.getPlayerPosition(game.getFirstIdToPlay()), cardTrickPos);
	}
	break;

	default:
		break;
	}
}

void SGame::endTrick()
{
	Game& game = getGameEngine()->getGame();
	//const Contract& contract = game.getContract();
	//int firstPosition = game.getPlayerPosition(game.getFirstIdToPlay());
	//PtrCard holdingCard;

	//PtrPlayer player = Rules::getInstance()->getTrickHolder(game.getTable(), game, firstPosition, contract, holdingCard);
	//game.setFirstIdToPlay(player->getId());	
	
	Message msg; 
	msg.type = Message::MessageType::SV_END_TRICK;
	sendMessageTCP(msg, true);
	//game.nextPlayerToPlay(); 

	if(game.playersHaveNoCards())
	{
		//Compute points
		game.computePoints();
	}

	msg = MessageBuilder::svFirstPlayerToPlay(game.getFirstIdToPlay());
	sendMessageTCP(msg, false);

	myIsTransition = false;
}

//void SGame::draw()
//{
//	Game& game = getGameEngine()->getGame();
//
//	PtrResourceTexture fond = TextureManager::getInstance()->getResource(2);
//
//	float x = 0, y = 0;
//	sf::Sprite spFond;
//	spFond.SetTexture(*fond->getTexture());
//	spFond.SetPosition(0, 0);
//	spFond.SetScale(1.f, 1.f);
//	getGameEngine()->getGraphicEngine()->draw(spFond);
//	
//	int number = game.getNumberPlayers();
//	for(int i = 0; i < number; i++)
//	{
//		x = 0;
//
//		Player* player = game.getPlayer(i);
//		const std::vector<PtrCard>& cards = player->getCards();
//
//		//std::cout << (static_cast<CardHandler<Player>* >(player)) << std::endl;
//
//		for(unsigned int j = 0; j < cards.size(); j++)
//		{
//			PtrResourceImage resource = cards[j]->getImage();
//			PtrTexture texture = resource->getResourceTexture()->getTexture();
//			sf::Sprite sp;
//			sp.SetTexture(*texture);
//			IntRect rect = resource->getImage().getRect();
//			sf::IntRect sfRect(rect.x, rect.y, rect.width, rect.height);
//			sp.SetTextureRect(sfRect);
//			sp.SetPosition(x, y);
//
//			cards[j]->setPosition(Point<float>(x, y));
//			cards[j]->setSize(rect.width, rect.height);
//			if(cards[j]->isSelected())
//			{
//				sp.SetColor(sf::Color(100, 100, 200));
//			}
//
//			x += 50;
//
//			getGameEngine()->getGraphicEngine()->draw(sp);
//		}
//
//		y += 300;
//	}
//}
