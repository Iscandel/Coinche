#ifndef H__GAMEVIEW_260120142204__H
#define H__GAMEVIEW_260120142204__H

#include "View.h"
#include "Card.h"

class GameView :
	public View
{
public:
	GameView(const std::string& path);
	~GameView(void);

	virtual void draw(GameState& state);
	void loadViewPosition(const std::string& path);

	void drawCards(GameState& state, const std::vector<PtrCard>& cards);
};

#endif