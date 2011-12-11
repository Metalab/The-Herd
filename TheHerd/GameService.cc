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
#include "Engine/AudioService.h"
#include "Engine/Clock.h"
#include "Engine/RocketService.h"
#include <sstream>
#include "GameConstants.h"
#include "RandomWalkComponent.h"
#include "InteractionComponent.h"
#include "MinionComponent.h"
#include "NPCComponent.h"
#include <Rocket/Core.h>
#include <fmod_event.hpp>
#include "SelectionService.h"

namespace Game {
	void GameService::startup() {
		unsigned rowSize = sqrt(kMinionCount);
		Engine::GameObjectService *gameObjectService = (Engine::GameObjectService*)Engine::ServiceManager::getSingletonPtr()->getService("gameObject");
		
		m_exchangeRate = kFoodExchangeStartup;
		
		Ogre::Entity *pGroundEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("ground", "ground.mesh");
		Ogre::SceneNode *pGroundNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("ground");
		pGroundNode->attachObject(pGroundEntity);
		
		for(unsigned i = 0; i < 10; ++i) {
			std::ostringstream S;
			S << "mushroom_or_1_" << i;
			
			Ogre::Entity *pMushroomEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(S.str(), "mushroom_or_001.mesh");
			Ogre::SceneNode *pMushroomNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(S.str());
			pMushroomNode->attachObject(pMushroomEntity);
			pMushroomNode->setScale(4.0, 4.0, 4.0);
			pMushroomNode->setPosition(kFieldWidth * random()/(float)RAND_MAX - kFieldWidth * .5, 0.0, kFieldHeight * random()/(float)RAND_MAX - kFieldHeight * .5);
			pMushroomNode->setOrientation(Ogre::Quaternion(Ogre::Radian(2.0*M_PI * random() / (float)RAND_MAX), Ogre::Vector3(0.0, 1.0, 0.0)));
		}
		for(unsigned i = 0; i < 10; ++i) {
			std::ostringstream S;
			S << "mushroom_or_2_" << i;
			
			Ogre::Entity *pMushroomEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(S.str(), "mushroom_or_002.mesh");
			Ogre::SceneNode *pMushroomNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(S.str());
			pMushroomNode->attachObject(pMushroomEntity);
			pMushroomNode->setScale(4.0, 4.0, 4.0);
			pMushroomNode->setPosition(kFieldWidth * random()/(float)RAND_MAX - kFieldWidth * .5, 0.0, kFieldHeight * random()/(float)RAND_MAX - kFieldHeight * .5);
			pMushroomNode->setOrientation(Ogre::Quaternion(Ogre::Radian(2.0*M_PI * random() / (float)RAND_MAX), Ogre::Vector3(0.0, 1.0, 0.0)));
		}

		for(unsigned i = 0; i < 10; ++i) {
			std::ostringstream S;
			S << "mushroom_red_1_" << i;
			
			Ogre::Entity *pMushroomEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(S.str(), "mushroom_red_001.mesh");
			Ogre::SceneNode *pMushroomNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(S.str());
			pMushroomNode->attachObject(pMushroomEntity);
			pMushroomNode->setScale(4.0, 4.0, 4.0);
			pMushroomNode->setPosition(kFieldWidth * random()/(float)RAND_MAX - kFieldWidth * .5, 0.0, kFieldHeight * random()/(float)RAND_MAX - kFieldHeight * .5);
			pMushroomNode->setOrientation(Ogre::Quaternion(Ogre::Radian(2.0*M_PI * random() / (float)RAND_MAX), Ogre::Vector3(0.0, 1.0, 0.0)));
		}
		for(unsigned i = 0; i < 10; ++i) {
			std::ostringstream S;
			S << "mushroom_red_2_" << i;
			
			Ogre::Entity *pMushroomEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(S.str(), "mushroom_red_002.mesh");
			Ogre::SceneNode *pMushroomNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(S.str());
			pMushroomNode->attachObject(pMushroomEntity);
			pMushroomNode->setScale(4.0, 4.0, 4.0);
			pMushroomNode->setPosition(kFieldWidth * random()/(float)RAND_MAX - kFieldWidth * .5, 0.0, kFieldHeight * random()/(float)RAND_MAX - kFieldHeight * .5);
			pMushroomNode->setOrientation(Ogre::Quaternion(Ogre::Radian(2.0*M_PI * random() / (float)RAND_MAX), Ogre::Vector3(0.0, 1.0, 0.0)));
		}
		Ogre::Entity *pLogEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("log", "log_big_001.mesh");
		Ogre::SceneNode *pLogNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("log");
		pLogNode->attachObject(pLogEntity);
		pLogNode->setScale(4.0, 4.0, 4.0);
		pLogNode->setPosition(0.0, 3.0, -(kFieldHeight * .5 + 8.0));
		pLogNode->setOrientation(Ogre::Quaternion(Ogre::Radian(M_PI_2), Ogre::Vector3(0.0, 0.0, 1.0)) * Ogre::Quaternion(Ogre::Radian(M_PI_4), Ogre::Vector3(1.0, 0.0, 0.0)));
		
		int totalMoney = 0;
		float totalLife = 0.0;
		
		for(unsigned i = 0; i < kMinionCount; ++i) {
			unsigned row = i % rowSize;
			unsigned col = i / rowSize;
			
			std::ostringstream S;
			S << "Minion (" << row << ", " << col << ")";
			
			Ogre::Entity *pMinionEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(S.str(), "sheep_cute.mesh");
			Ogre::SceneNode *pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(S.str());
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
			
			int money = random() % kMoneyMaxStartup;
			minion->props().Set("money", money);
			float life = kLifeMinStartup + (1.0 - kLifeMinStartup) * random() / (float)RAND_MAX;
			minion->props().Set("life", life);
			
			totalMoney += money;
			totalLife += life;

			minion->setWantsUpdate(true);
			gameObjectService->addGameObject(minion);

			Ogre::Vector3 pos(Ogre::Vector3((row / (float)rowSize - 0.5) * kFieldWidth, 0.0, (col / (float)rowSize - 0.5) * kFieldHeight));
			placeable->setPosition(pos);
			
			minion->addComponent<Game::RandomWalkComponent>()->setClock(m_clock);
			minion->addComponent<Game::InteractionComponent>();
			minion->addComponent<Game::MinionComponent>();
			minion->addComponent<Game::NPCComponent>();
			
			m_minions.push_back(minion);
		}

		Ogre::Entity *pMinionEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("player", "sheep_cute.mesh");
		Ogre::SceneNode *pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("player");
		pCubeNode->attachObject(pMinionEntity);
		
		m_player = new Engine::GameObject();
		Engine::Placeable *placeable = m_player->addComponent<Engine::Placeable>();
		placeable->setSceneNode(pCubeNode);
		m_player->addComponent<Game::MinionComponent>();
		m_player->addComponent<Game::InteractionComponent>();
		
		Engine::ObjectTextDisplayComponent *textDisplay = m_player->addComponent<Engine::ObjectTextDisplayComponent>();
		
		textDisplay->setText(m_playerName);
		
		// the player is total average
		int money = totalMoney / kMinionCount;
		m_player->props().Set("money", money);
		float life = totalLife / kMinionCount;
		m_player->props().Set("life", life);
		
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
		inputService->registerKeyDownListener(OIS::KC_W, ^(const OIS::KeyEvent &arg) {
			m_moveUp = true;
		});
		inputService->registerKeyUpListener(OIS::KC_UP, ^(const OIS::KeyEvent &arg) {
			m_moveUp = false;
		});
		inputService->registerKeyUpListener(OIS::KC_W, ^(const OIS::KeyEvent &arg) {
			m_moveUp = false;
		});
		inputService->registerKeyDownListener(OIS::KC_DOWN, ^(const OIS::KeyEvent &arg) {
			m_moveDown = true;
		});
		inputService->registerKeyDownListener(OIS::KC_S, ^(const OIS::KeyEvent &arg) {
			m_moveDown = true;
		});
		inputService->registerKeyUpListener(OIS::KC_DOWN, ^(const OIS::KeyEvent &arg) {
			m_moveDown = false;
		});
		inputService->registerKeyUpListener(OIS::KC_S, ^(const OIS::KeyEvent &arg) {
			m_moveDown = false;
		});
		inputService->registerKeyDownListener(OIS::KC_LEFT, ^(const OIS::KeyEvent &arg) {
			m_moveLeft = true;
		});
		inputService->registerKeyDownListener(OIS::KC_A, ^(const OIS::KeyEvent &arg) {
			m_moveLeft = true;
		});
		inputService->registerKeyUpListener(OIS::KC_LEFT, ^(const OIS::KeyEvent &arg) {
			m_moveLeft = false;
		});
		inputService->registerKeyUpListener(OIS::KC_A, ^(const OIS::KeyEvent &arg) {
			m_moveLeft = false;
		});
		inputService->registerKeyDownListener(OIS::KC_RIGHT, ^(const OIS::KeyEvent &arg) {
			m_moveRight = true;
		});
		inputService->registerKeyDownListener(OIS::KC_D, ^(const OIS::KeyEvent &arg) {
			m_moveRight = true;
		});
		inputService->registerKeyUpListener(OIS::KC_RIGHT, ^(const OIS::KeyEvent &arg) {
			m_moveRight = false;
		});
		inputService->registerKeyUpListener(OIS::KC_D, ^(const OIS::KeyEvent &arg) {
			m_moveRight = false;
		});
		
		m_playerHud = ((Engine::RocketService*)Engine::ServiceManager::getSingletonPtr()->getService("rocket"))->loadDocument("playerhud.rml");
		
		// inject constants from the code
		std::ostringstream S;
		S << 100.0 * kMoneyThresholdAttack / kMoneyThresholdPolice << "%";
		m_playerHud->GetElementById("hobolimit")->SetProperty("bottom", S.str().c_str());
		S.str("");
		S << 100.0 * kMoneyThresholdOccupy / kMoneyThresholdPolice << "%";
		m_playerHud->GetElementById("bankerlimit")->SetProperty("bottom", S.str().c_str());
		
		m_playerHud->Show();
		
		// audio
		
		Engine::AudioService *audioService = ((Engine::AudioService*)Engine::ServiceManager::getSingletonPtr()->getService("audio"));
		audioService->loadBank("theherd.fev");
		m_music = audioService->getEvent("theherd/TheHerd/music");
		m_music->start();
		m_walk = audioService->getEvent("theherd/TheHerd/walk");
		m_endSound = audioService->getEvent("theherd/TheHerd/end");
	}
	
	void GameService::shutdown() {
		m_walk->stop();
		m_music->stop();
		m_playerHud->RemoveReference();
		m_minions.clear();
	}
	
	void GameService::tick() {
		if(m_end)
			return;
		// new exchange rate for food
		m_exchangeRate = kFoodExchangeStartup - m_clock->offset() * kInflation;
		
		if(m_moveUp || m_moveDown || m_moveLeft || m_moveRight) {
			Engine::Placeable *placeable = m_player->getComponent<Engine::Placeable>();
			Ogre::Vector3 offset = Ogre::Vector3::ZERO;

			if(m_moveUp)
				offset = Ogre::Vector3(0.0, 0.0, -1.0);
			else if(m_moveDown)
				offset = Ogre::Vector3(0.0, 0.0, 1.0);
			if(m_moveLeft)
				offset += Ogre::Vector3(-1.0, 0.0, 0.0);
			else if(m_moveRight)
				offset += Ogre::Vector3(1.0, 0.0, 0.0);
			
			offset.normalise();
			
			placeable->sceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
			Ogre::Vector3 position = placeable->position() + offset * kMinionWalkSpeed * m_clock->lastIncrement();

			if(position.x < -(kFieldWidth * .5))
				position.x = -(kFieldWidth * .5);
			else if(position.x > (kFieldWidth * .5))
				position.x = (kFieldWidth * .5);
			if(position.z < -(kFieldHeight * .5))
				position.z = -(kFieldHeight * .5);
			else if(position.z > (kFieldHeight * .5))
				position.z = (kFieldHeight * .5);
			
			placeable->setPosition(position);
			placeable->sceneNode()->setDirection(offset);
			
			OgreFramework::getSingletonPtr()->m_pCamera->setPosition(position + Ogre::Vector3(0.0, 40.0, 40.0));
			
			if(!m_wasWalking) {
				m_walk->start();
				m_wasWalking = true;
			}
		} else if(m_wasWalking) {
			m_walk->stop();
			m_wasWalking = false;
		}
		
		// check for dead minions
		std::vector<std::list<Engine::GameObject*>::iterator> deadMinions;
		for(std::list<Engine::GameObject*>::iterator iter = m_minions.begin(); iter != m_minions.end(); ++iter) {
			MinionComponent *minionComponent = (*iter)->getComponent<MinionComponent>();
			if(minionComponent && minionComponent->life() <= 0.0)
				deadMinions.push_back(iter);
		}
		
		MinionComponent *playerMinionComponent = m_player->getComponent<MinionComponent>();
		Engine::GameObjectService *gameObjectService = (Engine::GameObjectService*)Engine::ServiceManager::getSingletonPtr()->getService("gameObject");		
		if(playerMinionComponent->life() <= 0.0) {
			// end condition reached
			Rocket::Core::ElementDocument *enddoc = ((Engine::RocketService*)Engine::ServiceManager::getSingletonPtr()->getService("rocket"))->loadDocument("dead.rml");
			enddoc->Show();
			enddoc->RemoveReference();
			m_playerHud->Hide();
			m_clock->setScale(0.0);
			m_end = true;
			
			m_music->stop();
			m_endSound->start();
			
			gameObjectService->setPaused(true);
		}

		SelectionService *selectionService = (SelectionService*)Engine::ServiceManager::getSingletonPtr()->getService("selection");
		for(std::vector<std::list<Engine::GameObject*>::iterator>::iterator iter = deadMinions.begin(); iter != deadMinions.end(); ++iter) {
			// check whether this minion is a stakeholder for someone
			for(std::list<Engine::GameObject*>::iterator iter2 = m_minions.begin(); iter2 != m_minions.end(); ++iter2) {
				MinionComponent *minionComponent = (*iter2)->getComponent<MinionComponent>();
				if(minionComponent) {
					Engine::GameObject *stakeholder = minionComponent->stakeHolder();
					if(stakeholder == **iter)
						minionComponent->stakeholderDied();
				}
			}
			if(playerMinionComponent->stakeHolder() == **iter)
				playerMinionComponent->stakeholderDied();
			
			selectionService->minionDied(**iter);
			
			gameObjectService->removeGameObject(**iter);
			m_minions.erase(*iter);
		}
		
		// update HUD
		float life = playerMinionComponent->life();
		Rocket::Core::Element *lifeBar = m_playerHud->GetElementById("life");
		
		std::ostringstream S;
		S << life*100.0 << "%";
		lifeBar->SetProperty("height", S.str().c_str());
		lifeBar->SetProperty("background-color", (life<.3)?"#f00a":(life<.6)?"#ff0a":"#0f0a");
		
		int money = playerMinionComponent->money();
		
		S.str("");
		S << "$" << money;
		m_playerHud->GetElementById("money")->SetInnerRML(S.str().c_str());
		S.str("");
		S << MIN(100.0, 100.0 * money / kMoneyThresholdPolice) << "%";
		m_playerHud->GetElementById("moneyBar")->SetProperty("height", S.str().c_str());
		
		unsigned rank = 1;
		int rank1money = 0;
		Engine::GameObject *rank1 = NULL;
		for(std::list<Engine::GameObject*>::iterator iter = m_minions.begin(); iter != m_minions.end(); ++iter) {
			int minionMoney = (*iter)->getComponent<MinionComponent>()->money();
			if(minionMoney > money)
				++rank;
			if(minionMoney > rank1money) {
				rank1money = minionMoney;
				rank1 = *iter;
			}
		}
		S.str("");
		S << rank;
		m_playerHud->GetElementById("rank")->SetInnerRML(S.str().c_str());
		
		if(rank1) {
			S.str("");
			S << rank1->name() << " ($" << rank1money << ")";
			m_playerHud->GetElementById("rank1")->SetInnerRML(S.str().c_str());
		} else
			m_playerHud->GetElementById("rank1")->SetInnerRML("YOU");
		
		Engine::GameObject *stakeholder = playerMinionComponent->stakeHolder();
		if(stakeholder) {
			unsigned count = playerMinionComponent->stakecount();
			int stakeholderMoney = stakeholder->getComponent<MinionComponent>()->money();
			S.str("");
			S << count << "x " << stakeholder->name() << " ($" << stakeholderMoney << ")";
			
			m_playerHud->GetElementById("stakeholder")->SetInnerRML(S.str().c_str());
			m_playerHud->GetElementById("stakeholder-info")->SetProperty("display", "block");
		} else
			m_playerHud->GetElementById("stakeholder-info")->SetProperty("display", "none");
		
		S.str("");
		S << (int)(100.0 * m_exchangeRate / kFoodExchangeStartup) << "%";
		m_playerHud->GetElementById("food")->SetInnerRML(S.str().c_str());
		
		if(m_exchangeRate < 0.0) {
			// end condition reached
			Rocket::Core::ElementDocument *enddoc = ((Engine::RocketService*)Engine::ServiceManager::getSingletonPtr()->getService("rocket"))->loadDocument((rank==1)?"win.rml":"lose.rml");
			enddoc->Show();
			enddoc->RemoveReference();
			m_playerHud->Hide();
			m_clock->setScale(0.0);
			m_end = true;
			
			m_music->stop();
			m_endSound->start();
			
			gameObjectService->setPaused(true);
		}
	}
}
