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
#include <list>
#include <Block.h>

namespace Engine {
	class Clock;
}

namespace Rocket {
	namespace Core {
		class ElementDocument;
	}
}

namespace Game {
	class GameService : public Engine::Service {
	public:
		GameService(Engine::Clock *clock, const std::string &playerName, std::string(^nameGenerator)(void)) : m_clock(clock), m_playerName(playerName), m_nameGenerator(Block_copy(nameGenerator)), m_exchangeRate(0.4) {}
		virtual ~GameService() {
			Block_release(m_nameGenerator);
		}
		
		float exchangeRate() {
			return m_exchangeRate;
		}
		
		Engine::GameObject *player() {
			return m_player;
		}
		
		Rocket::Core::ElementDocument *playerHud() {
			return m_playerHud;
		}
		
		Engine::Clock *clock() {
			return m_clock;
		}
		
		void startup();
		void shutdown();
		void tick();
	protected:
		std::list<Engine::GameObject*> m_minions;
		Engine::GameObject *m_player;
		Engine::Clock *m_clock;
		std::string m_playerName;
		std::string(^m_nameGenerator)(void);
		float m_exchangeRate;
		
		Rocket::Core::ElementDocument *m_playerHud;
		
		bool m_moveUp;
		bool m_moveDown;
		bool m_moveLeft;
		bool m_moveRight;
	};
}

#endif
