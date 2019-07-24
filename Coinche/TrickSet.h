#ifndef H__TRICKSET_250120150128__H
#define H__TRICKSET_250120150128__H

#include "Entity.h"
#include "Trick.h"

#include <boost/smart_ptr.hpp>
#include <vector>

class TrickSet : public GraphicEntity
{
public:
	TrickSet(void);
	~TrickSet(void);

	void addTrick(boost::shared_ptr<Trick> trick);

	std::vector<PtrCard> getCards();

	const std::vector<boost::shared_ptr<Trick>>& getTricks() {return myTricks;}

	void update(const Game&, unsigned int) {}
	//void draw() {}


protected:
	std::vector<boost::shared_ptr<Trick>> myTricks;
};

#endif