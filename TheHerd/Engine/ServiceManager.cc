//
//  ServiceManager.cc
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "ServiceManager.h"
#include "Service.h"
#include "Clock.h"
#include "OgreFramework.h"

namespace Ogre
{
    template<> Engine::ServiceManager* Ogre::Singleton<Engine::ServiceManager>::msSingleton = 0;
}

namespace Engine {
	void ServiceManager::registerService(const std::string &name, Service *service, int priority) {
		m_services.insert(std::pair<Service*, int>(service, priority));
		m_serviceMap[name] = service;
		service->setName(name);
	}
	
	void ServiceManager::unregisterService(Service *service) {
		for(ServiceSet::iterator iter = m_services.begin(); iter != m_services.end(); ++iter)
			if(iter->first == service) {
				service->shutdown();
				m_services.erase(iter);
				m_serviceMap.erase(service->name());
				
				delete service;
				return;
			}
	}
	
	Service *ServiceManager::getService(const std::string &name) {
		std::map<std::string, Service*>::iterator iter = m_serviceMap.find(name);
		if(iter == m_serviceMap.end())
			return NULL;
		else
			return iter->second;
	}
	
	void ServiceManager::registerClock(Clock *clock) {
		m_clocks.insert(clock);
	}
	
	void ServiceManager::unregisterClock(Clock *clock) {
		m_clocks.erase(clock);
	}
	
	void ServiceManager::startup() {
		for(ServiceSet::iterator iter = m_services.begin(); iter != m_services.end(); ++iter)
			iter->first->startup();
	}
	
	void ServiceManager::shutdown() {
		for(ServiceSet::iterator iter = m_services.begin(); iter != m_services.end(); ++iter) {
			iter->first->shutdown();
			delete iter->first;
		}
		
		m_services.clear();
	}
	
	void ServiceManager::tick() {
		double startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

		for(ServiceSet::iterator iter = m_services.begin(); iter != m_services.end(); ++iter)
			iter->first->tick();
		
		double lastFrameTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		
		for(std::set<Clock*>::iterator iter = m_clocks.begin(); iter != m_clocks.end(); ++iter)
			(*iter)->tick(lastFrameTime);
	}
}
