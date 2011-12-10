//
//  ObjectOverlayComponent.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "ObjectOverlayComponent.h"
#include "OgrePlatform.h"
#include "OgreFramework.h"
#include "ServiceManager.h"
#include "Placeable.h"
#include "RocketService.h"
#include <Rocket/Core.h>

namespace Engine {
	ObjectOverlayComponent::~ObjectOverlayComponent() {
		if(m_overlay)
			m_overlay->RemoveReference();
	}
	
	void ObjectOverlayComponent::setDocumentName(const std::string &doc) {
		RocketService *rocketService = (RocketService*)ServiceManager::getSingletonPtr()->getService("rocket");
		
		m_overlay = rocketService->loadDocument(doc);
		m_overlay->Show();
	}
	
	void ObjectOverlayComponent::tick() {
		if(!m_overlay)
			return;
		
		Ogre::Camera *cam = OgreFramework::getSingletonPtr()->m_pCamera;
		Ogre::SceneNode *sceneNode = gameObject()->getComponent<Placeable>()->sceneNode();
		Ogre::MovableObject *p = sceneNode->getAttachedObject(0); // FIXME?
		
		// get the projection of the object's AABB into screen space
		const Ogre::AxisAlignedBox& bbox = p->getWorldBoundingBox(true);
		Ogre::Matrix4 mat = cam->getProjectionMatrix() * cam->getViewMatrix();
		
		const Ogre::Vector3* corners = bbox.getAllCorners();
		
		float min_x = 1.0f, max_x = -1.0f, min_y = 1.0f, max_y = -1.0f;
		
		// expand the screen-space bounding-box so that it completely encloses 
		// the object's AABB
		for (int i=0; i<8; i++) {
			Ogre::Vector3 corner = corners[i];
			
			// multiply the AABB corner vertex by the view matrix to 
			// get a camera-space vertex
			corner = mat * corner;
			
			// make 2D relative/normalized coords from the view-space vertex
			// by dividing out the Z (depth) factor -- this is an approximation
			float x = corner.x / corner.z;
			float y = corner.y / corner.z;
			
			if (x < min_x) 
				min_x = x;
			
			if (x > max_x) 
				max_x = x;
			
			if (y < min_y) 
				min_y = y;
			
			if (y > max_y) 
				max_y = y;
		}
		float x = (max_x + min_x) * .5;
		float y = max_y;
		
		// we now have relative screen-space coords for the object's bounding box; here
		// we need to center the text above the BB on the top edge. The line that defines
		// this top edge is (min_x, min_y) to (max_x, min_y)
		
		const Rocket::Core::Box &box = m_overlay->GetBox();
		
		Ogre::RenderWindow *window = OgreFramework::getSingletonPtr()->m_pRenderWnd;
		
		min_x = (x * .5 + .5) * window->getWidth() - box.GetSize().x * .5;
		max_x = (x * .5 + .5) * window->getWidth() + box.GetSize().x * .5;
		min_y = (.5 - y * .5) * window->getHeight() - box.GetSize().y * .5;
		max_y = (.5 - y * .5) * window->getHeight() + box.GetSize().y * .5;
		
		if(max_x < 0.0 || min_x > window->getWidth() || max_y < 0.0 || max_y > window->getHeight()) {
			m_overlay->Hide();
		} else {
			m_overlay->Show();
			m_overlay->SetOffset(Rocket::Core::Vector2f(min_x, min_y), NULL);
		}
	}
}