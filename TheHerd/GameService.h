//
//  GameService.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_GameService_h
#define TheHerd_GameService_h

#include "Engine/Service.h"
#include "Engine/GameObject.h"
#include <vector>

namespace Engine {
	class Clock;
}

namespace Game {
	class GameService : public Engine::Service {
	public:
		GameService(Engine::Clock *clock) : m_clock(clock) {}
		
		void startup();
		void shutdown();
		void tick();
	protected:
		std::vector<Engine::GameObject*> m_minions;
		Engine::GameObject *m_player;
		Engine::Clock *m_clock;
		
		bool m_moveUp;
		bool m_moveDown;
		bool m_moveLeft;
		bool m_moveRight;
	};
}

#endif
