#include "SBidding.h"

#include "CardDefManager.h"

#include "Gaia/SFMLRenderer/SFMLRenderer.h"
#include "Gaia/XMLLoader/XMLGuiLoader.h"
#include "GameView.h"
#include "GameEngine.h"
#include "GraphicEngine.h"
#include "NetworkEngine.h"
#include "Game.h"
#include "MessageBuilder.h"
#include "EMoveToHand.h"
#include "EFlipCard.h"
#include "SGame.h"
#include "ResourceImageManager.h"
#include "tools/FileReading.h"
#include "tools/Tools.h"

#include <sstream>


SBidding::SBidding(void)
{
}


SBidding::~SBidding(void)
{
}

void SBidding::init()
{	
	Game& game = getGameEngine()->getGame();

	setView(PtrView(new GameView("data/gameViewPos.txt")));
	
	affectPlayerPositionToSouth();

	game.setContract(Contract());

	//gaia::GuiManager::getInstance()->clean();
	gaia::xml::XMLGuiLoader::loadGUI("data/gui/guiBidding.xml");

	if(getNetworkEngine()->isServer())
	{
		Message msg;
		msg.type = Message::MessageType::SV_GO_SBIDDING;
		sendMessageTCP(msg, false);
	}
	//else//(!getNetworkEngine()->isServer())
	//{
	//	Message msg;
	//	msg.type = Message::MessageType::CL_SBIDDING_READY;
	//	sendMessageTCP(msg);
	//}
	gaia::PtrComboBox valCombo = gaia::GuiManager::getInstance()->getWidget<gaia::ComboBox>("valueCombo");
	valCombo->addItem("80");
	valCombo->addItem("90");
	valCombo->addItem("100");
	valCombo->addItem("110");
	valCombo->addItem("120");
	valCombo->addItem("130");
	valCombo->addItem("140");
	valCombo->addItem("150");
	valCombo->addItem("160");
	//valCombo->addItem("170");
	valCombo->addItem("Capot");
	valCombo->addItem("Générale");
	valCombo->setNumberElementsDisplayed(5);
	valCombo->setTextColor(gaia::Color(150, 120, 80));

	gaia::ListBoxSimpleRenderer* renderer = (gaia::ListBoxSimpleRenderer*) valCombo->getListBox()->getWidgetRenderer();
	renderer->setBackgroundColor(gaia::Color(0, 0, 0));

	valCombo->setTextColor(gaia::Color(250, 250, 250));
	gaia::PtrComboBox colCombo = gaia::GuiManager::getInstance()->getWidget<gaia::ComboBox>("colorCombo");
	
	colCombo->addItem("Pique");
	colCombo->addItem("Coeur");
	colCombo->addItem("Carreau");
	colCombo->addItem("Trèfle");
	colCombo->addItem("Sans atout");
	colCombo->addItem("Tout atout");

	//gaia::PtrWindow window = gaia::GuiManager::getInstance()->getWidget<gaia::Window>("bidWindow");
	//window->
	//colCombo->setTextColor(gaia::Color(150, 120, 80));

	if(!getNetworkEngine()->isMulti() || getNetworkEngine()->isServer())
	{

		game.setPlayerIdToPlay(game.getFirstIdToPlay());
		game.nextPlayerToPlay();

		game.createDeck();

				Deck& deck = game.getDeck();
			std::vector<PtrCard>& cards = deck.getCards();
			std::ostringstream oss;
			for(unsigned int i = 0; i < cards.size(); i++)
			{
				oss << cards[i]->getId() << " " << cards[i]->getTypeId();
				if(i != cards.size() - 1) 
					oss << " ";
			}

			Message msg = MessageBuilder::svCreateDeck(cards.size(), oss.str());
			sendMessageTCP(msg, false);

			game.distributeCards();

			//oss.str("");
			//std::vector<std::ostringstream> tmp;
			//tmp.resize(game.getNumberPlayers());

			//std::vector<std::ostringstream> fakeTmp;
			//fakeTmp.resize(game.getNumberPlayers());

			//for(int i = 0; i < game.getNumberPlayers(); i++)
			//{
			//	PtrPlayer player = game.getPlayerByPosition(i);
			//	std::vector<PtrCard>& cards = player->getCards();

			//	int fakeCardId = CardDefManager::getInstance()->getFakeCardTypeId();

			//	for(int j = 0; j < cards.size(); j++)
			//	{
			//		tmp[i] << cards[j]->getId() << " " << cards[j]->getTypeId() << " ";
			//		fakeTmp[i] << cards[j]->getId() << " "<< fakeCardId << " ";
			//	}
			//}

			//for(int i = 0; i < game.getNumberPlayers(); i++)
			//{	
			//	for(int j = 0; j < game.getNumberPlayers(); j++)
			//	{
			//		PtrPlayer player = game.getPlayerByPosition(j);
			//		oss << player->getId() << " "; 

			//		if(i == j)
			//			oss << tmp[j].str();
			//		else
			//			oss << fakeTmp[j].str();
			//	}

			//	Message msg = MessageBuilder::svPlayerCards(game.getNumberPlayers(), game.getPlayerByPosition(0)->getCards().size(), oss.str());
			//	sendMessageTCP(msg, game.getPlayerByPosition(i)->getId());
			//	oss.str("");
			//}

			for(int i = 0; i < game.getNumberPlayers(); i++)
			{
				//affects the cards to the player (true cards for him + fake cards for others)
				Message msg = MessageBuilder::svPlayerCards(i, game);
				sendMessageTCP(msg, game.getPlayerByPosition(i)->getId());
			}

			Message message;
			message.type = Message::MessageType::SV_PLAYER_DISTRIBUTE_CARDS;
			sendMessageTCP(message);
	}
	//if(!getNetworkEngine()->isMulti() || (getNetworkEngine()->isMulti() && getNetworkEngine()->isServer()))
	//{
	//	game.distributeCards();

	//	if(getNetworkEngine()->isServer())
	//	{
	//		//sendPlayerCards();
	//		for(int i = 0; i < game.getNumberPlayers(); i++)
	//		{
	//		}
	//	}
	//}

	//for(int i = 0; i < game.getNumberPlayers(); i++)
	//{
	//	PtrPlayer player = game.getPlayerByPosition(i);
	//	std::vector<PtrCard>& cards = player->getCards();
	//	Point<double> center = myView->getGivenPosition("distributeCenter");

	//	for(unsigned int j = 0; j < cards.size(); j++)
	//	{
	//		cards[j]->setPosition(center);
	//	}
	//}


	
}

void SBidding::update()
{
	processQueue();

	Game& game = getGameEngine()->getGame();
	game.update(getGraphicEngine()->getElapsedTime());

	gaia::GuiManager::getInstance()->update(getGraphicEngine()->getElapsedTime());
	//for(int i = 0; i < game.getNumberPlayers(); i++)
	//{
	//	PtrPlayer player = game.getPlayerByPosition(i);
	//	player->update(game, getGraphicEngine()->getElapsedTime());
	//}
}

bool SBidding::catchEvent(const sf::Event& ev)
{
	Game& game = getGameEngine()->getGame();
	EntityManager entityManager = game.getEntityManager();

	if(!gaia::GuiManager::getInstance()->processEvent(gaia::SFMLInput(ev)))
	{
		if(ev.Type == sf::Event::KeyPressed)
		{
			if(ev.Key.Code == sf::Keyboard::N)
			{
				setGameState(PtrGameState(new SGame));
			}
		}
	}
	
	//if(ev.Type == sf::Event::MouseMoved)
	//{
	//	std::vector<PtrGraphicEntity> allEntities = entityManager.getEntities();
	//	for(unsigned int i = 0; i < allEntities.size(); i++)
	//	{
	//		allEntities[i]->setSelected(false);
	//	}

	//	std::vector<PtrGraphicEntity> entities;
	//	for(int i = 0; i < game.getNumberPlayers(); i++)
	//	{
	//		PtrPlayer player = game.getPlayerByPosition(i);

	//		std::vector<PtrGraphicEntity> tmp = player->getHoveredEntities(
	//				Point<double>(ev.MouseMove.X / (double)getGraphicEngine()->getWindowSizeX(),
	//							  ev.MouseMove.Y / (double)getGraphicEngine()->getWindowSizeY()),
	//				Point<int>(getGraphicEngine()->getWindowSizeX(),
	//						   getGraphicEngine()->getWindowSizeY())
	//		);
	//		entities.insert(entities.end(), tmp.begin(), tmp.end());
	//	}

	//	if(entities.size() != 0)
	//		entities.back()->setSelected(true);

	//	//if(entities.size() != 0)
	//	//	entities.back()->setSelected(true);

	//	//std::vector<PtrGraphicEntity> entities = 
	//	//	entityManager.getHoveredEntities(
	//	//		Point<double>(ev.MouseMove.X / getGraphicEngine()->getWindowSizeX(),
	//	//					  ev.MouseMove.Y / getGraphicEngine()->getWindowSizeY())); //A modif
	//	//if(entities.size() != 0)
	//	//	entities.back()->setSelected(true);
	//}

	return true;
}

void SBidding::processMessage(const Message& msg)
{
	switch(msg.type)
	{
	case Message::MessageType::CL_CARD_MOVED_TO_HAND:
	{
		int id = msg.iData[Message::Key::ClCardMovedToHand::CARD_ID];
		Game& game = this->getGameEngine()->getGame();
		for(int i = 0; i < game.getNumberPlayers(); i++)
		{
			PtrPlayer player = game.getPlayerByPosition(i);
			//if(player->getId() == game.getPlayerId())
			{
				
				std::vector<PtrCard>& cards = player->getCards();
				for(int j = 0; j < cards.size(); j++)
				{
					if(cards[j]->getId() == id)
					{
						cards[j]->setState(boost::shared_ptr<EFlipCard>(new EFlipCard));
						break;
					}
				}
			}
		}
		break;
	}
	case Message::MessageType::CL_SBIDDING_READY:
	{
		int id = msg.iData[0];

		Game& game = getGameEngine()->getGame();
		GameServer server = game.getServerData();
		server.addPlayerReady(id);

		if(server.allPlayersReady())
		{
			//myPlayersReady.push_back(id);

		//if(allPlayersReady())
		//if(myPlayersReady.size() == game.getNumberHumanPlayers())
		
			Deck& deck = game.getDeck();
			std::vector<PtrCard>& cards = deck.getCards();
			std::ostringstream oss;
			for(unsigned int i = 0; i < cards.size(); i++)
			{
				oss << cards[i]->getId() << " " << cards[i]->getTypeId();
				if(i != cards.size() - 1) 
					oss << " ";
			}

			Message msg = MessageBuilder::svCreateDeck(cards.size(), oss.str());
			sendMessageTCP(msg);

			game.distributeCards();

			for(int i = 0; i < game.getNumberPlayers(); i++)
			{
				//affects cards to the player (true cards + fake cards from others)
				Message msg = MessageBuilder::svPlayerCards(i, game);
				sendMessageTCP(msg, game.getPlayerByPosition(i)->getId());
			}
		}
		break;
	}

	case Message::MessageType::SV_CREATE_DECK:
	{
		int nbCards = msg.iData[Message::Key::SvCreateDeck::NB_CARDS];
		std::string data = msg.sData[Message::Key::SvCreateDeck::DATA];
		std::istringstream stream(data);


		Game& game = getGameEngine()->getGame();
		Deck& deck = game.getDeck();

		game.createDeck(nbCards, data);
		
		break;
	}

	case Message::MessageType::SV_PLAYER_CARDS:
	{
		int nbCards = msg.iData[Message::Key::SvPlayerCards::NB_CARDS];
		int nbPlayers = msg.iData[Message::Key::SvPlayerCards::NB_PLAYERS];
		std::string data = msg.sData[Message::Key::SvPlayerCards::DATA];

		Game& game = getGameEngine()->getGame();
		
		//game.distributeCards(); //fake function
		 
		std::istringstream stream(data);

		for(int i = 0; i < nbPlayers; i++)
		{
			int playerId;
			stream >> playerId;
			
			PtrPlayer player = game.getPlayerById(playerId);
			std::vector<PtrCard>& cards = player->getCards();
			for(int j = 0; j < nbCards; j++)
			{
				int cardId, typeId;

				stream >> cardId >> typeId;

				Deck& deck = game.getDeck();
				PtrCard card = deck.getCard(cardId);
				if(card)
				{
					card->setTypeId(typeId);
					player->addCard(card);
				}
				else
				{
					ILogger::log(ILogger::ERRORS) << "Can't give card " << cardId 
						<< " to player " << player->getName() << ".Card not found in the deck.\n";
				}
				
				//for(unsigned int k = 0; k < cards.size(); k++)
				//{
				//	//if(cards[k]->getId() == cardId)
				//	{
				//		cards[k]->setColor((Card::CardColor)color);
				//		cards[k]->setValue((Card::CardValue)value);
				//	}
				//}
			}
		}


	
		break;
	}

	case Message::MessageType::SV_PLAYER_DISTRIBUTE_CARDS:
	{
		Game& game = getGameEngine()->getGame();
		for(int i = 0; i < game.getNumberPlayers(); i++)
		{
			PtrPlayer player = game.getPlayerByPosition(i);
			std::vector<PtrCard>& cards = player->getCards();
			Point<double> center = myView->getGivenPosition("distributeCenter");

			for(unsigned int j = 0; j < cards.size(); j++)
			{
				cards[j]->setPosition(center);
			}
		}

		switch(game.getNumberPlayers())
		{
	case 2:
		break;
	case 3:
		break;
	case 4:
		//Demander aux Rules de renvoyer la liste des chaines utilisées ou directement les valeurs... Supprimer le switch.
		Point<double> deviationHoriz = myView->getGivenPosition("cardDeviationHoriz");
		Point<double> deviationVert = myView->getGivenPosition("cardDeviationVert");
		Point<double> cardSize = myView->getGivenPosition("cardSize");
		//Point<double> deviationVertDescend = myView->getGivenPosition("cardDeviationVert");

		//First player (The player)
		PtrPlayer player = game.getPlayerByPosition(0);
		std::vector<PtrCard> cards = player->getCards();
		Point<double> p1Left = myView->getGivenPosition("4p_p1Left");

		for(unsigned int j = 0; j < cards.size(); j++)
		{
			cards[j]->setState(boost::shared_ptr<EMoveToHand>(new EMoveToHand(p1Left + deviationHoriz * j)));
			//cards[j]->setPosition(p1Left + (deviationHoriz * j));
			cards[j]->setRelSize(cardSize.x, cardSize.y);
		}

		//Second player
		player = game.getPlayerByPosition(1);
		cards = player->getCards();
		Point<double> p2Left = myView->getGivenPosition("4p_p2Left");

		for(unsigned int j = 0; j < cards.size(); j++)
		{
			cards[j]->setState(boost::shared_ptr<EMoveToHand>(new EMoveToHand(p2Left - (deviationVert * j))));
			//cards2[j]->setPosition(p2Left - (deviationVert * j));
			cards[j]->setRelSize(cardSize.x, cardSize.y);
			cards[j]->setRotation(-90.f);
			cards[j]->setSelectable(false);
		}

		//Third player
		player = game.getPlayerByPosition(2);
		cards = player->getCards();
		Point<double> p3Left = myView->getGivenPosition("4p_p3Left");

		for(unsigned int j = 0; j < cards.size(); j++)
		{
			//actually "+" could be "-" (change the p3Left x value accordingly) //and set rotation to 180
			cards[j]->setState(boost::shared_ptr<EMoveToHand>(new EMoveToHand(p3Left + (deviationHoriz * j))));
			//cards3[j]->setPosition(p3Left + (deviationHoriz * j));
			cards[j]->setRelSize(cardSize.x, cardSize.y);
			cards[j]->setSelectable(false);
		}

		//Fourth player
		player = game.getPlayerByPosition(3);
		cards = player->getCards();
		Point<double> p4Left = myView->getGivenPosition("4p_p4Left");

		for(unsigned int j = 0; j < cards.size(); j++)
		{
			cards[j]->setState(boost::shared_ptr<EMoveToHand>(new EMoveToHand(p4Left + (deviationVert * j))));
			//cards4[j]->setPosition(p4Left + (deviationVert * j));
			cards[j]->setRelSize(cardSize.x, cardSize.y);
			cards[j]->setRotation(-90.f);
			cards[j]->setSelectable(false);
		}
	}

	}
	break;

	case Message::MessageType::SV_GO_SGAME:
	{
		setGameState(PtrGameState(new SGame));
	}

	case Message::MessageType::CL_NEW_BID:
	{
		int id = msg.iData[Message::Key::clNewBid::ID];
		int value = msg.iData[Message::Key::clNewBid::VALUE];
		int color = msg.iData[Message::Key::clNewBid::COLOR];

		Game& game = getGameEngine()->getGame();

		const Contract& contract = game.getContract();
		if(game.getPlayerIdToPlay() == id)
		{
			if(value > contract.myValue && value <= 250)
			{
				Message message = MessageBuilder::svUpdateBidCombo(value + 10);
				sendMessageTCP(message);

				game.nextPlayerToPlay();
				message = MessageBuilder::svPlayerToPlay(game.getPlayerIdToPlay());
				sendMessageTCP(message);
			}
		}

	}
	break;

	case Message::MessageType::SV_UPDATE_BID_COMBO:
	{
		gaia::PtrComboBox valCombo = gaia::GuiManager::getInstance()->getWidget<gaia::ComboBox>("valueCombo");
		//valCombo->
		std::string sValue = valCombo->getTextBox()->getText();
	}
	break;

	default:
		break;
	}
}

void SBidding::affectPlayerPositionToSouth()
{
	Game& game = getGameEngine()->getGame();
	game.affectPlayerPositionToSouth();
	//find player id
	
}

void SBidding::onValider(gaia::MouseEvent& ev)
{
	gaia::PtrComboBox valCombo = gaia::GuiManager::getInstance()->getWidget<gaia::ComboBox>("valueCombo");
	std::string sValue = valCombo->getTextBox()->getText();
	gaia::PtrComboBox colCombo = gaia::GuiManager::getInstance()->getWidget<gaia::ComboBox>("colorCombo");
	std::string sColor = colCombo->getTextBox()->getText();

	Message msg = MessageBuilder::clNewBid(tools::stringToNum<int>(sValue), tools::stringToNum<int>(sColor));
	clientRequestTCP(msg);
}

void SBidding::onPasser(gaia::MouseEvent& ev)
{
	Message msg = MessageBuilder::clNewBid(0, 0);
	clientRequestTCP(msg);
}

//Distribuer les cartes
//Etat ReceptionCartes, initialisé avec le num de la carte. Demander au Gfx la position centrale
//Déplacer la carte du centre vers la main du joueur
//Envoyer un message quand c'est fini avec le num de la carte
//Si c'est la dernière carte, changer l'état pour RetournerCarte



//Problème du client sur le proc serveur qui ne peut lancer de demandes
//Problème du nombre de joueurs / IA