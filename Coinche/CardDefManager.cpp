#include "CardDefManager.h"

#include "tools/Tools.h"
#include "tools/FileReading.h"


CardDefManager::CardDefManager(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CardDefManager::PtrResource CardDefManager::loadResource(int id)
{
	MapResources::iterator it = myResources.find(id);
	if(it == myResources.end())
		throw std::runtime_error("CardDef " + tools::numToString(id) + " should already exist.");

	throw std::runtime_error("No need to load CardDef " + tools::numToString(id) + " Operation aborted.");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CardDefManager::~CardDefManager(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CardDefManager::freeResources()
{
	MapResources::iterator it = myResources.begin();
	for( ; it != myResources.end(); ++it)
	{
		it = myResources.erase(it);
	}

	//myFakeCardDef.reset();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CardDefManager::freeResource(int id)
{
	MapResources::iterator it = myResources.find(id);
	if(it != myResources.end())
		myResources.erase(it);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//CardDefManager::PtrResource CardDefManager::getFakeCardDef()
//{
//	return myFakeCardDef;
//}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CardDefManager::preload(const std::string& fileName)
{
	TextFile file(fileName);

	int nbElements = 0, color, value, imageId, imageIdBack, typeId, nbJokers, nbFakeCards;

	file >> nbElements;
	file >> nbJokers;
	file >> nbFakeCards;

	for(int i = 0; i < nbElements; i++)
	{
		file >> typeId >> color >> value >> imageId >> imageIdBack;
		PtrResource def(new CardDef((Card::CardValue) value, (Card::CardColor) color, imageId, imageIdBack));

		myResources[typeId] = def;
	}

	//Jokers (same as cards actually)
	for(int i = 0; i < nbJokers; i++)
	{
		file >> typeId >> color >> value >> imageId >> imageIdBack;
		PtrResource def(new CardDef((Card::CardValue) value, (Card::CardColor) color, imageId, imageIdBack));
		myResources[typeId] = def;
	}

	//Fake card(s). Nearly the same
	for(int i = 0; i < nbFakeCards; i++)
	{
		file >> typeId >> color >> value >> imageId >> imageIdBack;
		PtrResource def(new CardDef((Card::CardValue) value, (Card::CardColor) color, imageId, imageIdBack));
		myResources[typeId] = def;

		myFakeCardId = typeId;
	}

	//Fake card
	//file >> typeId >> color >> value >> imageId;
	//PtrResource def(new CardDef((Card::CardValue) value, (Card::CardColor) color, imageId));
	//myFakeCardDef = def;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
