//
//  SelectionService.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "SelectionService.h"
#include "Engine/ServiceManager.h"
#include "Engine/InputService.h"
#include "OgreFramework.h"
#include "GameService.h"
#include "Engine/GameObject.h"
#include "MinionComponent.h"
#include "Engine/Clock.h"
#include <sstream>

namespace Game {
	void SelectionService::startup() {
		m_inMenu = false;
		static_cast<Engine::InputService*>(Engine::ServiceManager::getSingletonPtr()->getService("input"))->registerMouseListener(this);
	}
	
	void SelectionService::shutdown() {
		static_cast<Engine::InputService*>(Engine::ServiceManager::getSingletonPtr()->getService("input"))->removeMouseListener(this);
	}
	
	void SelectionService::tick() {
	}
	
	bool SelectionService::mouseMoved(const OIS::MouseEvent& e) {
		return false;
	}
	
	bool SelectionService::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
		OgreFramework *of = OgreFramework::getSingletonPtr();
		int width = of->m_pRenderWnd->getWidth();
		int height = of->m_pRenderWnd->getHeight();
		
		Ogre::Ray ray;
		of->m_pCamera->getCameraToViewportRay(e.state.X.abs / (float)width, e.state.Y.abs / (float)height, &ray);
		Ogre::RaySceneQuery *query = of->m_pSceneMgr->createRayQuery(ray);
		query->setSortByDistance(true, 1); // only get nearest result
		Ogre::RaySceneQueryResult &result = query->execute();
		
		if(result.size() == 0)
			return false;
		
		Ogre::RaySceneQueryResultEntry &entry = result[0];
		Ogre::SceneNode *resultSceneNode = entry.movable->getParentSceneNode();
		
		Ogre::UserObjectBindings &bindings = resultSceneNode->getUserObjectBindings();
		Ogre::Any queryGameObjectAny = bindings.getUserAny("GameObject");
		if(!queryGameObjectAny.isEmpty()) {
			Engine::GameObject *gameObject = Ogre::any_cast<Engine::GameObject*>(queryGameObjectAny);
			GameService *gameService = (GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game");
			if(gameService->player() != gameObject) { // can't do anything with ourselves
				MinionComponent *minionComponent = gameObject->getComponent<MinionComponent>();
				if(minionComponent) {
					// yes, it's actually a minion!
					gameService->clock()->setScale(0.5);
					m_inMenu = true;
					
					OgreFramework::getSingletonPtr()->m_pLog->logMessage("Click on " + gameObject->name());
				}
			}
		}
		
		of->m_pSceneMgr->destroyQuery(query);
		return false;
	}
	
	bool SelectionService::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
		if(m_inMenu) {
			m_inMenu = false;
			GameService *gameService = (GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game");
			gameService->clock()->setScale(1.0);
		}
		
		return false;
	}
}
