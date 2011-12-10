//
//  OgreService.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_OgreService_h
#define Testing_OgreService_h

#include "Service.h"
#include <OIS/OIS.h>
#include <vector>

namespace Ogre {
	class SceneManager;
	
	namespace RTShader {
		class ShaderGenerator;
	}
}

namespace Engine {
	class ShaderGeneratorTechniqueResolverListener;
	class InputService;
	class GameObject;
	
	class OgreService : public Engine::Service {
	public:
		OgreService(InputService *inputService, Clock *clock) : m_inputService(inputService), m_clock(clock) {}
		virtual ~OgreService() {}
		
		void startup();
		void shutdown();
		void tick();
		
		std::vector<GameObject*> sphereQuery(GameObject *gameObject, float radius);
		
	private:
		InputService *m_inputService;
		Clock *m_clock;
		
#ifdef USE_RTSHADER_SYSTEM
		Ogre::RTShader::ShaderGenerator*			mShaderGenerator;			// The Shader generator instance.
		ShaderGeneratorTechniqueResolverListener*	mMaterialMgrListener;		// Shader generator material manager listener.	
		bool initializeRTShaderSystem(Ogre::SceneManager* sceneMgr);
#endif // USE_RTSHADER_SYSTEM
	};
}

#endif
