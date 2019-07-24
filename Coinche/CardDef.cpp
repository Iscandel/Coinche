#include "CardDef.h"


CardDef::CardDef(Card::CardValue value, Card::CardColor color, int imageId, int imageIdBack)//int typeId)
:myValue(value)
,myColor(color)
,myImageId(imageId)
,myImageIdBack(imageIdBack)
//,myCardTypeId(typeId)
{
	myLoadingStatus = IS_LOADED;
}


CardDef::~CardDef(void)
{
}
