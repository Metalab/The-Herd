//
//  ServiceManager.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_ServiceManager_h
#define Testing_ServiceManager_h

#include <Ogre/OgreSingleton.h>
#include <set>
#include <map>

namespace Engine {
	class Service;
	class Clock;
	
	struct ServiceCompare {
		bool operator()(std::pair<Service*, int> s1, std::pair<Service*, int> s2) const {
			return s1.second < s2.second;
		}
	};
	
	class ServiceManager : public Ogre::Singleton<ServiceManager> {
	public:
		ServiceManager() : m_terminate(false) {}
		
		void registerService(const std::string &name, Service *service, int priority = 0);
		void unregisterService(Service *service);
		Service *getService(const std::string &name);
		
		void registerClock(Clock *clock);
		void unregisterClock(Clock *clock);
		
		void startup();
		void shutdown();
		
		void tick();
		
		void terminate() {
			m_terminate = true;
		}
		bool shouldTerminate() {
			return m_terminate;
		}
	private:
		bool m_terminate;

		ServiceManager(const ServiceManager &copy);
		ServiceManager &operator=(const ServiceManager &copy);
		
		typedef std::multiset<std::pair<Service*, int>,  ServiceCompare> ServiceSet;
		ServiceSet m_services;
		std::map<std::string, Service*> m_serviceMap;
		std::set<Clock*> m_clocks;
	};
}

#endif
