#pragma once

#include <map>
#include <string>
#include <boost/smart_ptr.hpp>

#include "tools/Point.h"

class GameState;

class View
{
public:
	View(void);
	~View(void);

	typedef std::map<std::string, Point<double> > PosMap;

	Point<double> getGivenPosition(const std::string& pos)
	{
		PosMap::iterator it = myPositions.find(pos);
		if(it != myPositions.end())
		{
			return it->second;
		}
		else
		{
			std::string msg = "No view position " + pos;
			throw std::runtime_error(msg.c_str());
		}
	}

	virtual void draw(GameState& state) = 0;

protected:
	PosMap myPositions;
};

typedef boost::shared_ptr<View> PtrView;

