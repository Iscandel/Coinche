#include "BasicIA.h"

#include "Player.h"
#include "Game.h"
#include "MessageBuilder.h"
#include "MessageSender.h"
#include "tools/Logger.h"
#include "tools/Tools.h"

BasicIA::BasicIA(void)
{
}


BasicIA::~BasicIA(void)
{
}

void BasicIA::init(Player& parent)
{
	mySleepingTime = 2000;

	const Contract& contract = parent.getGame().getContract();

	for(unsigned int i = 0; i < 4; i++)
	{
		std::map<rules::CardType, std::vector<CardPoint>> cardsOrder = Rules::getInstance()->getCardsOrder();
		if(contract.myBidType == rules::ALL_TRUMPS)
		{
			myCardsOrder[(Card::CardColor)i] = cardsOrder[rules::TRUMP];
		}
		else if(contract.myBidType == rules::NO_TRUMPS)
		{
			myCardsOrder[(Card::CardColor)i] = cardsOrder[rules::NO_TRUMP];
		}
		else
		{
			if(i == contract.myBidType)
			{
				myCardsOrder[(Card::CardColor)i] = cardsOrder[rules::TRUMP];
			}
			else
			{
				myCardsOrder[(Card::CardColor)i] = cardsOrder[rules::NORMAL];
			}
		}
	}
}

void BasicIA::free(Player& parent)
{
}

void BasicIA::update(Player& parent, const Game&, unsigned int elapsedTime)
{
	std::vector<PtrCard>& cards = parent.getCards();

	Game& game = parent.getGame();
	const Contract& contract = game.getContract();
	PtrTeam team = parent.getGame().getTeam(parent.getId());
	bool isTaker = contract.myTeamId == team->getTeamId();
	Table& table = game.getTable();

	if(game.getPlayerIdToPlay() == parent.getId() && cards.size() != 0)
	{
		mySleepingTime -= elapsedTime;
		if(mySleepingTime < 0)
		{
			mySleepingTime = 2000;

			if(table.getCards().size() == Rules::getInstance()->getNumberCards<Table>())
				return;
			
			//PtrCard card = cards[tools::random(0, cards.size() - 1)];
			//Message msg = MessageBuilder::clCardToPlay(card->getId());
			//msg.iData[0] = parent.getId();
			//MessageSender::getInstance()->sendMessageToGame(msg);

			updateRemainingCards(game, table);

			const std::vector<PtrCard> tableCards = parent.getGame().getTable().getCards();

			//special case, last card
			if(cards.size() == 1)
			{
				Message msg = MessageBuilder::clCardToPlay(cards[0]->getId());
				msg.iData[0] = parent.getId();
				MessageSender::getInstance()->sendMessageToGame(msg);
				return;
			}

			std::vector<Card::CardColor> trumpVec;
			trumpVec.push_back((Card::CardColor) contract.myBidType);

			std::vector<Card::CardColor> notTrumpVec = createNotTrumpVec(contract);
			std::vector<Card::CardColor> masterVec;

			int played = 0;

			//we play in first
			if(tableCards.size() == 0)
			{
				if(isTaker)
				{
					if(isTrumpRemaining(contract) && hasCardColor(parent, trumpVec))
					{
						if(isMaster(parent, trumpVec, masterVec)) //check contract type !!!!
						{
							played += playMasterCard(parent, trumpVec);//contract.myBidType);
						}
						else
						{
							played += playSmallCard(parent, trumpVec);
						}
					}
					else
					{
						if(isMaster(parent, notTrumpVec, masterVec)) //check contract type !!!!
						{
							played += playMasterCard(parent, masterVec);//contract.myBidType);
						}
						else
						{
							playSmallCard(parent, notTrumpVec);
						}
						//if(!tryPlayNotStrumpMasterCard(parent, contract))
						//{
						//	//play not trump small card
						//	playSmallCard(parent, contract, false);
						//}
					}
				}
				else
				{
					if(hasCardColor(parent, notTrumpVec))
					{
						if(isMaster(parent, notTrumpVec, masterVec)) //check contract type !!!!
						{
							played += playMasterCard(parent, masterVec);//contract.myBidType);
						}
						else
						{
							played += playSmallCard(parent, notTrumpVec);
						}
					}
					else
					{
						if(isMaster(parent, trumpVec, masterVec)) //check contract type !!!!
						{
							played += playMasterCard(parent, trumpVec);//contract.myBidType);
						}
						else
						{
							played += playSmallCard(parent, trumpVec);
						}
					}
					//if(!tryPlayNotStrumpMasterCard(parent, contract))
					//{
					//	//play not trump small card
					//	if(!tryPlaySmallCard(parent, contract, false))
					//		playNotMasterCard(parent, contract, true);//contract.myBidType);

					//}
				} //end isTaker
			} //end is first to play
			else
			{
				int firstPosition = game.getPlayerPosition(game.getFirstIdToPlay());
				PtrCard holdingCard;
				PtrPlayer holdingPlayer = 
					Rules::getInstance()->getTrickHolder(
						table, game, game.getPlayerPosition(game.getFirstIdToPlay()), contract, holdingCard
				);

				//trump asked
				if(tableCards[0]->getColor() == contract.myBidType)
				{
					if(Rules::getInstance()->hasCardColor(parent.getCards(), tableCards[0]->getColor()))
					{
						if(isMaster(parent, trumpVec, masterVec)) //check contract type !!!!
						{
							ILogger::log(ILogger::ALL) << "I am master at trump\n";
							played += playMasterCard(parent, trumpVec);//contract.myBidType);
						}
						else if(Rules::getInstance()->hasGreaterCards(parent.getCards(), holdingCard, contract))
						{
							ILogger::log(ILogger::ALL) << "Over trump\n";
							played += playSmallestGreaterCard(parent, holdingCard);
						}
						else
						{
							ILogger::log(ILogger::ALL) << "I play a small trump\n";
							played += playSmallCard(parent, trumpVec);
						}
					}
					else
					{
						if(team->getTeamId() == game.getTeam(holdingPlayer->getId())->getTeamId())
						{
							ILogger::log(ILogger::ALL) << "I help my partner with another color\n";
							played += playGoodCard(parent, notTrumpVec);
						}
						else
						{
							ILogger::log(ILogger::ALL) << "I play a small value from another color\n";
							played += playSmallCard(parent, tableCards[0]->getColor());
						}
					}
				}
				else
				{
					if(team->getTeamId() == game.getTeam(holdingPlayer->getId())->getTeamId())
					{
						if(Rules::getInstance()->hasCardColor(parent.getCards(), tableCards[0]->getColor()))
						{
							if(isMaster(parent, tableCards[0]->getColor()))
							{
								ILogger::log(ILogger::ALL) << "Master with " << tableCards[0]->getColor() << "\n";
								played += playMasterCard(parent, tableCards[0]->getColor()); //NB on n'est pas obligé de jouer l'as par ex si le partenaire a coupé...
							}
							else
							{
								ILogger::log(ILogger::ALL) << "I help my partner with the asked color\n";
								played += playGoodCard(parent, tableCards[0]->getColor());
							}
						}
						else if(hasCardColor(parent, notTrumpVec)) //we could save a good trump here...
						{
							ILogger::log(ILogger::ALL) << "I help my partner with another color\n";
							played += playGoodCard(parent, notTrumpVec);
						}
						else
						{
							ILogger::log(ILogger::ALL) << "I can only trump\n";
							played += playCorrectTrump(parent, table, holdingCard, trumpVec[0]); //check !!!!!!!!!!!
						}
					}
					else //partner is not holding...
					{
						if(Rules::getInstance()->hasCardColor(parent.getCards(), tableCards[0]->getColor()))
						{
							if(isMaster(parent, tableCards[0]->getColor()) && holdingCard->getColor() == tableCards[0]->getColor())
							{
								ILogger::log(ILogger::ALL) << "Master with " << tableCards[0]->getColor() << "\n";
								played += playMasterCard(parent, tableCards[0]->getColor());
							}
							else
							{
								ILogger::log(ILogger::ALL) << "I play a small value of the asked color\n";
								played += playSmallCard(parent, tableCards[0]->getColor());
							}
						}
						else
						{
							if(hasCardColor(parent, trumpVec))
							{
								ILogger::log(ILogger::ALL) << "I trump\n";
								played += playCorrectTrump(parent, table, holdingCard, trumpVec[0]); //check contract !!!!!!!!!!!!!!!
							}
							else
							{
								ILogger::log(ILogger::ALL) << "I have nothing. I play a small value \n";
								played += playSmallCard(parent, notTrumpVec);
							}
						}
					}
				} //end not trump asked
			} //end is not first to play

			if(!played)
			{
				ILogger::log(ILogger::ALL) << "On joue aléatoirement...";
				PtrCard card = cards[tools::random(0, cards.size() - 1)];
				Message msg = MessageBuilder::clCardToPlay(card->getId());
				msg.iData[0] = parent.getId();
				MessageSender::getInstance()->sendMessageToGame(msg);
			}
		} //end sleeping time ok
	} //end is our turn
}

void BasicIA::updateRemainingCards(Game& game, Table& table)
{
	const std::vector<boost::shared_ptr<Trick>>& tricks = game.getTrickSet().getTricks();

	if(tricks.size() != 0)
	{
		boost::shared_ptr<Trick> trick = tricks.back();

		const std::vector<PtrCard>& cards = trick->getCards();
		for(unsigned int i = 0; i < cards.size(); i++)
		{
			for(unsigned int j = 0; j < myCardsOrder[cards[i]->getColor()].size(); j++)
			{
				if(myCardsOrder[cards[i]->getColor()][j].myValue == cards[i]->getValue())
				{
					myCardsOrder[cards[i]->getColor()].erase(myCardsOrder[cards[i]->getColor()].begin() + j);
				}
			}
		}
	}

	const std::vector<PtrCard>& tableCards = table.getCards();

	for(unsigned int i = 0; i < tableCards.size(); i++)
	{
		for(unsigned int j = 0; j < myCardsOrder[tableCards[i]->getColor()].size(); j++)
		{
			if(myCardsOrder[tableCards[i]->getColor()][j].myValue == tableCards[i]->getValue())
			{
				myCardsOrder[tableCards[i]->getColor()].erase(myCardsOrder[tableCards[i]->getColor()].begin() + j);
			}
		}
	}
}

std::vector<Card::CardColor> BasicIA::createNotTrumpVec(const Contract& contract)
{
	std::vector<Card::CardColor> res;
	if(contract.myBidType != Card::CLUB)// && Rules::getInstance()->hasCardColor(player.getCards(), Card::CLUB))
	{
		res.push_back(Card::CLUB);
	}
	if(contract.myBidType != Card::DIAMOND)
	{
		res.push_back(Card::DIAMOND);
	}
	if(contract.myBidType != Card::HEART)
	{
		res.push_back(Card::HEART);
	}
	if(contract.myBidType != Card::SPADE)
	{
		res.push_back(Card::SPADE);
	}

	//notTrumpVec.erase(std::find(notTrumpVec.begin(),
	//							notTrumpVec.end(), 
	//							holdingCard->getColor()));

	return res;
}

bool BasicIA::isTrumpRemaining(const Contract& contract)//Player& parent, const Contract& contract)
{
	return (bool) myCardsOrder[(Card::CardColor)contract.myBidType].size();
	//switch(contract.myBidType)
	//{
	//case rules::CLUBS:
	//case rules::DIAMONDS:
	//case rules::HEARTS:
	//case rules::SPADES:
	//{
	//	std::vector<PtrCard>& cards = parent.getCards();
	//	for(unsigned int i = 0; i < cards.size(); i++)
	//	{
	//		if(cards[i]->getColor() == contract.myBidType)
	//		{
	//			return true;
	//		}
	//	}
	//}
	//break;

	//default:
	//	break;
	//}

	//return false;
}


bool BasicIA::isMaster(Player& parent, const std::vector<Card::CardColor>& colorVec, std::vector<Card::CardColor>& masterVec) 
{
	//std::vector<CardPoint> cardsOrder;

	//if(colorType == contract.myBidType || contract.myBidType == rules::ALL_TRUMPS)
	//	cardsOrder = myCardsOrder[rules::TRUMP];
	//else if(contract.myBidType == rules::NO_TRUMPS)
	//	cardsOrder = myCardsOrder[rules::NO_TRUMP];
	//else
	//	cardsOrder = myCardsOrder[rules::NORMAL];

	bool master = false;

	std::vector<PtrCard>& cards = parent.getCards();
	for(unsigned int i = 0; i < cards.size(); i++)
	{
		for(unsigned int j = 0; j < colorVec.size(); j++)
		{
			if(cards[i]->getColor() == colorVec[j])
			{
				if(myCardsOrder[colorVec[j]].size() > 0 && myCardsOrder[colorVec[j]].front().myValue == cards[i]->getValue())
				{
					masterVec.push_back(colorVec[j]);
					master = true;
				}
			}
		}
	}

	return master;
}

bool BasicIA::isMaster(Player& parent, Card::CardColor color)
{
	std::vector<Card::CardColor> colorVec;
	colorVec.push_back(color);
	std::vector<Card::CardColor> masterVec;

	return isMaster(parent, colorVec, masterVec);
}

bool BasicIA::hasCardColor(Player& parent, const std::vector<Card::CardColor>& colorVec)
{
	bool has = false;
	for(unsigned int i = 0; i < colorVec.size(); i++)
	{
		if(Rules::getInstance()->hasCardColor(parent.getCards(), colorVec[i]))
			has = true;
	}

	return has;
}
	
					
bool BasicIA::playMasterCard(Player& parent, const std::vector<Card::CardColor>& colorVec)//const Contract& contract, bool playStrump) //check contract type
{
	std::vector<PtrCard>& cards = parent.getCards();
	for(unsigned int i = 0; i < colorVec.size(); i++)
	{
		for(unsigned int j = 0; j < cards.size(); j++)
		{
			if(cards[j]->getColor() == colorVec[i])
			{
				if(myCardsOrder[colorVec[i]].front().myValue == cards[j]->getValue())
				{
					Message msg = MessageBuilder::clCardToPlay(cards[j]->getId());
					msg.iData[0] = parent.getId();
					MessageSender::getInstance()->sendMessageToGame(msg);

					return true;
				}
			}
		}
	}

	return false;
}

bool BasicIA::playMasterCard(Player& parent, Card::CardColor color)
{
	std::vector<Card::CardColor> colorVec;
	colorVec.push_back(color);

	return playMasterCard(parent, colorVec);
}

/*bool BasicIA::tryPlayNotStrumpMasterCard(Player& parent, const Contract& contract)
{
	return true;
}	*/	

bool BasicIA::playSmallestGreaterCard(Player& parent, PtrCard referenceCard)
{
	std::vector<PtrCard>& cards = parent.getCards();
	int distance = 100;
	PtrCard toPlay;

	int referenceCardPos = 100;
	for(unsigned int i = 0; i < myCardsOrder[referenceCard->getColor()].size(); i++)
	{
		if(myCardsOrder[referenceCard->getColor()][i].myValue == referenceCard->getValue())
		{
			referenceCardPos = i;
		}
	}

	for(unsigned int i = 0; i < cards.size(); i++)
	{
		if(cards[i]->getColor() == referenceCard->getColor())
		{
			for(unsigned int j = 0; j < myCardsOrder[referenceCard->getColor()].size(); j++)
			{
				if(myCardsOrder[referenceCard->getColor()][i].myValue == cards[i]->getValue())
				{
					int tmpDist = j - referenceCardPos;
					if(tmpDist > 0 && tmpDist < distance)
					{
						distance = tmpDist;
						toPlay = cards[i];
					}
				}
			}
		}
	}

	if(toPlay)
	{
		Message msg = MessageBuilder::clCardToPlay(toPlay->getId());
		msg.iData[0] = parent.getId();
		MessageSender::getInstance()->sendMessageToGame(msg);
		return true;
	}
	else
	{
		return false;
	}
}

bool BasicIA::playCorrectTrump(Player& parent, Table& table, PtrCard holdingCard, Card::CardColor color)
{
	std::vector<PtrCard>& cards = parent.getCards();
	int nbTrumps = 0;
	//int nbTrumpsBeforeMaster = 0;
	int bestCard = 100;
	PtrCard toPlay;

	for(unsigned int i = 0; i < cards.size(); i++)
	{
		if(cards[i]->getColor() == color)
		{
			nbTrumps++;
		}
	}

	for(unsigned int i = 0; i < cards.size(); i++)
	{
		if(cards[i]->getColor() == color)
		{
			for(unsigned int j = 0; j < myCardsOrder[color].size(); j++)
			{
				if(myCardsOrder[color][j].myValue == cards[i]->getValue())
				{
					if(j < bestCard)
					{
						bestCard = j;
						toPlay= cards[i];
					}
				}
			}
		}
	}

	//Ex As troisième...
	if(nbTrumps - bestCard >= 1 && 
		(holdingCard->getColor() != color || 
		(holdingCard->getColor() == color && !Rules::getInstance()->cardIsGreater(holdingCard, toPlay, parent.getGame().getContract()) )))
	{
		Message msg = MessageBuilder::clCardToPlay(toPlay->getId());
		msg.iData[0] = parent.getId();
		MessageSender::getInstance()->sendMessageToGame(msg);
		return true;
	}
	else
	{
		//Si coupé, surcoupe
		if(holdingCard->getColor() == color)
		{
			return playSmallestGreaterCard(parent, holdingCard);
		}
		else
		{
			return playSmallCard(parent, color);
		}
	}
}

//for not trump use
 bool CardsByPoints::operator ()(PtrCard first, PtrCard second)
 {
	 std::map<rules::CardType, std::vector<CardPoint>> cardsOrder = Rules::getInstance()->getCardsOrder();

	 int p1 = 0, p2 = 0;

	 for(unsigned int i = 0; i < cardsOrder[rules::NORMAL].size(); i++)
	 {
		 if(cardsOrder[rules::NORMAL][i].myValue == first->getValue())
		 {
			 p1 = cardsOrder[rules::NORMAL][i].myPoint;
		 }
		 if(cardsOrder[rules::NORMAL][i].myValue == second->getValue())
		 {
			 p2 = cardsOrder[rules::NORMAL][i].myPoint;
		 }
	 }

	 return p1 <= p2;
 }

//not trump function
bool BasicIA::playGoodCard(Player& parent, const std::vector<Card::CardColor>& colorVec)
{
	std::vector<PtrCard>& cards = parent.getCards();
	std::vector<PtrCard> potential;

	for(unsigned int i = 0; i < colorVec.size(); i++)
	{
		for(unsigned int j = 0; j < cards.size(); j++)
		{
			if(cards[j]->getColor() == colorVec[i])
			{
				potential.push_back(cards[j]);
			}
		}
	}

	//il faut trier les cartes de la meilleure à la moins bonne...
	std::sort(potential.begin(), potential.end(), CardsByPoints());

	for(int i = potential.size() - 1; i >= 0 ; i--)
	{
		if(myCardsOrder[cards[i]->getColor()].front().myValue != potential[i]->getValue() && 
			cards[i]->getValue() != Card::_10 && 
			!is10SecondNonMaster(parent, potential[i]->getColor()))
		{
			Message msg = MessageBuilder::clCardToPlay(potential[i]->getId());
			msg.iData[0] = parent.getId();
			MessageSender::getInstance()->sendMessageToGame(msg);
			return true;
		}
	}

	if(potential.size() != 0)
	{
		Message msg = MessageBuilder::clCardToPlay(potential[0]->getId());
		msg.iData[0] = parent.getId();
		MessageSender::getInstance()->sendMessageToGame(msg);
		return true;
	}
	else
	{
		return false;
	}

}

bool BasicIA::playGoodCard(Player& parent, Card::CardColor color)
{
	std::vector<Card::CardColor> colorVec;
	colorVec.push_back(color);
	return playGoodCard(parent, colorVec);
}

bool BasicIA::playSmallCard(Player& parent, const std::vector<Card::CardColor>& colorVec)
{
	int suckValue = 100;
	PtrCard toPlay;

	std::vector<PtrCard>& cards = parent.getCards();
	std::vector<PtrCard> potential;
	
	for(unsigned int i = 0; i < colorVec.size(); i++)
	{
		for(unsigned int j = 0; j < cards.size(); j++)
		{
			if(cards[j]->getColor() == colorVec[i])
			{
				for(unsigned int k = 0; k < myCardsOrder[colorVec[i]].size(); k++)
				{
					if(myCardsOrder[colorVec[i]][k].myValue == cards[j]->getValue())
					{
						//if((int)k > suckValue)
						if(myCardsOrder[colorVec[i]][k].myPoint < suckValue)
						{
							if(is10SecondNonMaster(parent, colorVec[i]))
							{
								potential.push_back(cards[j]);
							}
							else
							{
								suckValue = myCardsOrder[colorVec[i]][k].myPoint;
								//suckValue = k;
								toPlay = cards[j];
							}
						}
					}
				}
			}
		}
	}

	if(toPlay)
	{
		Message msg = MessageBuilder::clCardToPlay(toPlay->getId());
		msg.iData[0] = parent.getId();
		MessageSender::getInstance()->sendMessageToGame(msg);
		return true;
	}
	else if(potential.size() != 0)
	{
		Message msg = MessageBuilder::clCardToPlay(potential[0]->getId());
		msg.iData[0] = parent.getId();
		MessageSender::getInstance()->sendMessageToGame(msg);
		return true;
	}
	else
	{
		return false;
	}
}

bool BasicIA::playSmallCard(Player& parent, Card::CardColor color)
{
	std::vector<Card::CardColor> colorVec;
	colorVec.push_back(color);
	return playSmallCard(parent, colorVec);
}

//not trump function
bool BasicIA::is10SecondNonMaster(Player& player, Card::CardColor color)
{
	std::vector<PtrCard>& cards = player.getCards();
	int count = 0;
	bool ace = false, card10 = false;

	//ensure player has got 10 or Ace
	for(unsigned int i = 0; i < cards.size(); i++)
	{
		if(cards[i]->getColor() == color)
		{	
			if(cards[i]->getValue() == Card::ACE)
				ace = true;
			if(cards[i]->getValue() == Card::_10)
				card10 = true;

			count++;
		}
	}

	//check if player has got 10
	if(!card10)
		return false;

	//if 10 is master or player also has got Ace
	if(ace && card10)
		return false;
	
	//check if 10 is master
	if(card10 || (myCardsOrder[color].size() > 0 && (myCardsOrder[color][0].myValue == Card::_10)) )
		return false;

	return count == 2 ? true : false;
}