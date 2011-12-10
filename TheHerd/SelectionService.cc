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
#include "Engine/RocketService.h"
#include "OgreFramework.h"
#include "GameService.h"
#include "Engine/GameObject.h"
#include "MinionComponent.h"
#include "Engine/Clock.h"
#include <sstream>
#include <Rocket/Core.h>
#include "InteractionComponent.h"

namespace Game {
	void SelectionService::startup() {
		m_inMenu = false;
		static_cast<Engine::InputService*>(Engine::ServiceManager::getSingletonPtr()->getService("input"))->registerMouseListener(this);
		
		m_actionMenu = static_cast<Engine::RocketService*>(Engine::ServiceManager::getSingletonPtr()->getService("rocket"))->loadDocument("actionmenu.rml");
		target = NULL;
	}
	
	void SelectionService::shutdown() {
		static_cast<Engine::InputService*>(Engine::ServiceManager::getSingletonPtr()->getService("input"))->removeMouseListener(this);
		m_actionMenu->RemoveReference();
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
					target = gameObject;
					
					m_actionMenu->GetElementById("name")->SetInnerRML(gameObject->name().c_str());
					
					std::ostringstream S;
					S << "$" << minionComponent->money();
					m_actionMenu->GetElementById("money")->SetInnerRML(S.str().c_str());
					m_actionMenu->Show();
					Rocket::Core::Box box = m_actionMenu->GetBox();
					m_actionMenu->SetOffset(Rocket::Core::Vector2f(e.state.X.abs - box.GetSize().x * 0.5, e.state.Y.abs - box.GetSize().y * 0.5), NULL);
				}
			}
		}
		
		of->m_pSceneMgr->destroyQuery(query);
		return false;
	}
	
	bool SelectionService::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
		if(m_inMenu) {
			m_inMenu = false;
			
			Rocket::Core::Vector2f mousepos(e.state.X.abs, e.state.Y.abs);
			
			GameService *gameService = (GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game");
			InteractionComponent *interactionComponent = gameService->player()->getComponent<InteractionComponent>();
			
			// determine where the player has lifted the mouse
			if(m_actionMenu->GetElementById("trade")->IsPointWithinElement(mousepos))
				interactionComponent->trade(target);
			else if(m_actionMenu->GetElementById("attack")->IsPointWithinElement(mousepos)) {
				interactionComponent->attack(target); // TODO: police
			} else if(m_actionMenu->GetElementById("occupy")->IsPointWithinElement(mousepos))
				interactionComponent->occupy(target);
			else if(m_actionMenu->GetElementById("repay")->IsPointWithinElement(mousepos))
				interactionComponent->repay(target);
			
			m_actionMenu->Hide();
			gameService->clock()->setScale(1.0);
			
			target = NULL;
			return true;
		}
		
		return false;
	}
}
