//
//  Placeable.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "Placeable.h"
#include "GameObject.h"
#include <Ogre/OgreVector3.h>
#include <Ogre/OgreSceneNode.h>
#include <iostream>

namespace Engine {
	Placeable::Placeable(GameObject *gameObject) : GameComponent(gameObject) {
		gameObject->props().Set("position", Ogre::Vector3(0.0, 0.0, 0.0));
	}
	
	Placeable::~Placeable() {
		gameObject()->props().Delete("sceneNode");
	}
	
	void Placeable::setSceneNode(Ogre::SceneNode *sceneNode) {
		gameObject()->props().Set("sceneNode", sceneNode);
		Ogre::UserObjectBindings &objectBindings = sceneNode->getUserObjectBindings();
		Ogre::Any any(gameObject());
		objectBindings.setUserAny("GameObject", any);
	}
	
	void Placeable::setPosition(const Ogre::Vector3 &pos) {
		gameObject()->props().Set("position", pos);
		
		sceneNode()->setPosition(pos);
	}
	
	const Ogre::Vector3 &Placeable::position() {
		return sceneNode()->getPosition();
	}

}
