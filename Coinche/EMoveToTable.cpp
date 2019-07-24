#include "EMoveToTable.h"

#include "Card.h"
#include "tools/Tools.h"
#include "MessageBuilder.h"
#include "MessageSender.h"

#include <cmath>
#include <boost/smart_ptr.hpp>



EMoveToTable::EMoveToTable(const Point<double>& destination)
:myDestination(destination)
{
	
}

void EMoveToTable::init(Card& parent)
{
	const double speed = 0.3;//0.6;//0.16;
	Point<double> startPoint(parent.getX(), parent.getY());
	Point<double> deviation = myDestination - startPoint;
	double angle = std::atan2(-deviation.y, deviation.x);

	speedVector = Point<double>(speed * std::cos(angle), -speed * std::sin(angle));

	//Ou normaliser le vecteur deviation, puis multiplier par la norme du vecteur vitesse (speed)

	parent.setCurrentDrawnImage("imageFace");
}

void EMoveToTable::free(Card& parent)
{
	Message msg;
	msg.type = Message::MessageType::CARD_ARRIVED_ON_TABLE;
	MessageSender::getInstance()->sendMessageToGame(msg);
}

void EMoveToTable::update(Card& parent, const Game&, unsigned int elapsedTime)
{
	bool myDestXIsSup =  myDestination.x - parent.getX() > 0 ? true : false;
	bool myDestYIsSup =  myDestination.y - parent.getY() > 0 ? true : false;

	Point<double> startPoint(parent.getX(), parent.getY());	

	parent.setPosition(Point<double>(parent.getX() + speedVector.x /1000 * elapsedTime, parent.getY() + speedVector.y /1000 * elapsedTime));

	bool myNewDestXIsSup =  myDestination.x - parent.getX() > 0 ? true : false;
	bool myNewDestYIsSup =  myDestination.y - parent.getY() > 0 ? true : false;

	//If finished
	if(myDestXIsSup != myNewDestXIsSup || myDestYIsSup != myNewDestYIsSup)
	{
		parent.setPosition(Point<double>(myDestination.x, myDestination.y));
		parent.setState(boost::shared_ptr<EntityState<Card> >()); //Reset state

		//Message msg = MessageBuilder::clMovedToHand(parent.getId());
		//MessageSender::getInstance()->sendMessageToGame(msg);
	}
}
