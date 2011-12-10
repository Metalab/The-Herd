//
//  ObjectTextComponent.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_ObjectTextComponent_h
#define TheHerd_ObjectTextComponent_h

#include "GameObject.h"

namespace Rocket {
	namespace Core {
		class ElementDocument;
	}
}

namespace Engine {
	class ObjectTextDisplayComponent: public GameComponent {
	public:
		void setText(const std::string &text);
		void tick();
		
		ObjectTextDisplayComponent(GameObject *gameObject);
		virtual ~ObjectTextDisplayComponent();
	private:
		Rocket::Core::ElementDocument *m_overlay;
	};
}

#endif
