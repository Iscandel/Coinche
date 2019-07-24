#ifndef H__ENTITYMANAGER_260120150121__H
#define H__ENTITYMANAGER_260120150121__H

#include "Card.h"

class EntityManager
{
public:
	void registerEntity(PtrCard entity)
	{
		myEntities.push_back(entity);
	}

	PtrCard removeEntity(int id)
	{
		for(int i = 0; i < myEntities.size(); i++)
		{
			if(myEntities[i]->getId() == id)
			{
				myEntities[i]->removeFromHandler();
				return myEntities[i];
			}
		}
		return PtrCard();
	}

	void update();

	//std::vector<PtrGraphicEntity> getEntities() {return myEntities;}
	std::vector<PtrCard> getEntities() {return myEntities;}


	//std::vector<PtrGraphicEntity> getHoveredEntities(const Point<double>& relPos)
	//{
	//	std::vector<PtrGraphicEntity> res;
	//	for(unsigned int i = 0; i < myEntities.size(); i++)
	//	{
	//		Point<double> pos(myEntities[i]->getX(), myEntities[i]->getY());
	//		Point<double> size(myEntities[i]->getRelSizeX(), myEntities[i]->getRelSizeY());

	//		double rotation = myEntities[i]->getRotation();
	//		if(std::abs(rotation) > 0.00001)
	//		{
	//			std::vector<Point<double> > relGraphicPosition;
	//			relGraphicPosition.push_back(tools::applyRotation(pos, rotation));
	//			relGraphicPosition.push_back(tools::applyRotation(Point<double>(pos.x +  size.x, pos.y), rotation));
	//			relGraphicPosition.push_back(tools::applyRotation(Point<double>(pos.x, pos.y + size.y), rotation));
	//			relGraphicPosition.push_back(tools::applyRotation(Point<double>(pos.x + size.x, pos.y + size.y), rotation));

	//			Point<double> topLeft(relGraphicPosition[0]);
	//			Point<double> downRight(relGraphicPosition[0]);
	//			for(unsigned int i = 0; i < relGraphicPosition.size(); i++)
	//			{
	//				if(relGraphicPosition[i].x < topLeft.x)
	//					topLeft.x = relGraphicPosition[i].x;
	//				if(relGraphicPosition[i].y < topLeft.y)
	//					topLeft.y = relGraphicPosition[i].y;
	//				if(relGraphicPosition[i].x > downRight.x)
	//					downRight.x = relGraphicPosition[i].x;
	//				if(relGraphicPosition[i].y > downRight.y)
	//					downRight.y = relGraphicPosition[i].y;

	//				pos = topLeft;
	//				size.x = downRight.x - topLeft.x;
	//				size.y = downRight.y - topLeft.y;
	//			}

	//		}

	//		if(relPos.x >= pos.x &&
	//		   relPos.y >= pos.y &&
	//		   relPos.x <= pos.x + size.x && 
	//		   relPos.y <= pos.y + size.y)
	//		{
	//			res.push_back(myEntities[i]);
	//		}
	//	}
	//	return res;
	//}



	int createNewId();

	std::vector<PtrCard> myEntities;
	//std::vector<PtrGraphicEntity> myEntities;
};

#endif
