#include "Player.h"

#include "Game.h"

Player::Player(int id, Game* game)
:Entity(id)
,myGame(game)
{
}


Player::~Player(void)
{
}

void Player::sortCards()
{
	//std::vector<PtrCard> tmp = myCards;
	//for(unsigned int i = 0; i < myCards.size(); i++)
	//{
	//	for(unsigned int j = 0; j < myCards.size(); j++)
	//	{
	//		if(myCards[j]->getColor() == myCards[i]->getColor() && i != j)
	//		{
	//			if(i > 0 && myCards[i- 1]->getValue() < myCards[j]->getValue())
	//			{
	//				//myCards[i]
	//			}
	//		}
	//	}
	//}

	//1ère carte, trouver la distance min. Si insertion, on revient au début
	int i = 0;
	while(i < myCards.size())
	{
		std::pair<int, int> distAndIndex(100, -1);
		int distance = 100;
		
		for(int j = i + 1; j < myCards.size(); j++)
		{
			if(myCards[j]->getColor() == myCards[i]->getColor())
			{
				if(std::abs(myCards[j]->getValue() - myCards[i]->getValue()) < std::abs(distAndIndex.first))
				{
					distAndIndex.first = myCards[j]->getValue() - myCards[i]->getValue();
					distAndIndex.second = j;
				}
			}
		}

		if(distAndIndex.second != -1)// != std::abs(distAndIndex.second - i) != 1)
		{
			if(std::abs(distAndIndex.second - i) != 1 ||(distAndIndex.second - i == 1 && distAndIndex.first < 0) || (distAndIndex.second - i == -1 && distAndIndex.first > 0))
			{
				PtrCard tmp = myCards[i];
				myCards.erase(myCards.begin() + i);
				if(distAndIndex.first < 0 ) 
					myCards.insert(myCards.begin() + distAndIndex.second , tmp);
				else
					myCards.insert(myCards.begin() + distAndIndex.second - 1, tmp);
			
				i = 0;
			}
			else
				i++;
		}
		else
			i++;
	}
}

void Player::update(const Game& game, unsigned int elapsedTime)
{
	if(myState)
		myState->update(*this, game,elapsedTime);

	for(int i = 0; i < myCards.size(); i++)
	{
		myCards[i]->update(game, elapsedTime);
	}
}

void Player::setState(boost::shared_ptr<EntityState<Player> > state)
{
	if(myState)
		myState->free(*this);

	myState = state;

	if(myState) 
		myState->init(*this);
}

//bool Player::addCard(PtrCard c)
//{
//	//if(myCards.size() < Rules::nbCards)
//	//{
//	//	myCards.push_back(c);
//	//	c.affect(this);
//	//}
//	return false;
//}
//
//PtrCard Player::removeCard(Card::CardValue value, Card::CardColor color)
//{
//	for(int i = 0; i < myCards.size(); i++)
//	{
//		if(myCards[i]->getValue() == value && myCards[i]->getColor() == color)
//		{
//			return myCards[i];
//		}		
//	}
//
//	return PtrCard();
//}