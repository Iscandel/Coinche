#pragma once
#include "View.h"

class MenuView : public View
{
public:
	MenuView(const std::string& path);
	~MenuView(void);

	virtual void draw(GameState& state);
	void loadViewPosition(const std::string& path);
};
