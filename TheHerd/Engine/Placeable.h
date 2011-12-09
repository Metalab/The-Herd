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
	class Placeable : public Component {
	public:
		void setSceneNode(Ogre::SceneNode *sceneNode) {
			m_sceneNode = sceneNode;
		}
		Ogre::SceneNode *sceneNode() {
			return m_sceneNode;
		}
	private:
		Ogre::SceneNode *m_sceneNode;
	};
}

#endif
