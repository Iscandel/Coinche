#include "EFlipCard.h"

#include "Card.h"


EFlipCard::EFlipCard(void)
:myInitialSizeX(0.)
,myInitialSizeY(0.)
,myIsFirstPhase(true)
{
}


EFlipCard::~EFlipCard(void)
{
}

void EFlipCard::init(Card& parent)
{
	//Modifier éventuellement l'origine pour la centrer (ajuster en conséquence la position)
	myInitialSizeX = parent.getRelSizeX();
	myInitialSizeY = parent.getRelSizeY();

	myInitialX = parent.getX();
	myClock.Reset();
	myClock2.Reset();;
}

void EFlipCard::free(Card& parent)
{
	parent.setRelSize(myInitialSizeX, myInitialSizeY);
	//Remettre l'origine en haut à gauche et ajuster la position
}
	
void EFlipCard::update(Card& parent, const Game&, unsigned int elapsedTime)
{
	double sizeSpeedX = 3.; //0.5 du travail à faire par seconde sur sizeX
	double timeRequired = myInitialSizeX /  (sizeSpeedX * myInitialSizeX);
	double positionSpeedX = -((myInitialSizeX  / 2) / timeRequired);

	const double scaleY = 1. / 4.;
	double sizeSpeedY = -(myInitialSizeY * scaleY) / timeRequired;
	double positionSpeedY = (myInitialSizeY * scaleY / 2) / timeRequired;

	if(myIsFirstPhase)
	{
		sizeSpeedX = -sizeSpeedX;
		positionSpeedX = -positionSpeedX;
		positionSpeedY = -positionSpeedY;
		sizeSpeedY = -sizeSpeedY;
	}

	double currentSizeX = parent.getRelSizeX();
	double currentSizeY = parent.getRelSizeY();

	double newSizeX = 0.;
	//if(myClock.GetElapsedTime() > 1000)
	{
		newSizeX = currentSizeX + (sizeSpeedX * myInitialSizeX) / 1000. * elapsedTime;//myClock.GetElapsedTime();//elapsedTime;
		double newSizeY = currentSizeY + (sizeSpeedY) / 1000. * elapsedTime;//myClock.GetElapsedTime();//elapsedTime;
	
		double newX = parent.getX() + positionSpeedX / 1000. * elapsedTime;//myClock.GetElapsedTime();//elapsedTime;
		double newY = parent.getY() + positionSpeedY / 1000. * elapsedTime;//myClock.GetElapsedTime();//elapsedTime;
	myClock.Reset();
	//if(myClock2.GetElapsedTime() > 1000)
	//{
	//		//std::cout << myClock.GetElapsedTime() << " " << ((speed * myInitialSizeX) / 1000. * myClock.GetElapsedTime()) << std::endl;
	//		std::cout << myInitialSizeX << " " << speed << " " << newSizeX << std::endl;
	//		myClock2.Reset();
	//}
	//double newSizeX = currentSizeX + speed / 1000 * elapsedTime;

	parent.setRelSize(newSizeX, newSizeY);//currentSizeY);
	parent.setPosition(Point<double>(newX, newY));//parent.getY()));
	}

	//double newX = parent.getX() + (myInitialX + myInitialSizeX)

	if(newSizeX < 0.)
	{
		myIsFirstPhase = false;
		//parent.setCurrentImage("imageFace");
		parent.setCurrentDrawnImage("imageFace");
		std::cout << myClock2.GetElapsedTime() << std::endl;
	}

	if(newSizeX > myInitialSizeX)
	{
		parent.setState(boost::shared_ptr<EntityState<Card> >()); //Reset state
	}
}
