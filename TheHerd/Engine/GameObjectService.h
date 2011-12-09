//
//  GameObjectService.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_GameObjectManager_h
#define Testing_GameObjectManager_h

#include "Service.h"
#include <set>

namespace Engine {
	class GameObject;
	
	class GameObjectService : public Service {
	public:
		void startup() {}
		void shutdown();
		void tick();
		
		void addGameObject(GameObject *gameObject) {
			m_gameObjects.insert(gameObject);
		}
		
		void removeGameObject(GameObject *gameObject) {
			m_gameObjects.erase(gameObject);
		}
	private:
		std::set<GameObject*> m_gameObjects;
	};
}

#endif
