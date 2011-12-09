//
//  AudioService.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_AudioService_h
#define Testing_AudioService_h

#include "Service.h"
#include <string>

namespace FMOD {
	class EventSystem;
	class Event;
}

namespace Engine {
	class AudioService : public Service {
	public:
		AudioService(const std::string &resourcePath) : m_resourcePath(resourcePath) {}
		
		void startup();
		void shutdown();
		void tick();
		
		void loadBank(const std::string &name);
		FMOD::Event *getEvent(const std::string &name);
	private:
		std::string m_resourcePath;
		FMOD::EventSystem *m_eventsystem;
	};
}

#endif
