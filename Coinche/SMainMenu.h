#ifndef H__SMAINMENU_19022015__H
#define H__SMAINMENU_19022015__H

#include "GameState.h"
#include "Player.h"
#include "Gaia/Gaia.h"
#include "Gaia/SFMLRenderer/SFMLRenderer.h"
#include "Gaia/XMLLoader/XMLGaia.h"

#include <string>


class SMainMenu :
	public GameState
{
public:
	SMainMenu(void);
	~SMainMenu(void);

	void init();

	///////////////////////////////////////////////////////////////////////////
	/// Pauses the game state.
	///////////////////////////////////////////////////////////////////////////
	virtual void pause() {}

	///////////////////////////////////////////////////////////////////////////
	/// Resumes the game state.
	///////////////////////////////////////////////////////////////////////////
	virtual void resume() {}

	///////////////////////////////////////////////////////////////////////////
	/// Updates the game state.
	///////////////////////////////////////////////////////////////////////////
	virtual void update();

	virtual void processMessage(const Message& msg);

	bool catchEvent(const sf::Event& ev);

	void onClicSolo(gaia::MouseEvent& ev);
};

#endif