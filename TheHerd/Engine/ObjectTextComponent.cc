//
//  ObjectTextComponent.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "ObjectTextComponent.h"
#include "OgrePlatform.h"
#include "OgreFramework.h"
#include <sstream>
#include "ServiceManager.h"
#include "RocketService.h"
#include <Rocket/Core.h>

namespace Engine {
	ObjectTextDisplayComponent::ObjectTextDisplayComponent(GameObject *gameObject) : GameComponent(gameObject) {
		gameObject->props().Set("objectText_text", "");
		
		RocketService *rocketService = (RocketService*)ServiceManager::getSingletonPtr()->getService("rocket");
		
		m_overlay = rocketService->loadDocument("infooverlay.rml");
		m_overlay->Show();
	}

	ObjectTextDisplayComponent::~ObjectTextDisplayComponent() {
		gameObject()->props().Delete("objectText_text");
		m_overlay->RemoveReference();
	}

	void ObjectTextDisplayComponent::setText(const std::string& text) {
		gameObject()->props().Set("objectText_text", text);
		
		m_overlay->SetInnerRML(text.c_str());
	}

	void ObjectTextDisplayComponent::tick() {
		Ogre::Camera *cam = OgreFramework::getSingletonPtr()->m_pCamera;
		Ogre::SceneNode *sceneNode;
		gameObject()->props().Get("sceneNode", &sceneNode);
		
		Ogre::MovableObject *p = sceneNode->getAttachedObject(0); // FIXME?
		
		// get the projection of the object's AABB into screen space
		const Ogre::AxisAlignedBox& bbox = p->getWorldBoundingBox(true);
		Ogre::Matrix4 mat = cam->getViewMatrix();
		
		const Ogre::Vector3* corners = bbox.getAllCorners();
		
		float min_x = 1.0f, max_x = 0.0f, min_y = 1.0f, max_y = 0.0f;
		
		// expand the screen-space bounding-box so that it completely encloses 
		// the object's AABB
		for (int i=0; i<8; i++) {
			Ogre::Vector3 corner = corners[i];
			
			// multiply the AABB corner vertex by the view matrix to 
			// get a camera-space vertex
			corner = mat * corner;
			
			// make 2D relative/normalized coords from the view-space vertex
			// by dividing out the Z (depth) factor -- this is an approximation
			float x = corner.x / corner.z + 0.5;
			float y = corner.y / corner.z + 0.5;
			
			if (x < min_x) 
				min_x = x;
			
			if (x > max_x) 
				max_x = x;
			
			if (y < min_y) 
				min_y = y;
			
			if (y > max_y) 
				max_y = y;
		}
		
		// we now have relative screen-space coords for the object's bounding box; here
		// we need to center the text above the BB on the top edge. The line that defines
		// this top edge is (min_x, min_y) to (max_x, min_y)
		
		const Rocket::Core::Box &box = m_overlay->GetBox();
		
		Ogre::RenderWindow *window = OgreFramework::getSingletonPtr()->m_pRenderWnd;
		m_overlay->SetOffset(Rocket::Core::Vector2f(window->getWidth() - ((max_x + min_x) * .5 * window->getWidth() - box.GetSize().x * .5), min_y * window->getHeight()), NULL);
	}
}
