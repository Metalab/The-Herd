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

namespace Engine {
	ObjectTextDisplayComponent::ObjectTextDisplayComponent(GameObject *gameObject) : GameComponent(gameObject) {
		gameObject->props().Set("objectText_text", "");
		
		// create an overlay that we can use for later
		Ogre::Overlay *pOverlay = Ogre::OverlayManager::getSingleton().create("shapeName");
		gameObject->props().Set("objectText_overlay", pOverlay);
		Ogre::OverlayContainer *pContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "container1"));
		gameObject->props().Set("objectText_container", pContainer);
		
		pOverlay->add2D(pContainer);
		
		Ogre::OverlayElement *pText = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "shapeNameText");
		gameObject->props().Set("objectText_textElement", pText);
		pText->setDimensions(1.0, 1.0);
		pText->setMetricsMode(Ogre::GMM_PIXELS);
		pText->setPosition(0, 0);
		
		pText->setParameter("font_name", "blue16");
		pText->setParameter("char_height", "16");
		pText->setParameter("horz_align", "center");
		pText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0));
		
		pContainer->addChild(pText);
		pOverlay->show();
	}

	ObjectTextDisplayComponent::~ObjectTextDisplayComponent() {
		Ogre::Overlay *pOverlay;
		Ogre::OverlayContainer *pContainer;
		Ogre::OverlayElement *pText;
		
		gameObject()->props().Get("objectText_overlay", &pOverlay);
		gameObject()->props().Get("objectText_container", &pContainer);
		gameObject()->props().Get("objectText_textElement", &pText);
		
		// overlay cleanup -- Ogre would clean this up at app exit but if your app 
		// tends to create and delete these objects often it's a good idea to do it here.
		
		pOverlay->hide();
		Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
		pContainer->removeChild("shapeNameText");
		pOverlay->remove2D(pContainer);
		overlayManager->destroyOverlayElement(pText);
		overlayManager->destroyOverlayElement(pContainer);
		overlayManager->destroy(pOverlay);
		
		gameObject()->props().Delete("objectText_overlay");
		gameObject()->props().Delete("objectText_container");
		gameObject()->props().Delete("objectText_textElement");
		gameObject()->props().Delete("objectText_text");
	}

	void ObjectTextDisplayComponent::setText(const std::string& text) {
		Ogre::OverlayElement *pText;
		gameObject()->props().Get("objectText_textElement", &pText);
		
		gameObject()->props().Set("objectText_text", text);
		pText->setCaption(text);
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

		Ogre::OverlayContainer *pContainer;
		gameObject()->props().Get("objectText_container", &pContainer);

		//m_pContainer->setPosition(min_x, min_y);
		pContainer->setPosition(1-max_x, min_y);  // Edited by alberts: This code works for me
		pContainer->setDimensions(max_x - min_x, 0.1); // 0.1, just "because"
	}
}
