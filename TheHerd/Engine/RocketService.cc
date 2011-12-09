//
//  RocketService.cc
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "RocketService.h"
#include "ServiceManager.h"
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include "RenderInterfaceOgre3D.h"
#include "SystemInterfaceOgre3D.h"
#include "OgreService.h"
#include "OgreFramework.h"
#include "InputService.h"

namespace Engine {
	void RocketService::startup() {
		Ogre::RenderWindow *window = OgreFramework::getSingletonPtr()->m_pRenderWnd;
		
		// Rocket initialisation.
		m_ogre_renderer = new RenderInterfaceOgre3D(window->getWidth(), window->getHeight());
		Rocket::Core::SetRenderInterface(m_ogre_renderer);
		
		m_ogre_system = new SystemInterfaceOgre3D();
		Rocket::Core::SetSystemInterface(m_ogre_system);
		
		Rocket::Core::Initialise();
		Rocket::Controls::Initialise();
		
		// Load the fonts from the path to the sample directory.
		Rocket::Core::FontDatabase::LoadFontFace((m_path + "/Delicious-Roman.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((m_path + "/Delicious-Bold.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((m_path + "/Delicious-Italic.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((m_path + "/Delicious-BoldItalic.otf").c_str());
		
		m_context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(window->getWidth(), window->getHeight()));
		Rocket::Debugger::Initialise(m_context);
		
		// Load the mouse cursor and release the caller's reference.
		Rocket::Core::ElementDocument* cursor = m_context->LoadMouseCursor((m_path + "/cursor.rml").c_str());
		if (cursor)
			cursor->RemoveReference();
		
		OgreFramework::getSingletonPtr()->m_pSceneMgr->addRenderQueueListener(this);
		
		static_cast<InputService*>(ServiceManager::getSingletonPtr()->getService("input"))->registerMouseListener(this);
	}
	
	Rocket::Core::ElementDocument *RocketService::loadDocument(const std::string &name) {
		return m_context->LoadDocument((m_path + "/" + name).c_str());
	}
	
	void RocketService::shutdown() {
		OgreFramework::getSingletonPtr()->m_pSceneMgr->removeRenderQueueListener(this);
		m_context->RemoveReference();
		Rocket::Core::Shutdown();
		
		delete m_ogre_system;
		delete m_ogre_renderer;
	}
	
	// Called from Ogre before a queue group is rendered.
	void RocketService::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
		{
			m_context->Update();
			
			ConfigureRenderSystem();
			m_context->Render();
		}
	}
	
	// Configures Ogre's rendering system for rendering Rocket.
	void RocketService::ConfigureRenderSystem()
	{
		Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();
		
		// Set up the projection and view matrices.
		Ogre::Matrix4 projection_matrix;
		BuildProjectionMatrix(projection_matrix);
		render_system->_setProjectionMatrix(projection_matrix);
		render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);
		
		// Disable lighting, as all of Rocket's geometry is unlit.
		render_system->setLightingEnabled(false);
		// Disable depth-buffering; all of the geometry is already depth-sorted.
		render_system->_setDepthBufferParams(false, false);
		// Rocket generates anti-clockwise geometry, so enable clockwise-culling.
		render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
		// Disable fogging.
		render_system->_setFog(Ogre::FOG_NONE);
		// Enable writing to all four channels.
		render_system->_setColourBufferWriteEnabled(true, true, true, true);
		// Unbind any vertex or fragment programs bound previously by the application.
		render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		
		// Set texture settings to clamp along both axes.
		Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
		addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
		addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
		addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
		render_system->_setTextureAddressingMode(0, addressing_mode);
		
		// Set the texture coordinates for unit 0 to be read from unit 0.
		render_system->_setTextureCoordSet(0, 0);
		// Disable texture coordinate calculation.
		render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		// Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
		// except in the case of scaling tiled decorators.
		render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
		// Disable texture coordinate transforms.
		render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
		// Reject pixels with an alpha of 0.
		render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
		// Disable all texture units but the first.
		render_system->_disableTextureUnitsFrom(1);
		
		// Enable simple alpha blending.
		render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
		
		// Disable depth bias.
		render_system->_setDepthBias(0, 0);
	}
	
	// Builds an OpenGL-style orthographic projection matrix.
	void RocketService::BuildProjectionMatrix(Ogre::Matrix4& projection_matrix)
	{
		float z_near = -1;
		float z_far = 1;
		
		projection_matrix = Ogre::Matrix4::ZERO;
		Ogre::RenderWindow *window = OgreFramework::getSingletonPtr()->m_pRenderWnd;
		
		// Set up matrices.
		projection_matrix[0][0] = 2.0f / window->getWidth();
		projection_matrix[0][3]= -1.0000000f;
		projection_matrix[1][1]= -2.0f / window->getHeight();
		projection_matrix[1][3]= 1.0000000f;
		projection_matrix[2][2]= -2.0f / (z_far - z_near);
		projection_matrix[3][3]= 1.0000000f;
	}
	
	bool RocketService::mouseMoved(const OIS::MouseEvent& e)
	{
		int key_modifier_state = GetKeyModifierState();
		
		m_context->ProcessMouseMove(e.state.X.abs, e.state.Y.abs, key_modifier_state);
		if (e.state.Z.rel != 0)
			m_context->ProcessMouseWheel(e.state.Z.rel / -120, key_modifier_state);
		
		return true;
	}
	
	bool RocketService::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		m_context->ProcessMouseButtonDown((int) id, GetKeyModifierState());
		return true;
	}
	
	bool RocketService::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		m_context->ProcessMouseButtonUp((int) id, GetKeyModifierState());
		return true;
	}
	
	int RocketService::GetKeyModifierState()
	{
		int modifier_state = 0;
		
		/*
		OIS::Keyboard *keyboard = OgreFramework::getSingletonPtr()->m_pKeyboard;
		if (keyboard->isModifierDown(OIS::Keyboard::Ctrl))
			modifier_state |= Rocket::Core::Input::KM_CTRL;
		if (keyboard->isModifierDown(OIS::Keyboard::Shift))
			modifier_state |= Rocket::Core::Input::KM_SHIFT;
		if (keyboard->isModifierDown(OIS::Keyboard::Alt))
			modifier_state |= Rocket::Core::Input::KM_ALT;
		
		UInt32 key_modifiers = GetCurrentEventKeyModifiers();
		if (key_modifiers & (1 << alphaLockBit))
			modifier_state |= Rocket::Core::Input::KM_CAPSLOCK;
		*/
		return modifier_state;
	}
}
