//
//  InputService.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_InputService_h
#define Testing_InputService_h

#include "Service.h"
#include <OIS/OIS.h>
#include <vector>
#include <map>

namespace Engine {
	class InputService : public Service, public OIS::KeyListener, public OIS::MouseListener {
	public:
		void startup();
		void shutdown();
		
		void tick();
		
		typedef void(^Listener)(const OIS::KeyEvent &arg);
		typedef std::map<OIS::KeyCode, Listener> ListenerList;
		typedef ListenerList::iterator ListenerListHandle;
		
		ListenerListHandle registerKeyDownListener(OIS::KeyCode keyCode, Listener listener);
		ListenerListHandle registerKeyUpListener(OIS::KeyCode keyCode, Listener listener);
		
		void registerMouseListener(OIS::MouseListener *listener) {
			m_mouseListeners.push_back(listener);
		}
		void removeMouseListener(OIS::MouseListener *listener);
		
		bool keyPressed(const OIS::KeyEvent &arg);
		bool keyReleased(const OIS::KeyEvent &arg);
		bool mouseMoved(const OIS::MouseEvent &arg);
		bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID bid);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID bid);
	private:
		std::vector<OIS::MouseListener*> m_mouseListeners;
		ListenerList m_keyDownListenerList;
		ListenerList m_keyUpListenerList;
	};
}

#endif
