//
//  GameObjectService.cc
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "GameObjectService.h"
#include "GameObject.h"

namespace Engine {
	void GameObjectService::shutdown() {
		
	}
	
	void GameObjectService::tick() {
		for(std::set<GameObject*>::iterator iter = m_gameObjects.begin(); iter != m_gameObjects.end(); ++iter) {
			(*iter)->tick();
		}
	}
}
