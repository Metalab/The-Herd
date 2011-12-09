//
//  InputService.cc
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "InputService.h"
#include "OgreFramework.h"
#include "ServiceManager.h"

namespace Engine {
	void InputService::tick() {
		OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
		OgreFramework::getSingletonPtr()->m_pMouse->capture();
	}
	
	void InputService::startup() {
	}
	
	void InputService::shutdown() {
	}
	
	void InputService::removeMouseListener(OIS::MouseListener *listener) {
		for(std::vector<OIS::MouseListener*>::iterator iter = m_mouseListeners.begin(); iter != m_mouseListeners.end(); ++iter)
			if(*iter == listener) {
				m_mouseListeners.erase(iter);
				return;
			}
	}
	
	bool InputService::keyPressed(const OIS::KeyEvent &arg) {
		if(arg.key == OIS::KC_ESCAPE) {
			ServiceManager::getSingletonPtr()->terminate();
			return true;
		}
		
		if(OgreFramework::getSingletonPtr()->keyPressed(arg))
			return true;

		return false;
	}
	
	bool InputService::keyReleased(const OIS::KeyEvent &arg) {
		if(OgreFramework::getSingletonPtr()->keyReleased(arg))
			return true;
		return false;
	}
	
	bool InputService::mouseMoved(const OIS::MouseEvent &arg) {
		for(std::vector<OIS::MouseListener*>::iterator iter = m_mouseListeners.begin(); iter != m_mouseListeners.end(); ++iter)
			if((*iter)->mouseMoved(arg))
				return true;
		return false;
	}
	
	bool InputService::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID bid) {
		for(std::vector<OIS::MouseListener*>::iterator iter = m_mouseListeners.begin(); iter != m_mouseListeners.end(); ++iter)
			if((*iter)->mousePressed(arg, bid))
				return true;
		return false;
	}
	
	bool InputService::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID bid) {
		for(std::vector<OIS::MouseListener*>::iterator iter = m_mouseListeners.begin(); iter != m_mouseListeners.end(); ++iter)
			if((*iter)->mouseReleased(arg, bid))
				return true;
		return false;
	}
}
