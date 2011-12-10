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

namespace Game {
	class SelectionService : public Engine::Service, public OIS::MouseListener {
	public:
		void startup();
		void shutdown();
		void tick();
		
		bool mouseMoved(const OIS::MouseEvent& e);
		bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
	private:
		bool m_inMenu;
	};
}

#endif
