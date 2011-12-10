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
		virtual ~InteractionComponent() {}
		
		void trade(Engine::GameObject *minion);
		void attack(Engine::GameObject *minion);
		void occupy(Engine::GameObject *minion);
		void repay(Engine::GameObject *minion);
		void police(Engine::GameObject *minion);
		
		void tick() {}
	};
}

#endif
