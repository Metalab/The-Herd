//
//  RandomWalkComponent.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "RandomWalkComponent.h"
#include "GameConstants.h"
#include "Engine/Placeable.h"
#include "Engine/ServiceManager.h"
#include "Engine/OgreService.h"
#include "OgreFramework.h"
#include "Engine/Clock.h"
#include "NPCComponent.h"

namespace Game {
	RandomWalkComponent::RandomWalkComponent(Engine::GameObject *gameObject) : GameComponent(gameObject) {
		m_direction = Ogre::Vector2((random() / (float)RAND_MAX) * 2.0 - 1.0,
									(random() / (float)RAND_MAX) * 2.0 - 1.0);
		m_direction.normalise();
		
		m_realDirection = m_direction;
	}

	void RandomWalkComponent::tick() {
		if(!m_clock)
			return;
		Engine::Placeable *placeable = gameObject()->getComponent<Engine::Placeable>();
		
		Ogre::Vector3 position = placeable->position();
		Ogre::Vector3 newPosition = position + Ogre::Vector3(m_realDirection.x, 0.0, m_realDirection.y) * m_clock->lastIncrement() * kMinionWalkSpeed;
		
		if(newPosition.x < -(kFieldWidth * .5) || newPosition.x > (kFieldWidth * .5) || newPosition.z < -(kFieldHeight * .5) || newPosition.z > (kFieldHeight * .5)) {
			m_direction = Ogre::Vector2(-position.x, -position.z);
			m_direction.normalise();
		}
		
		m_realDirection = m_realDirection * .6 + m_direction * .4;
		
		placeable->sceneNode()->setDirection(m_realDirection.x, 0.0, m_realDirection.y, Ogre::Node::TS_WORLD);
		placeable->setPosition(newPosition);
		
		// check for colissions with the border and other entities, and turn randomly
		Engine::OgreService *ogreService = (Engine::OgreService*)Engine::ServiceManager::getSingletonPtr()->getService("ogre");
		
		std::vector<Engine::GameObject*> colliders = ogreService->sphereQuery(gameObject(), kMinionPrivacyRadius);
		if(colliders.size() > 0) {
			// defer interaction to another component
			NPCComponent *npcComponent = gameObject()->getComponent<NPCComponent>();
			if(npcComponent)
				for(std::vector<Engine::GameObject*>::iterator iter = colliders.begin(); iter != colliders.end(); ++iter)
					npcComponent->interactWith(*iter);
			
			std::vector<Ogre::Vector2> resultDirections;
			for(std::vector<Engine::GameObject*>::iterator iter = colliders.begin(); iter != colliders.end(); ++iter) {
				Ogre::Vector3 otherPosition = (*iter)->getComponent<Engine::Placeable>()->position();
				Ogre::Vector2 difference(otherPosition.x - position.x, otherPosition.z - position.z);
				difference.normalise();
				
				resultDirections.push_back(difference);
			}
			
			// calculate average of all angles
			float angle = 0.0;
			for(std::vector<Ogre::Vector2>::iterator iter = resultDirections.begin(); iter != resultDirections.end(); ++iter) {
				float rAngle = acosf(iter->x);
				if(iter->x < 0.0) // more than 180°?
					rAngle = 2.0 * M_PI - rAngle;
				angle += rAngle;
			}
			angle /= resultDirections.size();
			
			angle += M_PI; // walk into the other direction

			m_direction.x = cosf(angle);
			m_direction.y = sinf(angle);
		}
		
		// try to steer towards the center
		
		Ogre::Vector2 centerDirection(-newPosition.x, -newPosition.z);
		centerDirection.normalise();
		m_direction = m_direction * .9 + centerDirection * .1;
		m_direction.normalise();
	}
}
