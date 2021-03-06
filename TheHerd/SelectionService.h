//
//  SelectionService.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_SelectionService_h
#define TheHerd_SelectionService_h

#include "Engine/Service.h"
#include <OIS/OIS.h>

namespace Rocket {
	namespace Core {
		class ElementDocument;
	}
}

namespace Engine {
	class GameObject;
}

namespace FMOD {
	class Event;
}

namespace Game {
	class SelectionService : public Engine::Service, public OIS::MouseListener {
	public:
		void startup();
		void shutdown();
		void tick();
		
		void minionDied(Engine::GameObject *minion);
		
		bool mouseMoved(const OIS::MouseEvent& e);
		bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
	private:
		void updateMenu();
		
		bool m_inMenu;
		bool m_canTrade;
		bool m_canAttack;
		bool m_canOccupy;
		bool m_canRepay;
		bool m_canPolice;
		Rocket::Core::ElementDocument *m_actionMenu;
		Engine::GameObject *target;
		
		FMOD::Event *m_tradeEvent;
		FMOD::Event *m_attackEvent;
		FMOD::Event *m_occupyEvent;
		FMOD::Event *m_repayEvent;
	};
}

#endif
