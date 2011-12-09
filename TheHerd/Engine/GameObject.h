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
		GameComponent(GameObject *gameObject) : m_gameObject(gameObject) {}
		virtual ~GameComponent() {}
		
		GameObject *gameObject() {
			return m_gameObject;
		}
		
		virtual void tick() = 0;
	private:
		
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
			return static_cast<T*>(*m_components.insert(new T(this)).first);
		}
		template<typename T> T *getComponent() {
			for(std::set<GameComponent*>::iterator iter = m_components.begin(); iter != m_components.end(); ++iter) {
				T *component = dynamic_cast<T*>(*iter);
				if(component)
					return component;
			}
			return NULL;
		}
	private:
		bool m_wantsUpdate;
		std::set<GameComponent*> m_components;
		KeyValueDictionary m_properties;
	};
}

#endif
