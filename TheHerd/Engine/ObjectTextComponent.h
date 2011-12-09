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

namespace Engine {
	class ObjectTextDisplayComponent: public GameComponent {
	public:
		void setText(const std::string &text);
		void tick();
	protected:
		ObjectTextDisplayComponent(GameObject *gameObject);
		~ObjectTextDisplayComponent();
	};
}

#endif
