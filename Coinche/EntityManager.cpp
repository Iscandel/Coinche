#include "EntityManager.h"


void EntityManager::update()
{
	std::vector<PtrCard>::iterator it = myEntities.begin();
	for(it; it != myEntities.end();)
	{
		if((*it)->mustBeDestroyed())
			it = myEntities.erase(it);
		else
			++it;
	}
}

int EntityManager::createNewId()
{
	static int id = 1;

	return id++;
}