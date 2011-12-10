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
#include "Engine/InputService.h"
#include "Engine/Clock.h"
#include <sstream>

namespace Game {
	const unsigned kMinionCount = 100;
	const unsigned kFieldWidth = 200;
	const unsigned kFieldHeight = 300;
	
	void GameService::startup() {
		unsigned rowSize = sqrt(kMinionCount);
		Engine::GameObjectService *gameObjectService = (Engine::GameObjectService*)Engine::ServiceManager::getSingletonPtr()->getService("gameObject");
		
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
			gameObjectService->addGameObject(minion);
			
			m_minions.push_back(minion);
		}

		Ogre::Entity *pMinionEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("player", "ogrehead.mesh");
		Ogre::SceneNode *pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("player");
		pCubeNode->setPosition(0.0, 0.0, 0.0);
		pCubeNode->setScale(0.2, 0.2, 0.2);
		pCubeNode->attachObject(pMinionEntity);
		
		m_player = new Engine::GameObject();
		Engine::Placeable *placeable = m_player->addComponent<Engine::Placeable>();
		placeable->setSceneNode(pCubeNode);
		
		Engine::ObjectTextDisplayComponent *textDisplay = m_player->addComponent<Engine::ObjectTextDisplayComponent>();
		textDisplay->setText("Player");
		
		m_player->setWantsUpdate(true);
		gameObjectService->addGameObject(m_player);
		
		m_moveUp = false;
		m_moveDown = false;
		m_moveLeft = false;
		m_moveRight = false;
		
		Engine::InputService *inputService = (Engine::InputService*)Engine::ServiceManager::getSingletonPtr()->getService("input");
		inputService->registerKeyDownListener(OIS::KC_UP, ^(const OIS::KeyEvent &arg) {
			m_moveUp = true;
		});
		inputService->registerKeyUpListener(OIS::KC_UP, ^(const OIS::KeyEvent &arg) {
			m_moveUp = false;
		});
		inputService->registerKeyDownListener(OIS::KC_DOWN, ^(const OIS::KeyEvent &arg) {
			m_moveDown = true;
		});
		inputService->registerKeyUpListener(OIS::KC_DOWN, ^(const OIS::KeyEvent &arg) {
			m_moveDown = false;
		});
		inputService->registerKeyDownListener(OIS::KC_LEFT, ^(const OIS::KeyEvent &arg) {
			m_moveLeft = true;
		});
		inputService->registerKeyUpListener(OIS::KC_LEFT, ^(const OIS::KeyEvent &arg) {
			m_moveLeft = false;
		});
		inputService->registerKeyDownListener(OIS::KC_RIGHT, ^(const OIS::KeyEvent &arg) {
			m_moveRight = true;
		});
		inputService->registerKeyUpListener(OIS::KC_RIGHT, ^(const OIS::KeyEvent &arg) {
			m_moveRight = false;
		});
	}
	
	void GameService::shutdown() {
		m_minions.clear();
	}
	
	void GameService::tick() {
		if(m_moveUp)
			m_player->getComponent<Engine::Placeable>()->sceneNode()->translate(0.0, 0.0, -10.0 / m_clock->lastIncrement());
		else if(m_moveDown)
			m_player->getComponent<Engine::Placeable>()->sceneNode()->translate(0.0, 0.0, 10.0 / m_clock->lastIncrement());
		if(m_moveLeft)
			m_player->getComponent<Engine::Placeable>()->sceneNode()->translate(-10.0 / m_clock->lastIncrement(), 0.0, 0.0);
		else if(m_moveRight)
			m_player->getComponent<Engine::Placeable>()->sceneNode()->translate(10.0 / m_clock->lastIncrement(), 0.0, 0.0);
	}
}
