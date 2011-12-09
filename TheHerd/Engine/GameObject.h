//
//  GameObject.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_GameObject_h
#define Testing_GameObject_h

#include "KeyValueDictionary.h"
#include <set>

namespace Engine {
	class GameObject;
	class GameComponent {
	protected:
		GameObject *gameObject() {
			return m_gameObject;
		}
		
		virtual void tick() = 0;
	private:
		GameComponent(GameObject *gameObject) : m_gameObject(gameObject) {}
		virtual ~GameComponent() {}
		
		GameObject *m_gameObject;
		friend class GameObject;
	};
	
	class GameObject {
	public:
		GameObject() : m_wantsUpdate(false) {}
		~GameObject();
		
		void setWantsUpdate(bool flag) {
			m_wantsUpdate = flag;
		}
		
		void tick();
		
		KeyValueDictionary &props() {
			return m_properties;
		}
		
		template<typename T> T *addComponent() {
			return &(*m_components.insert(new T(this)).first);
		}
		template<typename T> T *getComponent();
	private:
		bool m_wantsUpdate;
		std::set<GameComponent*> m_components;
		KeyValueDictionary m_properties;
	};
}

#endif
