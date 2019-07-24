#include "MenuView.h"

#include "Game.h"
#include "GameEngine.h"
#include "GraphicEngine.h"
#include "TextureManager.h"


MenuView::MenuView(const std::string&)
{
}


MenuView::~MenuView(void)
{
}

void MenuView::draw(GameState& state)
{
	Game& game = state.getGameEngine()->getGame();
	int windowSizeX = state.getGraphicEngine()->getWindowSizeX();
	int windowSizeY = state.getGraphicEngine()->getWindowSizeY();

	PtrResourceTexture fond = TextureManager::getInstance()->getResource(3);

	sf::Sprite spFond;
	spFond.SetTexture(*fond->getTexture());
	spFond.SetPosition(0, 0);
	spFond.SetScale((float)windowSizeX / fond->getTexture()->GetWidth(), (float)windowSizeY / fond->getTexture()->GetHeight());
	state.getGraphicEngine()->draw(spFond);
}
