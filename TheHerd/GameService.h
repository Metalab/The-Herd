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
#include <Block.h>

namespace Engine {
	class Clock;
}

namespace Game {
	class GameService : public Engine::Service {
	public:
		GameService(Engine::Clock *clock, const std::string &playerName, std::string(^nameGenerator)(void)) : m_clock(clock), m_playerName(playerName), m_nameGenerator(Block_copy(nameGenerator)) {}
		virtual ~GameService() {
			Block_release(m_nameGenerator);
		}
		
		void startup();
		void shutdown();
		void tick();
	protected:
		std::vector<Engine::GameObject*> m_minions;
		Engine::GameObject *m_player;
		Engine::Clock *m_clock;
		std::string m_playerName;
		std::string(^m_nameGenerator)(void);
		
		bool m_moveUp;
		bool m_moveDown;
		bool m_moveLeft;
		bool m_moveRight;
	};
}

#endif
