//
//  EventLogService.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_EventLogService_h
#define TheHerd_EventLogService_h

#include "Engine/Service.h"
#include <deque>

namespace Rocket {
	namespace Core {
		class ElementDocument;
	}
}

namespace Engine {
	class GameObject;
}

namespace Game {
	class EventLogService : public Engine::Service {
	public:
		EventLogService() : Service() {}
		
		void logInteraction(Engine::GameObject *subject, Engine::GameObject *object, const std::string &verb);
		
		void startup();
		void shutdown();
		void tick();
	private:
		std::deque<std::string> m_log;
	};
}

#endif
