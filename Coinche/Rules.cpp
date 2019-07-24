#include "Rules.h"
#include "Deck.h"
#include "Game.h"
#include "Player.h"
#include "tools/Tools.h"
#include "tools/Logger.h"

#include <algorithm>

const int Rules::DECK_SIZE = 32;

Rules::Rules(void)
{
	std::vector<CardPoint> trump;
	trump.push_back(CardPoint(Card::J, 20));
	trump.push_back(CardPoint(Card::_9, 14));
	trump.push_back(CardPoint(Card::ACE, 11));
	trump.push_back(CardPoint(Card::_10, 10));
	trump.push_back(CardPoint(Card::K, 4));
	trump.push_back(CardPoint(Card::Q, 3));
	trump.push_back(CardPoint(Card::_8, 0));
	trump.push_back(CardPoint(Card::_7, 0));

	myCardsOrder[rules::TRUMP] = trump;

	std::vector<CardPoint> normal;
	normal.push_back(CardPoint(Card::ACE, 11));
	normal.push_back(CardPoint(Card::_10, 10));
	normal.push_back(CardPoint(Card::K, 4));
	normal.push_back(CardPoint(Card::Q, 3));
	normal.push_back(CardPoint(Card::J, 2));
	normal.push_back(CardPoint(Card::_9, 0));
	normal.push_back(CardPoint(Card::_8, 0));
	normal.push_back(CardPoint(Card::_7, 0));

	myCardsOrder[rules::NORMAL] = normal;

	std::vector<CardPoint> noTrump;
	noTrump.push_back(CardPoint(Card::ACE, 19));
	noTrump.push_back(CardPoint(Card::_10, 10));
	noTrump.push_back(CardPoint(Card::K, 4));
	noTrump.push_back(CardPoint(Card::Q, 3));
	noTrump.push_back(CardPoint(Card::J, 2));
	noTrump.push_back(CardPoint(Card::_9, 0));
	noTrump.push_back(CardPoint(Card::_8, 0));
	noTrump.push_back(CardPoint(Card::_7, 0));

	myCardsOrder[rules::NO_TRUMP] = noTrump;

}


Rules::~Rules(void)
{
}

std::vector<PtrCard> Rules::shuffle(Deck& d) //A revoir
{
	std::vector<PtrCard>& vec = d.getCards();
	std::vector<PtrCard> res;
	if(isRandomShuffle())
	{
		//res = vec;
		std::random_shuffle(vec.begin(), vec.end());
	}
	else
	{
		int number = tools::random((int)(res.size() / 2 - res.size() / 4), (int)(res.size() / 2 + res.size() / 4));

		res.insert(res.end(), vec.begin() + number, vec.end());
		res.insert(res.end(), vec.begin(), vec.begin() + number - 1);
		vec = res;
	}

	return res;
}

void Rules::setNumberOfPlayers(int number)
{
	myNumberOfPlayers = number;
	if(number == 2)
	{
		setNumberCardsPerPlayer(16);
		setNumberCardsPerDeck(DECK_SIZE);
	}
	else if(number == 3)
	{
		setNumberCardsPerPlayer(10);
		setNumberCardsPerDeck(30);
	}
	else
	{
		setNumberCardsPerPlayer(8);
		setNumberCardsPerDeck(DECK_SIZE);
	}
}

bool Rules::validateCardPlayed(Game& game, boost::shared_ptr<Player> player, PtrCard card)
{
	Table& table = game.getTable();
	const std::vector<PtrCard>& cards = table.getCards();

	if(cards.size() == 0)
		return true;

	bool isCardOk = false;

	const Contract& contract = game.getContract();
	PtrCard holdingCard;

	//int position = game.getPlayerPosition(player->getId());
	//int firstPosition = position - cards.size();
	//if(firstPosition < 0)
	//{
	//	firstPosition += game.getNumberPlayers();
	//}
	int firstPosition = game.getPlayerPosition(game.getFirstIdToPlay());

	//Ensure the player plays the same (asked) color if he can
	std::vector<PtrCard>& playerCards = player->getCards();
	if(card->getColor() != cards[0]->getColor() && hasCardColor(playerCards, cards[0]->getColor()))
		return false;

	//boost::shared_ptr<Player> firstPlayer = game.getPlayerByPosition(firstPosition);

	PtrPlayer holder = getTrickHolder(table, game, firstPosition, contract, holdingCard);
	ILogger::log(ILogger::ALL) << "Player " << game.getPlayerByPosition(firstPosition)->getName() << " is the first to play.\n";
	ILogger::log(ILogger::ALL) << "Player " << holder->getName() << " is the current holder with " << *holdingCard << ".\n";
	switch(contract.myBidType)
	{
	case rules::CLUBS:
	case rules::DIAMONDS:
	case rules::HEARTS:
	case rules::SPADES:
	{
		//If first player doesn't play a trump card
		if (cards[0]->getColor() != contract.myBidType) //Check that enum share the same values
		{
			//If we play the same color, that's ok
			if(card->getColor() == cards[0]->getColor())
			{
				isCardOk = true;
			}
			else //if(!hasCardColor(playerCards, cards[0]->getColor()))
			{				
				//if the player's partner holds, play what you wish
				//if(holder->getTeam()->getTeamId() == player->getTeam()->getTeamId())
				if(game.getTeam(holder->getId())->getTeamId() == game.getTeam(player->getId())->getTeamId())
				{
					isCardOk = true;
				}
				else
				{
					//check if a better trump than holder can be played
					bool hasBetterTrump = false;
					bool hasTrump = false;
					for(unsigned int i = 0; i < playerCards.size(); i++)
					{
						if(playerCards[i]->getColor() == contract.myBidType)
						{
							hasTrump = true;
							if(cardIsGreater(playerCards[i], holdingCard, contract))
							{
								hasBetterTrump = true;
							}
						}
					}

					//if a trump is played
					if(card->getColor() == contract.myBidType)
					{
						if(cardIsGreater(card, holdingCard, contract))
						{
							isCardOk = true;
						}
						else
						{
							//If the player had a better trump, he should have played it
							if(hasBetterTrump)
							{
								isCardOk = false;
							}
							else
							{
								isCardOk = true;
							}
						}
					}
					else
					{
						//If player has a trump and the holding card is not trump
						if(hasTrump)// && holdingCard->getColor() != contract.myBidType)
						{
							isCardOk = false;
						}
						else
						{
							isCardOk = true;
						}
					}
				}
			}
		}
		else // if(!hasCardColor(playerCards, cards[0]->getColor()))
		{
			if(cardIsGreater(card, holdingCard, contract))
			{
				isCardOk = true;
			}
			else
			{
				if(hasGreaterCards(playerCards, holdingCard, contract))
					isCardOk = false;
				else
					isCardOk = true;
			}
		}
	}
	break;
	default:
		break;
	}

	if(isCardOk)
		ILogger::log(ILogger::ALL) << "Card " << *card << " can be played.\n";
	else
		ILogger::log(ILogger::ALL) << "Card " << *card << " can't be played.\n";

	ILogger::log(ILogger::ALL) << "========================================\n";

	return isCardOk;
}

boost::shared_ptr<Player> Rules::getTrickHolder(Table& table, Game& game, int firstPositionToPlay, const Contract& contract, PtrCard& holdingCard)
{
	const std::vector<PtrCard>& cards = table.getCards();

	if(cards.size() == 0)
		return boost::shared_ptr<Player>();

	int currentPosition = firstPositionToPlay;
	int holdingPosition = currentPosition;

	holdingCard = cards[0];

	switch(contract.myBidType)
	{
	case rules::CLUBS:
	case rules::DIAMONDS:
	case rules::HEARTS:
	case rules::SPADES:
	{
		//if first card is strump
		if(holdingCard->getColor() == contract.myBidType)
		{
			for(unsigned int i = 1; i < cards.size(); i++)
			{
				currentPosition++;
				if(currentPosition > 3)
					currentPosition = 0;

				if(cardIsGreater(cards[i], holdingCard, contract))
				{
					holdingPosition = currentPosition;
					holdingCard = cards[i];
				}
			}

			return game.getPlayerByPosition(holdingPosition);
		}
		else
		{
			bool strumpUsed = false;
			for(unsigned int i = 1; i < cards.size(); i++)
			{
				currentPosition++;
				if(currentPosition > 3)
					currentPosition = 0;

				//if not strump
				if(cards[i]->getColor() != contract.myBidType)
				{
					if(cardIsGreater(cards[i], holdingCard, contract) && !strumpUsed)
					{
						holdingPosition = currentPosition;
						holdingCard = cards[i];
					}
				}
				else
				{
					if(!strumpUsed)
					{
						holdingPosition = currentPosition;
						holdingCard = cards[i];
						strumpUsed = true;
					}
					else
					{
						if(cardIsGreater(cards[i], holdingCard, contract))
						{
							holdingPosition = currentPosition;
							holdingCard = cards[i];
						}
					}
				}
			}

			return game.getPlayerByPosition(holdingPosition);
		}
	}
	break;
	default:
		break;
	}


}

bool Rules::cardIsGreater(PtrCard card, PtrCard reference, const Contract& contract)
{
	if(card->getColor() != reference->getColor())
		return false;
	
	std::vector<CardPoint> cards;

	switch(contract.myBidType)
	{
	case rules::CLUBS:
	case rules::DIAMONDS:
	case rules::HEARTS:
	case rules::SPADES:
	{
		if(reference->getColor() == contract.myBidType)
			cards = myCardsOrder[rules::TRUMP];
		else
			cards = myCardsOrder[rules::NORMAL];
	}
	break;
	
	case rules::ALL_TRUMPS:
	{
		cards = myCardsOrder[rules::TRUMP];
	}
	break;

	case rules::NO_TRUMPS:
	{
		cards = myCardsOrder[rules::NO_TRUMP];
	}
	break;

	default:
		break;
	}
	
	for(unsigned int i = 0; i < cards.size(); i++)
	{
		if(cards[i].myValue == card->getValue())
			return true;
		else if(cards[i].myValue == reference->getValue())
			return false;
	}

	//shouldn't arrive here
	return false;
}

bool Rules::hasCardColor(const std::vector<PtrCard>& playerCards, Card::CardColor color)
{
	for(unsigned int i = 0; i < playerCards.size(); i++)
	{
		//If the player had a card with the same color, 
		if(playerCards[i]->getColor() == color)
		{
			return true;
		}
	}

	return false;
}

bool Rules::hasGreaterCards(const std::vector<PtrCard>& playerCards, PtrCard holdingCard, const Contract& contract)
{
	//Ensure holding card is strump !!!!!!!!!!!

	for(unsigned int i = 0; i < playerCards.size(); i++)
	{
		//If the player had a card with the same color, 
		if(cardIsGreater(playerCards[i], holdingCard, contract))
		{
			return true;
		}
	}

	return false;
}

void Rules::distributeCards(Deck& cards, std::vector<PtrPlayer>& vec)//(std::vector<PtrCard>& cards, std::vector<Player*>& vec)//(
{
	if(cards.getCards().size() != getNumberCardsPerDeck())
		return;

	int cpt = 0;
	switch(vec.size())
	{
	case 4:
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			const int first = 3;
			for(int j = 0; j < first; j++)
			{
				vec[i]->addCard(cards.getTop());
			}
		}

		for(unsigned int i = 0; i < vec.size(); i++)
		{
			int second = 2;
			for(int j = 0; j < second; j++)
			{
				vec[i]->addCard(cards.getTop());
			}
		}
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			const int third = 3;
			for(int j = 0; j < third; j++)
			{
				vec[i]->addCard(cards.getTop());
			}
		}
		//const int first = 3;
		//for(int i = 0; i < first; i++)
		//{
		//	cpt += first;
		//	vec[i]->giveCard(&(*cards), cards + cpt);
		//}
		//int second = 2;
		//for(int i = 0; i < second; i++)
		//{
		//	cpt += second;
		//	vec[i]->giveCard(&(*cards), cards + cpt);
		//}
		//const int third = 3;
		//for(int i = 0; i < third; i++)
		//{
		//	cpt += third;
		//	vec[i]->giveCard(&(*cards), cards + cpt);
		//}
	}
}

int Rules::sumPoints(const std::vector<PtrCard> cards, const Contract& contract, bool lastTrick)
{
	assert(cards.size() > 0 && "Sum points, no cards to sum");

	int sum = 0;

	std::vector<CardPoint> strumpPoints;

	switch(contract.myBidType)
	{
	case rules::CLUBS:
	case rules::DIAMONDS:
	case rules::HEARTS:
	case rules::SPADES:
	{
		std::vector<CardPoint> strumpPoints = myCardsOrder[rules::TRUMP];
		std::vector<CardPoint> normalPoints = myCardsOrder[rules::NORMAL];

		for(unsigned int i = 0; i < cards.size(); i++)
		{
			if(cards[i]->getColor() == contract.myBidType)
			{
				for(unsigned int j = 0; j < strumpPoints.size(); j++)
				{
					if(strumpPoints[j].myValue == cards[i]->getValue())
					{
						sum += strumpPoints[j].myPoint;
						break;
					}
				}
			}
			else
			{
				for(unsigned int j = 0; j < normalPoints.size(); j++)
				{
					if(normalPoints[j].myValue == cards[i]->getValue())
					{
						sum += normalPoints[j].myPoint;
						break;
					}
				}
			}
		}
	}
	break;
	
	default:
		break;			
	}

	if(lastTrick)
		sum += 10;

	return sum;
}

bool Rules::checkContractAndComputeScore(Game& game, const Contract& contract, std::map<int, int>& tricksPerPlayerId)
{
	std::map<int, PtrTeam>& teams = game.getTeams();

	int multiplier = 1;
	if(contract.myCoincheType == rules::COINCHE)
	{
		multiplier = 2;
	}
	else if(contract.myCoincheType == rules::SURCOINCHE)
	{
		multiplier = 4;
	}

	if(isGeneral(game.getTeam(contract.myTeamId), tricksPerPlayerId))
	{
		
	}
	else if(isCapot(game.getTeam(contract.myTeamId), tricksPerPlayerId))
	{
			teams[contract.myTeamId]->resetCurrentPlayScore();
			teams[contract.myTeamId]->addCurrentPlayScore(250 + contract.myValue * multiplier);
			game.addCurrentScorePlayToGlobal();
	}
	else if(game.getTeam(contract.myTeamId)->getCurrentPlayScore() >= contract.myValue)
	{
		//switch(contract.myBidType)
		//{
		//case rules::CLUBS:
		//case rules::DIAMONDS:
		//case rules::HEARTS:
		//case rules::SPADES:
		//{
		//}
		//break;
		//}
		
		if(contract.myCoincheType == rules::COINCHE || contract.myCoincheType == rules::SURCOINCHE)
		{
			//teams[contract.myTeamId]->resetCurrentPlayScore();
			std::map<int, PtrTeam>::iterator it = teams.begin();
			for(; it != teams.end(); ++it)
			{
				
				if(it->first != contract.myTeamId)
				{
					it->second->resetCurrentPlayScore();
				}
			}
		}
		
		//roundScores(teams);
		teams[contract.myTeamId]->addCurrentPlayScore(contract.myValue * multiplier);

		game.addCurrentScorePlayToGlobal();//contract.myTeamId, tmpCalculation[contract.myTeamId]);
		//std::map<int, PtrTeam>::iterator it = teams.begin();
		//for(; it != teams.end(); ++it)
		//{
		//	if(it->first != contract.myTeamId)
		//	{
		//		game.addTeamScore(contract.myTeamId, tmpCalculation[contract.myTeamId]);
		//	}
		//}
	}
	else
	{
		//teams[contract.myTeamId]->resetCurrentPlayScore();
		std::map<int, PtrTeam>::iterator it = teams.begin();
		for(; it != teams.end(); ++it)
		{
			it->second->resetCurrentPlayScore();
			if(it->first != contract.myTeamId)
			{
				//160 to other teams + contract value
				it->second->addCurrentPlayScore(160 + contract.myValue * multiplier);
				//game.addTeamScore(it->first, 160 + contract.myValue * multiplier);
			}
		}
		game.addCurrentScorePlayToGlobal();
	}
	//std::map<int, PtrTeam>::iterator it = tmpCalculation.begin();
	//for(; it != tmpCalculation.end(); ++it)
	//{
	//	if(it->second->getTeamId() == contract.myTeamId)
	//	{
	//		if(it->second->getScore() > contract.myValue)
	//		{
	//			game.getPlayerById(it->first)->getTeam()->addScore(it->second->getScore());
	//			ILogger::log(ILogger::ALL) << "Nb points " << game.getPlayerById(it->first)->getTeam()->getScore() << "\n.";
	//			//parcourir les autres joueurs, ajouter leur score...
	//			return true;
	//		}
	//	}
	//}

	return false;
}

bool Rules::isGeneral(PtrTeam team, std::map<int, int>& tricksPerPlayerId)
{
	const std::vector<int>& ids = team->getPlayerIds();

	for(unsigned int i = 0; i < ids.size(); i++)
	{
		if(tricksPerPlayerId[ids[i]] == getNumberCardsPerPlayer())
			return true;
	}

	return false;
}

bool Rules::isCapot(PtrTeam team, std::map<int, int>& tricksPerPlayerId)
{
	const std::vector<int>& ids = team->getPlayerIds();
	int teamTricks = 0;

	for(unsigned int i = 0; i < ids.size(); i++)
	{
		teamTricks += tricksPerPlayerId[ids[i]];
	}

	if(teamTricks == getNumberCardsPerPlayer())
		return true;
	return false;
}