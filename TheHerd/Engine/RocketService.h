//
//  RocketService.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_RocketService_h
#define Testing_RocketService_h

#include "Service.h"
#include <Ogre/OgreRenderQueueListener.h>
#include <OIS/OIS.h>

namespace Rocket {
	namespace Core {
		class Context;
		class ElementDocument;
	}
}

namespace Engine {
	class SystemInterfaceOgre3D;
	class RenderInterfaceOgre3D;
	
	class RocketService : public Service, public Ogre::RenderQueueListener, public OIS::MouseListener {
	public:
		RocketService(const std::string &path) : m_path(path) {}
		void startup();
		void shutdown();
		
		void tick() {}
		
		void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		void ConfigureRenderSystem();
		void BuildProjectionMatrix(Ogre::Matrix4& projection_matrix);

		bool mouseMoved(const OIS::MouseEvent& e);
		bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		
		Rocket::Core::ElementDocument *loadDocument(const std::string &name);

	private:
		int GetKeyModifierState();
		
		std::string m_path;
		Rocket::Core::Context *m_context;
		SystemInterfaceOgre3D *m_ogre_system;
		RenderInterfaceOgre3D *m_ogre_renderer;
	};
}

#endif
