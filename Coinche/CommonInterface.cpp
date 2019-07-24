#include "CommonInterface.h"
#include "Deck.h"
#include "Player.h"

#include "SGame.h"
#include "SBidding.h"
#include "SLobby.h"
#include "SMainMenu.h"

//#include "GameView.h"
#include "MessageSender.h"

#include "Tools/Logger.h"

//#include <iostream>

CommonInterface::CommonInterface(void)
{
	//utiles::creerGuiDat();

	myGame.linkGameEngine(&myGame);
	myGame.linkGraphicEngine(&myGraphic);
	myGame.linkNetworkEngine(&myNetwork);
	myGame.linkSoundEngine(&mySound);

	myNetwork.linkGameEngine(&myGame);
	myNetwork.linkGraphicEngine(&myGraphic);
	myNetwork.linkSoundEngine(&mySound);

	mySound.linkGameEngine(&myGame);
	mySound.linkGraphicEngine(&myGraphic);
	mySound.linkNetworkEngine(&myNetwork);

	myGraphic.linkGameEngine(&myGame);
	myGraphic.linkNetworkEngine(&myNetwork);
	myGraphic.linkSoundEngine(&mySound);

	MessageSender::getInstance()->linkGameEngine(&myGame);
}


CommonInterface::~CommonInterface(void)
{
}


void CommonInterface::run()
{

	//Deck d;
	////d.addCard(PtrCard(new Card(Card::_1, Card::CLUB)));
	////d.addCard(PtrCard(new Card(Card::_2, Card::CLUB)));
	////d.addCard(PtrCard(new Card(Card::_3, Card::CLUB)));
	////d.addCard(PtrCard(new Card(Card::_4, Card::CLUB)));
	////d.addCard(PtrCard(new Card(Card::_5, Card::CLUB)));
	////d.addCard(PtrCard(new Card(Card::_6, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::_7, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::_8, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::_9, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::_10, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::J, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::Q, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::K, Card::CLUB)));
	//d.addCard(PtrCard(new Card(Card::ACE, Card::CLUB)));

	////d.addCard(PtrCard(new Card(Card::_1, Card::DIAMOND)));
	////d.addCard(PtrCard(new Card(Card::_2, Card::DIAMOND)));
	////d.addCard(PtrCard(new Card(Card::_3, Card::DIAMOND)));
	////d.addCard(PtrCard(new Card(Card::_4, Card::DIAMOND)));
	////d.addCard(PtrCard(new Card(Card::_5, Card::DIAMOND)));
	////d.addCard(PtrCard(new Card(Card::_6, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::_7, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::_8, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::_9, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::_10, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::J, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::Q, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::K, Card::DIAMOND)));
	//d.addCard(PtrCard(new Card(Card::ACE, Card::DIAMOND)));

	////d.addCard(PtrCard(new Card(Card::_1, Card::SPADE)));
	////d.addCard(PtrCard(new Card(Card::_2, Card::SPADE)));
	////d.addCard(PtrCard(new Card(Card::_3, Card::SPADE)));
	////d.addCard(PtrCard(new Card(Card::_4, Card::SPADE)));
	////d.addCard(PtrCard(new Card(Card::_5, Card::SPADE)));
	////d.addCard(PtrCard(new Card(Card::_6, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::_7, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::_8, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::_9, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::_10, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::J, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::Q, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::K, Card::SPADE)));
	//d.addCard(PtrCard(new Card(Card::ACE, Card::SPADE)));

	////d.addCard(PtrCard(new Card(Card::_1, Card::HEART)));
	////d.addCard(PtrCard(new Card(Card::_2, Card::HEART)));
	////d.addCard(PtrCard(new Card(Card::_3, Card::HEART)));
	////d.addCard(PtrCard(new Card(Card::_4, Card::HEART)));
	////d.addCard(PtrCard(new Card(Card::_5, Card::HEART)));
	////d.addCard(PtrCard(new Card(Card::_6, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::_7, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::_8, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::_9, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::_10, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::J, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::Q, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::K, Card::HEART)));
	//d.addCard(PtrCard(new Card(Card::ACE, Card::HEART)));

	//std::vector<Player*> vec;
	//vec.resize(4);
	//for(unsigned int i = 0; i < vec.size(); i++)
	//{
	//	vec[i] = new Player;
	//}

	
	//Rules::getInstance()->shuffle(d);
	//Rules::getInstance()->distributeCards(d, vec);
	//
	//for(unsigned int i = 0; i < vec.size(); i++)
	//{
	//	std::cout << (static_cast<CardHandler<Player>* >(vec[i])) << std::endl;
	//}
	try
	{
		//PtrGameState sgame(new SBidding);
		//sgame->setView(PtrView(new GameView));
		PtrGameState slobby(new SLobby);
		myGame.setGameState(PtrGameState(new SMainMenu));//lobby);

		while(!myGame.isFinished())
		{
			myGame.catchEvents();//.catchEvents();

			myGame.update();
			mySound.update();
			myNetwork.update();
			myGraphic.update();

			myGraphic.draw();
		}
	}
	catch(const std::exception& e)
	{
		ILogger::log(ILogger::ERRORS) << e.what() << "\n";
		std::cout << e.what() << "lol";
		getchar();
	}
	catch(...)
	{
		ILogger::log(ILogger::ERRORS) << "Unknown exception...\n";
		std::cout << "lol";
		getchar();
	}
}