#include "GraphicEngine.h"
#include "GameEngine.h"
#include "Gaia/Gaia.h"
#include "Gaia/SFMLRenderer/SFMLRenderer.h"
#include "Gaia/XMLLoader/XMLGaia.h"

GraphicEngine::GraphicEngine(void)
:myElapsedTime(0)
{
	setWindowSize(Point<int>(1024, 768));
	myWindow.Create(sf::VideoMode(getWindowSizeX(), getWindowSizeY()), "Yop");
	//myWindow.EnableVerticalSync(true);
	myWindow.SetFramerateLimit(60);
	gaia::GuiManager::getInstance()->setWindowSize(getWindowSizeX(), getWindowSizeY());
	gaia::BaseGraphics* graphics = new gaia::SFMLGraphics(&myWindow);
	graphics->setGlobalFont(gaia::PtrFont(new gaia::SFMLFont("./data/fonts/PORKYS_.ttf")));
	gaia::GuiManager::getInstance()->setGraphics(graphics);
	gaia::xml::XMLWidgetLookLoader::loadImageSet("./data/gui/imagesWindowsTheme.xml");
	gaia::xml::XMLWidgetLookLoader::loadLooks("./data/gui/lookWindowsTheme.xml");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
GraphicEngine::~GraphicEngine(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void GraphicEngine::processMessage(const Message& msg)
{
	switch(msg.type)
	{
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void GraphicEngine::update()
{
	processQueue();

	//draw();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void GraphicEngine::draw()
{
	setElapsedTime(myClock.GetElapsedTime());
	//std::cout << 1000. / getElapsedTime() << std::endl;
	myClock.Reset();
	myWindow.Clear(sf::Color::Black);
	getGameEngine()->draw();
	gaia::GuiManager::getInstance()->draw();
	sf::Font font;
	font.LoadFromFile("./data/fonts/PORKYS_.ttf");
	sf::Text text("Solo", font, 32);
	text.SetCharacterSize(32);
	myWindow.Draw(text);
	sf::RectangleShape rect(sf::Vector2f(text.GetGlobalBounds().Width, text.GetGlobalBounds().Height));
	rect.SetPosition(0.f, 0.f);
	rect.SetFillColor(sf::Color(0,0,0,50));
	myWindow.Draw(rect);
	myWindow.Display();
}

void GraphicEngine::draw(const sf::Drawable& sp)
{
	myWindow.Draw(sp);
}

void GraphicEngine::draw(PtrTexture texture, int x, int y, IntRect& rect, int scaleX, int scaleY, float rotation)
{
}