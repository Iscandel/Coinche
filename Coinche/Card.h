#ifndef H__CARD_261220131405__H
#define H__CARD_261220131405__H

#include "Entity.h"
#include "ResourceImage.h"
#include "EntityState.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>


class ICardHandler;

class Card : public GraphicEntity, public boost::enable_shared_from_this<Card>
{
public:
	enum CardColor
	{
		HEART,
		SPADE,
		CLUB,
		DIAMOND,
		UNKNOWNC
	};

	enum CardValue{
		//_1,
		ACE = 1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		_10,
		J,
		Q,
		K,
		JOKER,
		UNKNOWNV
	};

public:
	typedef std::map<std::string, PtrResourceImage> ResourceImagesMap;

public:
	Card(int id, Card::CardValue val, Card::CardColor col, int typeId);
	Card(int id, int typeId);
	~Card(void);

	void addResourceImage(const std::string& idName, PtrResourceImage res);// {myImages.insert(std::make_pair(idName, res));}// = res;}
	//void setImageBack(PtrResourceImage res) {myImageBack = res;}
	PtrResourceImage getImageFace() {return myImages["imageFace"];}
	PtrResourceImage getImageBack() {return myImages["imageBack"];}
	//PtrResourceImage getCurrentImage() {return myCurrentImage;} //ou std::vector. Retourner éventuellement l'objet Image
	std::vector<Image>& getCurrentImages() {return myCurrentImages;} //ou std::vector. Retourner éventuellement l'objet Image

	virtual void update(const Game& game, unsigned int elapsed);
	virtual void draw() {}
	virtual void moveTo(float x, float y) {}

	CardValue getValue() const {return myValue;}
	CardColor getColor() const {return myColor;}
	int getTypeId() const {return myTypeId;}
	void setTypeId(int typeId);

	void setValue(CardValue value) {myValue = value;}
	void setColor(CardColor color) {myColor = color;}

	//Called by card handler
	void affect(ICardHandler* handler);
	//Called by card handler
	void freeHandler() {myHandler = NULL;}

	void setState(boost::shared_ptr<EntityState<Card> > state);

	void removeFromHandler();

	void addCurrentDrawnImage(const std::string& name, int num = 0);
	void setCurrentDrawnImage(const std::string& name, int num = 0);
	//void setCurrentImage(const std::string& name) {myCurrentImage = myImages[name];} //A modif en vecteur de std::string ?

	friend std::ostream& operator << (std::ostream& o, const Card& h)
	{
		std::string col = "";
		if(h.getColor() == Card::CLUB)
			col = "CLUB";
		else if(h.getColor()  == Card::SPADE)
			col = "SPADE";
		else if(h.getColor()  == Card::HEART)
			col = "HEART";
		else if(h.getColor()  == Card::DIAMOND)
			col = "DIAMOND";
		o << h.getValue() << " " << col;

		return o;
	}

protected:
	CardValue myValue;
	CardColor myColor;

	int myTypeId;

	ICardHandler* myHandler;

	ResourceImagesMap myImages;
	//PtrResourceImage myCurrentImage;
	std::vector<Image> myCurrentImages;

	//PtrResourceImage myImage;
	//PtrResourceImage myImageBack;

	boost::shared_ptr<EntityState<Card> > myState;
};

typedef boost::shared_ptr<Card> PtrCard;

//std::vector<Trick> Rules::lookLastTricks(const TrickSet& set) {}
//
//class TrickSet
//{
//	void addTrick(Trick& trick);
//};
//
//class Trick : public CardHandler<Trick>
//{
//};

//class ICardHandler
//{
//public:
//	virtual bool addCard(PtrCard c) = 0;
//	virtual PtrCard removeCard(Card::CardValue value, Card::CardColor color) = 0;
//
//protected:
//	std::vector<PtrCard> myCards;
//};
//
//template<class T>
//class CardHandler
//{
//public:
//	virtual bool addCard(PtrCard c);
//	virtual PtrCard removeCard(Card::CardValue value, Card::CardColor color);
//};
//
//template<class T>
//bool CardHandler<T>::addCard(PtrCard c)
//{
//	//if(myCards.size() < Rules::getNbCards<T>)
//	//{
//	//	myCards.push_back(c);
//	//	c.affect(this);
//	//}
//	return false;
//}
//
//template<class T>
//PtrCard CardHandler<T>::removeCard(Card::CardValue value, Card::CardColor color)
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

//class Deck : public CardHandler<Deck>
//{
//
//};

#endif