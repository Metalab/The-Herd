//
//  MinionComponent.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_MinionComponent_h
#define TheHerd_MinionComponent_h

#include "Engine/GameObject.h"

namespace Game {
	class MinionComponent : public Engine::GameComponent {
	public:
		MinionComponent(Engine::GameObject *gameObject) : GameComponent(gameObject) {}
		virtual ~MinionComponent() {}
		
		float life();
		void changeLife(float amount);
		
		int money();
		void changeMoney(int amount);
		
		void addStake(Engine::GameObject *stakeholder);
		Engine::GameObject *stakeHolder();
		void buyOutStake();
	};
}

#endif
