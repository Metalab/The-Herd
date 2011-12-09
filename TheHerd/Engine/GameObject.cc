//
//  GameObject.cc
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "GameObject.h"

namespace Engine {
	GameObject::~GameObject() {
		for(std::set<GameComponent*>::iterator iter = m_components.begin(); iter != m_components.end(); ++iter) {
			delete *iter;
		}
	}
	
	void GameObject::tick() {
		if(m_wantsUpdate)
			for(std::set<GameComponent*>::iterator iter = m_components.begin(); iter != m_components.end(); ++iter)
				(*iter)->tick();
	}
}
