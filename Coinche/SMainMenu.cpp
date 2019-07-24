#include "SMainMenu.h"
#include "GraphicEngine.h"

#include "HoverableTextRenderer.h"
#include "MenuView.h"
#include "SPreBidding.h"
#include "SLobby.h"
#include "NetworkEngine.h"
#include "GameEngine.h"

SMainMenu::SMainMenu(void)
{
	
}


SMainMenu::~SMainMenu(void)
{
}

void SMainMenu::update()
{
	gaia::GuiManager::getInstance()->update(getGraphicEngine()->getElapsedTime());
}

void SMainMenu::processMessage(const Message& msg)
{
	//switch(msg.type)
	//{
	//case Message::MessageType::SV_CONNECTION_RESULT:
	//{
	//	if(msg.iData[Message::Key::SvConnectionResult::RESULT] == 1)
	//	{
	//		std::string name = "Yoo";
	//		//sendMessageTCP(MessageBuilder::ClConnection(name, false));//, false);
	//		clientRequestTCP(MessageBuilder::ClConnection(name, false));
	//		ILogger::log() << "CL : player " << name << " connected.\n";
	//	}
	//	else
	//	{
	//		//disconnect
	//	}
	//}
	//break;

	//default:
	//	break;
	//}
}

void SMainMenu::init()
{
	setView(PtrView(new MenuView("")));
	gaia::xml::XMLGuiLoader::loadGUI("./data/gui/guiAccueil.xml");
	
	//gaia::GuiManager::getInstance()->createWidget("Label", "label");
	gaia::PtrLabel label = gaia::GuiManager::getInstance()->getWidget<gaia::Label>("Solo");
	label->setWidgetRenderer(new HoverableTextRenderer);
	label->subscribeMouseReleased(gaia::my_bind(&SMainMenu::onClicSolo, this));

	label = gaia::GuiManager::getInstance()->getWidget<gaia::Label>("Multi");
	label->setWidgetRenderer(new HoverableTextRenderer);

	label = gaia::GuiManager::getInstance()->getWidget<gaia::Label>("Options");
	label->setWidgetRenderer(new HoverableTextRenderer);
	//gaia::PtrLabel label(new gaia::Label("ahhdsk", 0,0,100,100));
	//label->setText("Coincoinche");
	//label->setTextColor(gaia::Color(150,150,10));
	//label->setFontSize(90);
	//label->setFont(gaia::PtrFont(new gaia::SFMLFont("./data/fonts/PORKYS_.ttf")));
	//gaia::IWidgetRenderer* wr = new gaia::LabelRenderer;
	//wr->addImage("", gaia::Image());
	//label->setWidgetRenderer(new gaia::LabelRenderer);
	//gaia::GuiManager::getInstance()->getRootWidget()->add(label);
}

bool SMainMenu::catchEvent(const sf::Event& ev)
{
	if(!gaia::GuiManager::getInstance()->processEvent(gaia::SFMLInput(ev)))
	{
	}

	
	//switch(ev.Type)
	//{
	//case sf::Event::KeyPressed:
	//	if(ev.Key.Code == sf::Keyboard::S)
	//	{
	//		getNetworkEngine()->listen(8205);
	//		sf::Sleep(500);
	//		sendMessageToGame(MessageBuilder::ClConnection("Moi", true));
	//		//sendMessageToNetwork(MessageBuilder::ClConnection("Moi"));
	//	}
	//	else if(ev.Key.Code == sf::Keyboard::C)
	//	{
	//		sendMessageToNetwork(MessageBuilder::ClConnection("Yoo", false));
	//	}
	//	else if(ev.Key.Code == sf::Keyboard::N)
	//	{
	//		Game& game = getGameEngine()->getGame();
	//		
	//		if(getNetworkEngine()->isServer())
	//		{
	//			PtrPlayer player = game.getPlayerByPosition(0);
	//			changeTeam(player, 1);
	//			
	//			//player = addIA(game.createNewPlayerId());
	//			//player = game.getPlayerByPosition(1);
	//			//changeTeam(player, 2);

	//			player = addIA();
	//			player = addIA();

	//			player = game.getPlayerByPosition(1);
	//			changeTeam(player, 2);

	//			player = game.getPlayerByPosition(2);
	//			changeTeam(player, 1);

	//			//player = addIA(game.createNewPlayerId());
	//			
	//			//changeTeam(player, 2);

	//			if(game.getNumberPlayers() == 3)
	//			{
	//				//player = addIA(game.createNewPlayerId());
	//				player = addIA();
	//			
	//			}

	//			player = game.getPlayerByPosition(3);
	//			changeTeam(player, 2);

	//		setGameState(PtrGameState(new SPreBidding));
	//		}

	//		//MessageBuilder
	//	}
	//break;
	//}

	return true;
}


void SMainMenu::onClicSolo(gaia::MouseEvent& ev)
{
	//getNetworkEngine()->listen(8205);
	//sf::Sleep(500);
	//sendMessageToGame(MessageBuilder::ClConnection("Moi", true));

	//Game& game = getGameEngine()->getGame();
	//		
	//if(getNetworkEngine()->isServer())
	//{
	//	PtrPlayer player = game.getPlayerByPosition(0);
	//	Message msg = MessageBuilder::svChangeTeam(player->getId(), 1);
	//	sendMessageTCP(msg, true);


	//	player = addIA();
	//	player = addIA();

	//	player = game.getPlayerByPosition(1);
	//	Message msg = MessageBuilder::svChangeTeam(player->getId(), 2);
	//	sendMessageTCP(msg, true);

	//	player = game.getPlayerByPosition(2);
	//	Message msg = MessageBuilder::svChangeTeam(player->getId(), 1);
	//	sendMessageTCP(msg, true);

	//	//player = addIA(game.createNewPlayerId());

	//	//changeTeam(player, 2);

	//		player = addIA();

	//	
	//	player = game.getPlayerByPosition(3);
	//	Message msg = MessageBuilder::svChangeTeam(player->getId(), 2);
	//	sendMessageTCP(msg, true);

	Game& game = getGameEngine()->getGame();

	PtrPlayer player = game.addPlayer(); //addHumanPlayer
	player->setName("Player");
	game.setPlayerId(player->getId());
	game.addPlayerTeam(1, player->getId());

	player = game.addIA();
	game.addPlayerTeam(2, player->getId());
	
	player = game.addIA();
	game.addPlayerTeam(1, player->getId());

	player = game.addIA();
	game.addPlayerTeam(2, player->getId());

	game.setDealerId(game.getPlayerByPosition(tools::random(0, game.getNumberPlayers() - 1))->getId());

	setGameState(PtrGameState(new SPreBidding));

	gaia::GuiManager::getInstance()->cleanDelayed();
	//setGameState(PtrGameState(new SLobby));//new SPreBidding));
	//}

}
