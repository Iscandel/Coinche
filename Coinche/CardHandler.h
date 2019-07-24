#ifndef H__CARDHANDLER_311220131218__H
#define H__CARDHANDLER_311220131218__H

#include <ostream>
#include <vector>

#include "Card.h"
#include "Rules.h"

class ICardHandler
{
public:
	virtual ~ICardHandler(void);

	virtual bool addCard(PtrCard c) = 0;
	virtual std::vector<PtrCard>& getCards() = 0;
	PtrCard removeCard(PtrCard card);
	void removeCards();
	virtual PtrCard getCard(int id) = 0;
	virtual PtrCard removeCard(int id) = 0;
	//virtual PtrCard removeCard(Card::CardValue value, Card::CardColor color) = 0;
	
	//void reset() {myCards.clear();}

protected:
	std::vector<PtrCard> myCards;
};

template<class T>
class CardHandler : public ICardHandler
{
public:
	virtual bool addCard(PtrCard c);
	virtual std::vector<PtrCard>& getCards();

	virtual PtrCard getCard(int id);
	virtual PtrCard removeCard(int id);
	//virtual PtrCard removeCard(Card::CardValue value, Card::CardColor color);

	friend std::ostream& operator << (std::ostream& o, CardHandler<T>* h)
	{
		for(unsigned int i = 0; i < h->getCards().size(); i++)
		{
			o << h->getCards()[i]  << std::endl;
			//std::string col = "";
			//if(h->getCards()[i]->getColor() == Card::CLUB)
			//	col = "CLUB";
			//else if(h->getCards()[i]->getColor() == Card::SPADE)
			//	col = "SPADE";
			//else if(h->getCards()[i]->getColor() == Card::HEART)
			//	col = "HEART";
			//else if(h->getCards()[i]->getColor() == Card::DIAMOND)
			//	col = "DIAMOND";
			//o << h->getCards()[i]->getValue() << " " << col << std::endl;
		}

		return o;
	}

	std::vector<PtrGraphicEntity> getHoveredEntities(const Point<double>& relPos, 
													 const Point<int>& windowSize)
	{
		//std::cout << relPos.x << " " << relPos.y << std::endl;
		std::vector<PtrGraphicEntity> res;
		for(int i = 0; i < myCards.size(); i++)
		{
			Rect<double> boundingRect = myCards[i]->getBoundingRect(windowSize.x, windowSize.y);

			if(relPos.x >= boundingRect.x &&
			   relPos.y >= boundingRect.y &&
			   relPos.x <= boundingRect.x + boundingRect.width && 
			   relPos.y <= boundingRect.y + boundingRect.height)
			{
				if(myCards[i]->isSelectable())
					res.push_back(myCards[i]);
			}
		}
		return res;
	}
};

template<class T>
bool CardHandler<T>::addCard(PtrCard c)
{
	if((int)myCards.size() < Rules::getInstance()->getNumberCards<T>() && c)
	{
		c->affect(this);
		myCards.push_back(c);	
		return true;
	}
	return false;
}

template<class T>
std::vector<PtrCard>& CardHandler<T>::getCards()
{
	return myCards;
}

template<class T>
PtrCard CardHandler<T>::getCard(int id)
{
	for(unsigned int i = 0; i < myCards.size(); i++)
	{
		if(myCards[i]->getId() == id)
			return myCards[i];
	}

	return PtrCard();
}

template<class T>
PtrCard CardHandler<T>::removeCard(int id)
{
	for(unsigned int i = 0; i < myCards.size(); i++)
	{
		if(myCards[i]->getId() == id)
		{
			PtrCard res = myCards[i];
			res->freeHandler();
			myCards.erase(myCards.begin() + i);
			return res;
		}		
	}

	return PtrCard();
}

//template<class T>
//PtrCard CardHandler<T>::removeCard(Card::CardValue value, Card::CardColor color)
//{
//	for(unsigned int i = 0; i < myCards.size(); i++)
//	{
//		if(myCards[i]->getValue() == value && myCards[i]->getColor() == color)
//		{
//			PtrCard res = myCards[i];
//			myCards.erase(myCards.begin() + i);
//			return res;
//		}		
//	}
//
//	return PtrCard();
//}

#endif