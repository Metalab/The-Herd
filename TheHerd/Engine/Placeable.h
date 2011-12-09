//
//  Placeable.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_Placeable_h
#define Testing_Placeable_h

#include "GameObject.h"

namespace Ogre {
	class SceneNode;
}

namespace Engine {
	class Placeable : public GameComponent {
	public:
		Placeable(GameObject *gameObject) : GameComponent(gameObject) {}
		
		void setSceneNode(Ogre::SceneNode *sceneNode) {
			gameObject()->props().Set("sceneNode", sceneNode);
		}
		Ogre::SceneNode *sceneNode() {
			Ogre::SceneNode *sceneNode;
			gameObject()->props().Get("sceneNode", &sceneNode);
			return sceneNode;
		}
		void tick() {}
	};
}

#endif
