//
//  ObjectOverlayComponent.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_ObjectOverlayComponent_h
#define TheHerd_ObjectOverlayComponent_h

#include "GameObject.h"

namespace Rocket {
	namespace Core {
		class ElementDocument;
	}
}

namespace Engine {
	class ObjectOverlayComponent : public GameComponent {
	public:
		ObjectOverlayComponent(GameObject *gameObject) : GameComponent(gameObject), m_overlay(NULL) {}
		virtual ~ObjectOverlayComponent();
		void setDocumentName(const std::string &doc);
		
		Rocket::Core::ElementDocument *document() {
			return m_overlay;
		}
		
		void tick();
	private:
		Rocket::Core::ElementDocument *m_overlay;
	};
}

#endif
