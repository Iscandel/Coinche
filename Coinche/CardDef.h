#ifndef H__CARDDEF_090120152112__H
#define H__CARDDEF_090120152112__H

#include "Card.h"
#include "IResource.h"

class CardDef : public IResource
{
public:
	CardDef(Card::CardValue value, Card::CardColor color, int imageId, int imageIdBack);//int typeId);
	~CardDef(void);
	
	Card::CardValue myValue;
	Card::CardColor myColor;
	//int myCardTypeId;
	int myImageId;
	int myImageIdBack;
};

#endif