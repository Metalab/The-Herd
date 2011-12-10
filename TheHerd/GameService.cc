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
#include "Engine/ObjectOverlayComponent.h"
#include "Engine/Placeable.h"
#include "Engine/ServiceManager.h"
#include "Engine/GameObjectService.h"
#include "Engine/InputService.h"
#include "Engine/Clock.h"
#include <sstream>
#include "GameConstants.h"
#include "RandomWalkComponent.h"
#include "InteractionComponent.h"
#include <Rocket/Core.h>

namespace Game {
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
			pCubeNode->setScale(0.1, 0.1, 0.1);
			pCubeNode->attachObject(pMinionEntity);
			
			Engine::GameObject *minion = new Engine::GameObject();
			Engine::Placeable *placeable = minion->addComponent<Engine::Placeable>();
			placeable->setSceneNode(pCubeNode);
			
			std::string name = m_nameGenerator();
			minion->setName(name);
			
			Engine::ObjectOverlayComponent *overlayComponent = minion->addComponent<Engine::ObjectOverlayComponent>();
			overlayComponent->setDocumentName("minioninfo.rml");
			
			Rocket::Core::ElementDocument *doc = overlayComponent->document();
			doc->GetElementById("name")->SetInnerRML(name.c_str());
			doc->GetElementById("life")->SetInnerRML("100%");
			doc->GetElementById("money")->SetInnerRML("$100");
			
			minion->setWantsUpdate(true);
			gameObjectService->addGameObject(minion);

			Ogre::Vector3 pos(Ogre::Vector3((row / (float)rowSize - 0.5) * kFieldWidth, 0.0, (col / (float)rowSize - 0.5) * kFieldHeight));
			placeable->setPosition(pos);
			
			minion->addComponent<Game::RandomWalkComponent>()->setClock(m_clock);
			minion->addComponent<Game::InteractionComponent>();
			minion->setWantsUpdate(true);
			
			m_minions.push_back(minion);
		}

		Ogre::Entity *pMinionEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("player", "ogrehead.mesh");
		Ogre::SceneNode *pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("player");
		pCubeNode->setScale(0.1, 0.1, 0.1);
		pCubeNode->attachObject(pMinionEntity);
		
		m_player = new Engine::GameObject();
		Engine::Placeable *placeable = m_player->addComponent<Engine::Placeable>();
		placeable->setSceneNode(pCubeNode);
		
		Engine::ObjectTextDisplayComponent *textDisplay = m_player->addComponent<Engine::ObjectTextDisplayComponent>();
		
		textDisplay->setText(m_playerName);
		
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
		if(m_moveUp || m_moveDown || m_moveLeft || m_moveRight) {
			Engine::Placeable *placeable = m_player->getComponent<Engine::Placeable>();
			Ogre::Vector3 offset = Ogre::Vector3::ZERO;

			if(m_moveUp)
				offset = Ogre::Vector3(0.0, 0.0, -10.0 / m_clock->lastIncrement());
			else if(m_moveDown)
				offset = Ogre::Vector3(0.0, 0.0, 10.0 / m_clock->lastIncrement());
			if(m_moveLeft)
				offset += Ogre::Vector3(-10.0 / m_clock->lastIncrement(), 0.0, 0.0);
			else if(m_moveRight)
				offset += Ogre::Vector3(10.0 / m_clock->lastIncrement(), 0.0, 0.0);
			
			placeable->sceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
			Ogre::Vector3 position = placeable->position() + offset;

			if(position.x < -(kFieldWidth * .5))
				position.x = -(kFieldWidth * .5);
			else if(position.x > (kFieldWidth * .5))
				position.x = (kFieldWidth * .5);
			if(position.z < -(kFieldHeight * .5))
				position.z = -(kFieldHeight * .5);
			else if(position.z > (kFieldHeight * .5))
				position.z = (kFieldHeight * .5);
			
			placeable->setPosition(position);
			placeable->sceneNode()->setDirection(-offset);
			
			OgreFramework::getSingletonPtr()->m_pCamera->setPosition(position + Ogre::Vector3(0.0, 40.0, 40.0));
		}
	}
}
