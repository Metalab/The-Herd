//
//  OgreService.cc
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "OgreService.h"
#include "OgrePlatform.h"
#include "OgreFramework.h"
#include "Clock.h"
#include "InputService.h"
#include "GameObject.h"
#include "Placeable.h"

#ifdef USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif

namespace Engine {
#ifdef USE_RTSHADER_SYSTEM
	/** This class demonstrates basic usage of the RTShader system.
	 It sub class the material manager listener class and when a target scheme callback
	 is invoked with the shader generator scheme it tries to create an equivalent shader
	 based technique based on the default technique of the given material.
	 */
	class ShaderGeneratorTechniqueResolverListener : public Ogre::MaterialManager::Listener
	{
	public:
		
		ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* pShaderGenerator)
		{
			mShaderGenerator = pShaderGenerator;			
		}
		
		/** This is the hook point where shader based technique will be created.
		 It will be called whenever the material manager won't find appropriate technique
		 that satisfy the target scheme name. If the scheme name is out target RT Shader System
		 scheme name we will try to create shader generated technique for it. 
		 */
		virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, 
													  const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, 
													  const Ogre::Renderable* rend)
		{	
			Ogre::Technique* generatedTech = NULL;
			
			// Case this is the default shader generator scheme.
			if (schemeName == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
			{
				bool techniqueCreated;
				
				// Create shader generated technique for this material.
				techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
																				originalMaterial->getName(), 
																				Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
																				schemeName);	
				
				// Case technique registration succeeded.
				if (techniqueCreated)
				{
					// Force creating the shaders for the generated technique.
					mShaderGenerator->validateMaterial(schemeName, originalMaterial->getName());
					
					// Grab the generated technique.
					Ogre::Material::TechniqueIterator itTech = originalMaterial->getTechniqueIterator();
					
					while (itTech.hasMoreElements())
					{
						Ogre::Technique* curTech = itTech.getNext();
						
						if (curTech->getSchemeName() == schemeName)
						{
							generatedTech = curTech;
							break;
						}
					}				
				}
			}
			
			return generatedTech;
		}
		
	protected:	
		Ogre::RTShader::ShaderGenerator*	mShaderGenerator;			// The shader generator instance.		
	};
#endif
	
	/*-----------------------------------------------------------------------------
	 | Initialize the RT Shader system.	
	 -----------------------------------------------------------------------------*/
	bool OgreService::initializeRTShaderSystem(Ogre::SceneManager* sceneMgr) {			
		if (Ogre::RTShader::ShaderGenerator::initialize()) {
			mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
			
			mShaderGenerator->addSceneManager(sceneMgr);
			
			// Setup core libraries and shader cache path.
			Ogre::StringVector groupVector = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
			Ogre::StringVector::iterator itGroup = groupVector.begin();
			Ogre::StringVector::iterator itGroupEnd = groupVector.end();
			Ogre::String shaderCoreLibsPath;
			Ogre::String shaderCachePath;
			
			for (; itGroup != itGroupEnd; ++itGroup)
			{
				Ogre::ResourceGroupManager::LocationList resLocationsList = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
				Ogre::ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
				Ogre::ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
				bool coreLibsFound = false;
				
				// Try to find the location of the core shader lib functions and use it
				// as shader cache path as well - this will reduce the number of generated files
				// when running from different directories.
				for (; it != itEnd; ++it)
				{
					if ((*it)->archive->getName().find("RTShaderLib") != Ogre::String::npos)
					{
						shaderCoreLibsPath = (*it)->archive->getName() + "/";
						shaderCachePath = shaderCoreLibsPath;
						coreLibsFound = true;
						break;
					}
				}
				// Core libs path found in the current group.
				if (coreLibsFound) 
					break; 
			}
			
			// Core shader libs not found -> shader generating will fail.
			if (shaderCoreLibsPath.empty())			
				return false;			
			
			// Create and register the material manager listener.
			mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);				
			Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
		}
		
		return true;
	}
	
	void OgreService::startup() {
		try {
			if(!OgreFramework::getSingletonPtr()->initOgre("The Herd", m_configPath, m_inputService, m_inputService))
				exit(0);
			
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("OgreService initialized!");

			Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
			
			// Clear event times
			Ogre::Root::getSingleton().clearEventTimes();
			
#ifdef USE_RTSHADER_SYSTEM
			initializeRTShaderSystem(OgreFramework::getSingletonPtr()->m_pSceneMgr);
			Ogre::MaterialPtr baseWhite = Ogre::MaterialManager::getSingleton().getByName("BaseWhite", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);				
			baseWhite->setLightingEnabled(false);
			mShaderGenerator->createShaderBasedTechnique("BaseWhite", 
														 Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
														 Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);	
			mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, 
											   "BaseWhite");
			baseWhite->getTechnique(0)->getPass(0)->setVertexProgram(
																	 baseWhite->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
			baseWhite->getTechnique(0)->getPass(0)->setFragmentProgram(
																	   baseWhite->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
			
			// creates shaders for base material BaseWhiteNoLighting using the RTSS
			mShaderGenerator->createShaderBasedTechnique("BaseWhiteNoLighting", 
														 Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
														 Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);	
			mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, 
											   "BaseWhiteNoLighting");
			Ogre::MaterialPtr baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
			baseWhiteNoLighting->getTechnique(0)->getPass(0)->setVertexProgram(baseWhiteNoLighting->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
			baseWhiteNoLighting->getTechnique(0)->getPass(0)->setFragmentProgram(baseWhiteNoLighting->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
#endif
		} catch( Ogre::Exception& e ) {
			std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
		}

	}
	
	void OgreService::shutdown() {
		
	}
	
	void OgreService::tick() {
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive()) {
			OgreFramework::getSingletonPtr()->updateOgre(m_clock->lastIncrement());
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
		}
	}
	
	std::vector<GameObject*> OgreService::sphereQuery(GameObject *gameObject, float radius) {
		std::vector<GameObject*> result;
		Ogre::Vector3 pos = gameObject->getComponent<Placeable>()->position();
		Ogre::SceneNode *sceneNode = gameObject->getComponent<Placeable>()->sceneNode();
		
		Ogre::SphereSceneQuery *query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createSphereQuery(Ogre::Sphere(pos, radius));
		query->setQueryMask(1);
		Ogre::SceneQueryResult &queryResult = query->execute();
		
		//list<MovableObject*>
		for(Ogre::SceneQueryResultMovableList::iterator iter = queryResult.movables.begin(); iter != queryResult.movables.end(); ++iter) {
			Ogre::SceneNode *resultSceneNode = (*iter)->getParentSceneNode();
			if(resultSceneNode != sceneNode) {
				Ogre::UserObjectBindings &bindings = resultSceneNode->getUserObjectBindings();
				Ogre::Any queryGameObjectAny = bindings.getUserAny("GameObject");
				if(!queryGameObjectAny.isEmpty())
					result.push_back(Ogre::any_cast<GameObject*>(queryGameObjectAny));
			}
		}
		
		OgreFramework::getSingletonPtr()->m_pSceneMgr->destroyQuery(query); // can't reuse, since the gameObjects move around all of the time
		
		return result;
	}

}
