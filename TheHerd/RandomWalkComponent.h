//
//  RandomWalkComponent.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_RandomWalkComponent_h
#define TheHerd_RandomWalkComponent_h

#include "Engine/GameObject.h"
#include <Ogre/OgreVector2.h>

namespace Engine {
	class Clock;
}

namespace Game {
	class RandomWalkComponent : public Engine::GameComponent {
	public:
		RandomWalkComponent(Engine::GameObject *gameObject);
		
		void setClock(Engine::Clock *clock) {
			m_clock = clock;
		}
		
		void tick();
	private:
		Ogre::Vector2 m_direction;
		Engine::Clock *m_clock;
	};
}


#endif
