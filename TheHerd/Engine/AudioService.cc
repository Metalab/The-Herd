//
//  AudioService.cc
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "AudioService.h"
#include <fmod_event.hpp>
#include <fmod_errors.h>
#include <Ogre/OgreLogManager.h>
#include <sstream>

namespace Engine {
	void AudioService::startup() {
		FMOD_RESULT result;
		result = FMOD::EventSystem_Create(&m_eventsystem);
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "FMOD::EventSystem_Create(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
			return;
		}
		result = m_eventsystem->init(64, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL);
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "eventsystem->init(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
			return;
		}
		result = m_eventsystem->setMediaPath((m_resourcePath + "/").c_str());
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "eventsystem->setMediaPath(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
			return;
		}
	}
	
	void AudioService::shutdown() {
		FMOD_RESULT result;
		result = m_eventsystem->release();
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "eventsystem->release(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
		}
	}
	
	void AudioService::loadBank(const std::string &name) {
		FMOD_RESULT result;
		result = m_eventsystem->load(name.c_str(), 0, 0);
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "eventsystem->load(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
		}
	}
	
	void AudioService::tick() {
		FMOD_RESULT result;
		result = m_eventsystem->update();
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "eventsystem->update(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
		}
	}
	
	FMOD::Event *AudioService::getEvent(const std::string &name) {
		FMOD::Event *event;
		FMOD_RESULT result = m_eventsystem->getEvent(name.c_str(), FMOD_EVENT_DEFAULT, &event);
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "eventsystem->getEvent(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
			return NULL;
		}
		return event;
	}
	
	FMOD::EventGroup *AudioService::getEventGroup(const std::string &name) {
		FMOD::EventGroup *eventgroup;
		FMOD_RESULT result = m_eventsystem->getGroup(name.c_str(), FMOD_EVENT_DEFAULT, &eventgroup);
		if(result != FMOD_OK) {
			std::ostringstream S;
			S << "eventsystem->getGroup(" << result << "): " << FMOD_ErrorString(result);
			
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LML_CRITICAL, S.str());
			return NULL;
		}
		return eventgroup;
	}
}
