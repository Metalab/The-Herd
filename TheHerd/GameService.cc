//
//  GameService.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "GameService.h"
#include "OgrePlatform.h"
#include "OgreFramework.h"
#include <Ogre/OgreEntity.h>
#include "Engine/ObjectTextComponent.h"
#include "Engine/Placeable.h"
#include "Engine/ServiceManager.h"
#include "Engine/GameObjectService.h"
#include <sstream>

namespace Game {
	const unsigned kMinionCount = 100;
	const unsigned kFieldWidth = 200;
	const unsigned kFieldHeight = 300;
	
	void GameService::startup() {
		unsigned rowSize = sqrt(kMinionCount);
		
		for(unsigned i = 0; i < kMinionCount; ++i) {
			unsigned row = i % rowSize;
			unsigned col = i / rowSize;
			
			std::ostringstream S;
			S << "Minion (" << row << ", " << col << ")";
			
			Ogre::Entity *pMinionEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(S.str(), "ogrehead.mesh");
			Ogre::SceneNode *pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(S.str());
			pCubeNode->setPosition((row / (float)rowSize - 0.5) * kFieldWidth, 0.0, (col / (float)rowSize - 0.5) * kFieldHeight);
			pCubeNode->setScale(0.1, 0.1, 0.1);
			pCubeNode->attachObject(pMinionEntity);
			
			Engine::GameObject *minion = new Engine::GameObject();
			Engine::Placeable *placeable = minion->addComponent<Engine::Placeable>();
			placeable->setSceneNode(pCubeNode);
			
			Engine::ObjectTextDisplayComponent *textDisplay = minion->addComponent<Engine::ObjectTextDisplayComponent>();
			textDisplay->setText(S.str());
			
			minion->setWantsUpdate(true);
			
			Engine::GameObjectService *gameObjectService = (Engine::GameObjectService*)Engine::ServiceManager::getSingletonPtr()->getService("gameObject");
			
			gameObjectService->addGameObject(minion);
			
			m_minions.push_back(minion);
		}
	}
	
	void GameService::shutdown() {
		m_minions.clear();
	}
	
	void GameService::tick() {
		
	}
}