#include "Card.h"
#include "CardHandler.h"

#include "CardDefManager.h"
#include "ResourceImageManager.h"

Card::Card(int id, Card::CardValue val, Card::CardColor col, int typeId)
:GraphicEntity(id)
,myValue(val)
,myColor(col)
,myHandler(NULL)
,myTypeId(typeId)
{
	CardDefManager::PtrResource def = CardDefManager::getInstance()->getResource(typeId);
	if(def)
	{
		addResourceImage("imageFace", ResourceImageManager::getInstance()->getResource(def->myImageId));
		addResourceImage("imageBack", ResourceImageManager::getInstance()->getResource(def->myImageIdBack));
	}
}

Card::Card(int id, int typeId)
:GraphicEntity(id)
,myValue(UNKNOWNV)
,myColor(UNKNOWNC)
,myHandler(NULL)
,myTypeId(-1)
{
	setTypeId(typeId);
}


Card::~Card(void)
{
}

void Card::setTypeId(int typeId)
{
	CardDefManager::PtrResource def = CardDefManager::getInstance()->getResource(typeId);
	if(def)
	{
		myColor = def->myColor;
		myValue = def->myValue;
		addResourceImage("imageFace", ResourceImageManager::getInstance()->getResource(def->myImageId));
	    addResourceImage("imageBack", ResourceImageManager::getInstance()->getResource(def->myImageIdBack));
		myTypeId = typeId;
	}
}

void Card::addResourceImage(const std::string& idName, PtrResourceImage res)
{
	myImages[idName] = res;
}

void removeResourceImage(const std::string& idName)
{
}

void Card::addCurrentDrawnImage(const std::string& name, int num)
{
	ResourceImagesMap::iterator it = myImages.find(name);
	if(it != myImages.end())
	{
		myCurrentImages.push_back(it->second->getImage(num));
	}
}

void Card::setCurrentDrawnImage(const std::string& name, int num) 
{
	myCurrentImages.clear();
	addCurrentDrawnImage(name, num);
}

void Card::affect(ICardHandler* handler)
{
	if(myHandler)// && handler)
	{
		myHandler->removeCard(shared_from_this());
	}
	
	myHandler = handler;
}

void Card::update(const Game& game, unsigned int elapsed) 
{
	if(myState)
	{
		myState->update(*this, game, elapsed);
	}
}

void Card::setState(boost::shared_ptr<EntityState<Card> > state) 
{
	if(myState)
		myState->free(*this);

	myState = state;

	if(myState) 
		myState->init(*this);
}

void Card::removeFromHandler() 
{
	//myHandler->removeCard(shared_from_this());

	//freeHandler();
	affect(NULL);
}
