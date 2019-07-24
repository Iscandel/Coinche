#include "Game.h"
#include "CardDefManager.h"
#include "EEmptyIA.h"
#include "tools/FileReading.h"
#include "tools/Logger.h"
#include "ResourceImageManager.h"
#include "Rules.h"
#include "TextureManager.h"
#include "ResourceImageManager.h"
#include "ResourcesFile.h"


Game::Game(void)
:myPlayerId(-1)
,myNumberHumanPlayers(0)
,myDealerId(0)
,myPlayerIdToPlay(0)
//,myTricks(0)
{
	//tmp
	Rules::getInstance()->setNumberOfPlayers(4);
	Rules::getInstance()->setRandomShuffle(true);

	ResourcesFile::getInstance()->read("./data/TexturesDat.dat", "TexturesDat");

	TextureManager::getInstance()->preload();
	ResourceImageManager::getInstance()->preload();
	CardDefManager::getInstance()->preload("data/cards.txt");

	//TextFile file("data/cards.txt");
	//int nbElements = 0, color, value, imageId;

	//file >> nbElements;
	//for(int i = 0; i < nbElements; i++)
	//{
	//	file >> color >> value >> imageId;
	//	PtrCard card(new Card(i, (Card::CardValue)value, (Card::CardColor) color));
	//	card->setImage("imageFace", ResourceImageManager::getInstance()->getResource(imageId));

	//	//A modif. Ok
	//	card->setImage("imageBack", ResourceImageManager::getInstance()->getResource(55));

	//	myEntityManager.registerEntity(card);
	//	myDeck.addCard(card);
	//}

	//myPlayers.resize(4);
	//for(unsigned int i = 0; i < myPlayers.size(); i++)
	//{
	//	myPlayers[i] = PtrPlayer(new Player(i));
	//}

	//distributeCards();
	
}

Game::~Game(void)
{
}

void Game::createDeck()
{
	resetDeck();

	if(getNumberPlayers() == 4)
	{
		TextFile file("data/deck4p.txt");
		int nbElements = 0, cardId, color, value, imageId;

		file >> nbElements;
		for(int i = 0; i < nbElements; i++)
		{
			file >> cardId;
			
			//Should draw a random value for id. id array. draw a random value from it.
			//createCard(i, cardId);
			createCard(myEntityManager.createNewId(), cardId);
					   

			////Should draw a random value for id. id array. draw a random value from it.
			//PtrCard card(new Card(i, (Card::CardValue)def->myValue, (Card::CardColor) def->myColor));
			//card->setImage("imageFace", ResourceImageManager::getInstance()->getResource(def->myImageId));

			//card->setImage("imageBack", ResourceImageManager::getInstance()->getResource(def->myImageIdBack));

			//myEntityManager.registerEntity(card);
			//myDeck.addCard(card);
		}
	}
	else
	{
		ILogger::log() << "Deck couldn't be created";
	}
}

void Game::createDeck(int nbCards, const std::string& data)
{
	resetDeck();

	std::istringstream stream(data);
	for(int i = 0; i < nbCards; i++)
	{
		int id, cardTypeId;
		stream >> id >> cardTypeId;
		
		createCard(id, cardTypeId);
	}
}

void Game::createCard(int id, int typeId)
{
	//CardDefManager::PtrResource def = CardDefManager::getInstance()->getResource(cardId);
	//	if(def)
	//	{
	//(Card::CardValue)def->myValue, 
	//				   (Card::CardColor) def->myColor, 
	//				   def->myImageId, 
	//				   def->myImageIdBack);
	//PtrCard card(new Card(id, value, color));
	//card->setImage("imageFace", ResourceImageManager::getInstance()->getResource(imageId));
	//card->setImage("imageBack", ResourceImageManager::getInstance()->getResource(ImageIdBack));

	PtrCard card(new Card(id, typeId));
	myEntityManager.registerEntity(card);
	myDeck.addCard(card);
}

void Game::resetDeck()
{
	std::vector<PtrCard> cards = myDeck.getCards();
	for(unsigned int i = 0; i < cards.size(); i++)
	{
		int id = cards[i]->getId();
		myEntityManager.removeEntity(id);
	}

	myDeck.removeCards();
}

void Game::distributeCards()
{
	Rules::getInstance()->shuffle(myDeck);
	Rules::getInstance()->distributeCards(myDeck, myPlayers);

	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		myPlayers[i]->sortCards();
	}
}

void Game::update(unsigned int elapsedTime)
{
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		myPlayers[i]->update(*this, elapsedTime);
	}

	std::map<int, PtrTeam>::iterator it = myTeams.begin();
	for(it; it != myTeams.end(); ++it)
	{
		if(it->second->mustBeDestroyed())
			it = myTeams.erase(it);
	}

	myTable.update(*this, elapsedTime);
}

void Game::affectPlayerPositionToSouth()
{
	int playerPos = myPlayers.size();
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == getPlayerId())
		{
			playerPos = i;
		}
	}


	if(playerPos == myPlayers.size())
	{
		ILogger::log() << "Unable to shift player position. Unknown reason !\n";
		return;
	}
	if(playerPos == 0)
		return;

	//Copy players to the player pos
	std::vector<PtrPlayer> subVector(myPlayers.begin(), myPlayers.begin() + playerPos);
	//Erase them
	myPlayers.erase(myPlayers.begin(), myPlayers.begin() + playerPos);

	myPlayers.insert(myPlayers.end(), subVector.begin(), subVector.end());
}

PtrPlayer Game::addPlayer(PtrPlayer player)
{
	player->setGame(this);
	myPlayers.push_back(player);
	myGameServerData.addNotLocalId(player->getId());

	return player;
}

PtrPlayer Game::addPlayer()
{
	PtrPlayer player(new Player(myEntityManager.createNewId(), this));
	player->setGame(this);
	myPlayers.push_back(player);
	myGameServerData.addNotLocalId(player->getId());

	return player;
}

PtrPlayer Game::addIA(PtrPlayer ia)
{
	ia->setGame(this);
	myPlayers.push_back(ia);
	myGameServerData.addIAId(ia->getId());
	if(ia->getName() == "")
		ia->setName(generateRandomName());
	//myGameServerData.addLocalId(ia->getId());

	return ia;
}

PtrPlayer Game::addIA()
{
	PtrPlayer ia(new Player(myEntityManager.createNewId(), this));
	ia->setState(boost::shared_ptr<EEmptyIA>(new EEmptyIA));
	//generate random name
	std::string name = generateRandomName();
	ia->setName(name);
	myPlayers.push_back(ia);
	myGameServerData.addIAId(ia->getId());
	//myGameServerData.addLocalId(ia->getId());

	return ia;
}

std::string Game::generateRandomName()
{
	static std::string names[] = 
	{
		"John",
		"Ron",
		"Elsa",
		"Gaby",
		"Stephen",
		"Victor",
		"Peter"
	};

	return names[tools::random(0, 6)];
}

void Game::setPlayerId(int id)
{
	myPlayerId = id;
	myGameServerData.removeId(id);
	myGameServerData.addLocalId(id);
}

PtrPlayer Game::getPlayerById(int id)
{
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == id)
			return myPlayers[i];
	}
	return PtrPlayer();
}

void Game::removePlayer(int id)
{
	if(id == myPlayerId)
		myPlayerId = -1;

	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == id)
		{
			myPlayers.erase(myPlayers.begin() + i);
			return;
		}
	}
	myGameServerData.removeId(id);
}

void Game::removePlayerByPosition(int position)
{
	assert(position < myPlayers.size());
	myPlayers.erase(myPlayers.begin() + position);
}

//int Game::createNewPlayerId()
//{
//	static int id = 0;
//
//	return id++;
//}

void Game::setDealerId(int id)
{
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == id)
		{
			myDealerId = id;
			
			//hack
			setFirstIdToPlay(id);

			nextPlayerToPlay();
			setFirstIdToPlay(getPlayerIdToPlay());
			
			return;
		}
	}

	ILogger::log() << "No existing player with id " << id << 
		" to set the current dealer.\n";
}

void Game::setPlayerIdToPlay(int id)
{
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == id)
		{
			myPlayerIdToPlay = id;
			return;
		}
	}

	throw std::runtime_error("No existing player with id " + tools::numToString(id) +
		" to set the current player to play.\n");
}

void Game::setFirstIdToPlay(int id) 
{
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == id)
		{
			myFirstIdToPlay = id; 
			myPlayerIdToPlay = id;
			return;
		}
	}

	throw std::runtime_error("No existing player with id " + tools::numToString(id) +
		" to set the current player to play.\n");
}

int Game::getPlayerPosition(int id) const
{
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == id)
		{
			return i;
		}
	}

	throw std::runtime_error("No player found for id " + tools::numToString(id) + "\n");
}

void Game::nextPlayerToPlay()
{
	for(unsigned int i = 0; i < myPlayers.size(); i++)
	{
		if(myPlayers[i]->getId() == myPlayerIdToPlay)
		{
			if(i == getNumberPlayers() - 1)
			{
				myPlayerIdToPlay = myPlayers[0]->getId();
			}
			else
			{
				myPlayerIdToPlay = myPlayers[i+1]->getId();
			}

			return;
		}
	}
}

void Game::endTrick(int firstPosition, Point<double>& cardTrickPos)
{	
	//int firstPosition = getPlayerPosition(getFirstIdToPlay());
	PtrCard holdingCard;

	PtrPlayer player = Rules::getInstance()->getTrickHolder(getTable(), *this, firstPosition, myContract, holdingCard);
	boost::shared_ptr<Trick> trick(new Trick(firstPosition, player->getId()));

	//no reference to avoid side effects...
	std::vector<PtrCard> cards = myTable.getCards();

	for(unsigned int i = 0; i < cards.size(); i++)
	{
		cards[i]->setPosition(cardTrickPos);
		cards[i]->setCurrentDrawnImage("imageBack");
		cards[i]->setRotation(90.f);
		trick->addCard(cards[i]);
	}


	setFirstIdToPlay(player->getId());	

	addTrick(trick);
}

void Game::addTrick(boost::shared_ptr<Trick> trick)
{
	myTricks.addTrick(trick);
}

std::pair<int, std::string> Game::computePoints()
{
	//std::vector<int> points;

	const std::vector<boost::shared_ptr<Trick>>& tricks = myTricks.getTricks();
	////id, teamId
	//std::map<int, PtrTeam> tmpCalculation;
	//for(unsigned int i = 0; i < myPlayers.size(); i++)
	//{
	//	bool found = false;
	//	std::map<int, PtrTeam>::iterator it = tmpCalculation.begin();
	//	for(; it != tmpCalculation.end(); ++it)
	//	{
	//		if(it->second->getTeamId() == myPlayers[i]->getTeam()->getTeamId())
	//		{
	//			tmpCalculation[myPlayers[i]->getId()] = it->second;
	//			found = true;
	//		}
	//	}

	//	if(found)
	//	{
	//		tmpCalculation[myPlayers[i]->getId()] = PtrTeam(new Team(myPlayers[i]->getTeam()->getTeamId()));
	//	}
	//}

	std::map<int, int> tricksPerPlayer;
	std::map<int, PtrTeam>::iterator it = myTeams.begin();
	for(; it != myTeams.end(); ++it)
	{
		//tmpCalculation[it->first] = 0;
		it->second->resetCurrentPlayScore();
	}


	for(unsigned int i = 0; i < tricks.size(); i++)
	{
		int winningId = tricks[i]->getWinningPlayerId();
	
		bool lastTrick = i == tricks.size() - 1 ? true : false;
		int points = Rules::getInstance()->sumPoints(tricks[i]->getCards(), myContract, lastTrick);

		PtrTeam team = getTeam(winningId);
		team->addCurrentPlayScore(points);

		std::map<int, int>::iterator it = tricksPerPlayer.find(winningId);
		if(it == tricksPerPlayer.end())
		{
			tricksPerPlayer[winningId] = 0;
		}
		
		tricksPerPlayer[winningId]++;
	}
	//Compter les plis de chaque joueur
	if(Rules::getInstance()->checkContractAndComputeScore(*this, myContract, tricksPerPlayer))
	{
		ILogger::log(ILogger::ALL) << "Contrat réussi ! \n";
		ILogger::log(ILogger::ALL) << myTeams[1]->getGlobalScore() << "\n";
		ILogger::log(ILogger::ALL) << myTeams[2]->getGlobalScore() << "\n";
	}
	else
	{
		ILogger::log(ILogger::ALL) << "Contrat chuté !\n";
		ILogger::log(ILogger::ALL) << myTeams[1]->getGlobalScore() << "\n";
		ILogger::log(ILogger::ALL) << myTeams[2]->getGlobalScore() << "\n";
	}

	return std::pair<int, std::string>();
}

bool Game::playersHaveNoCards()
{
	return myPlayers[0]->getCards().size() ? false : true;
}

void Game::addCurrentScorePlayToGlobal()
{
	std::map<int, PtrTeam>::iterator it = myTeams.begin();
	for(; it != myTeams.end(); ++it)
	{
		it->second->addCurrentScorePlayToGlobal();
	}
}

void Game::addPlayerTeam(int teamId, int playerId)
{
	std::map<int, PtrTeam>::iterator it = myTeams.begin();
	for(; it != myTeams.end(); ++it)
	{
		it->second->removePlayerId(playerId);
	}

	it = myTeams.find(teamId);
	if(it == myTeams.end())
	{
		myTeams[teamId].reset(new Team(teamId));
	}
	myTeams[teamId]->addPlayer(playerId);
}

PtrTeam Game::getTeam(int id)
{
	std::map<int, PtrTeam>::iterator it = myTeams.begin();
	for(; it != myTeams.end(); ++it)
	{
		if(it->second->isPlayerIdPresent(id))
		{
			return it->second;
		}
	}

	return PtrTeam();
}