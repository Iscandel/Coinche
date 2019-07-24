#include "TrickSet.h"


TrickSet::TrickSet(void)
:GraphicEntity(0)
{

}


TrickSet::~TrickSet(void)
{
}

void TrickSet::addTrick(boost::shared_ptr<Trick> trick)
{
	myTricks.push_back(trick);
}

std::vector<PtrCard> TrickSet::getCards()
{
	std::vector<PtrCard> res;
	for(unsigned int i = 0; i < myTricks.size(); i++)
	{
		std::vector<PtrCard> cards = myTricks[i]->getCards();
		res.insert(res.end(), cards.begin(), cards.end());
	}

	return res;
}