//
//  NPCComponent.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_NPCComponent_h
#define TheHerd_NPCComponent_h

#include "Engine/GameObject.h"

namespace Game {
	class NPCComponent : public Engine::GameComponent {
	public:
		NPCComponent(Engine::GameObject *gameObject) : GameComponent(gameObject) {}
		
		void interactWith(Engine::GameObject *minion);
		void tick() {}
	};
}

#endif
