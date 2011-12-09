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

namespace Game {
	class GameService : public Engine::Service {
	public:
		void startup();
		void shutdown();
		void tick();
	protected:
		std::vector<Engine::GameObject*> m_minions;
	};
}

#endif
