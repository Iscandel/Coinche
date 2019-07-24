#ifndef H__CONTRACT_140220152204__H
#define H__CONTRACT_140220152204__H

#include <vector>

namespace rules
{
	enum BiddingType
	{
		HEARTS,
		SPADES,
		CLUBS,
		DIAMONDS,
		ALL_TRUMPS,
		NO_TRUMPS
	};

	enum Announce
	{
		TIERCE,
		BELOTE,
		_50,
		_100,
		NORMAL_SQUARE,
		ACE_SQUARE,
		JACK_SQUARE
	};

	enum CoincheType
	{
		NOT_COINCHE,
		COINCHE,
		SURCOINCHE
	};
}

struct Announce
{
	int myValue;
	int myOwnerId;
	rules::Announce myAnnounce;
	//std::vector<std::string> mySentences;
};

struct Contract
{
	Contract()
	{
		myValue = 0;
		myTeamId = 0;
		myBidType = rules::CLUBS;
		myCoincheType = rules::CoincheType::NOT_COINCHE;
	}

	int myValue;
	int myTeamId;
	std::vector<Announce> myAnnounces;
	rules::BiddingType myBidType;
	rules::CoincheType myCoincheType;
};

#endif