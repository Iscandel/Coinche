#include "HoverableTextRenderer.h"

HoverableTextRenderer::HoverableTextRenderer(gaia::PtrWidget widget)
:TplWidgetRenderer<gaia::Label>(widget)
{
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void HoverableTextRenderer::draw_impl(gaia::BaseGraphics* gfx)
{
	const std::string& text = myWidget->getText();
	gaia::PtrFont font = myWidget->getFont();
	
	if(myWidget->getWidgetState() == gaia::BaseWidget::HOVER)
	{
		gfx->drawRectangle(gaia::Point(0, 0), 
						   gaia::Point(myWidget->getWidth(), myWidget->getHeight()), 
						   gaia::Color(0, 0, 0, 20),
						   5,
						   gaia::Color(200, 200, 200, 75)
		);
	}

	gfx->drawText(font, 
		text, 
		0,//myWidget->getX(), 
		0,//myWidget->getY(), 
		myWidget->getTextColor(), 
		myWidget->getFontSize());
}