//
//  InteractionComponent.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_InteractionComponent_h
#define TheHerd_InteractionComponent_h

#include "Engine/GameObject.h"

namespace Game {
	class InteractionComponent : public Engine::GameComponent {
	public:
		InteractionComponent(Engine::GameObject *gameObject) : GameComponent(gameObject) {}
		
		void interactWith(Engine::GameObject *minion);
		void tick() {}
	};
}

#endif
