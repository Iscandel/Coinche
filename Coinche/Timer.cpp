#include "Timer.h"

#include <cmath>

Timer::Timer()
:myTime(0.f)
,myIsPaused(false)
{
	//MonHorloge.Reset();
}

Timer::~Timer(void)
{
}

void Timer::start()
{
	myTime = 0.f;
	myIsPaused = false;
	//MonHorloge.Reset();
}

void Timer::pause(unsigned int elapsed)
{
	if(myIsPaused)
		return;

	myTime += elapsed;

	//monTemps += MonHorloge.GetElapsedTime();
	//MonHorloge.Reset();

	myIsPaused = true;
}

unsigned int Timer::elapsedTime(unsigned int elapsed)
{
	if(!myIsPaused)
	{
		myTime += elapsed;
		//monTemps += MonHorloge.GetElapsedTime();
		//MonHorloge.Reset();
	}

	return myTime;
}


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
Countdown::Countdown(unsigned int duration)
{
	reset(duration);
}

void Countdown::reset(unsigned int duration)
{
	//monEstTermine = false;
	myDuration = duration;
}

//bool CompteARebours::estTermine() const 
//{
//	return MonChrono.tempsEcoule() >= maDuree;
//}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Constructeur
///////////////////////////////////////////////////////////////////////////
TimerFunction::TimerFunction(boost::function0<void> fonction, unsigned int duration, int id, unsigned int range)
:myFunction(fonction)
,myIsFinished(false)
,id(id)
,myRange(0)
,coeff(0)
{
	if(duration >= 0)
		myDuration = duration;
	else
		myIsFinished = true;

	//if(intervalle != -1)
	if(range > 1) //1 ms
	{
		//if(intervalle > 0)
			myRange = range;
	}

	start();
}

///////////////////////////////////////////////////////////////////////////
// Mise à jour de la classe
///////////////////////////////////////////////////////////////////////////
void TimerFunction::update(unsigned int elapsed)
{
	if(myIsFinished)
		return;
	
	//Cas 1 appel de fonction
	//if(abs((float)((float)monIntervalle - -1.f)) < 0.001f)
	if(myRange <= 1)
	{
		if(elapsedTime(elapsed) >= myDuration)
		{
			myFunction();
			myIsFinished = true;
		}
	}
	else //Cas Timer (appel toutes les x sec)
	{
		if(elapsedTime(elapsed) >= coeff * myRange )
		{
			myFunction();
			coeff++;
		}
		if(elapsedTime(0) >= myDuration)
		{
			myIsFinished = true;
		}
	}
}
