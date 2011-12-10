//
//  EventLogService.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "EventLogService.h"
#include <Rocket/Core.h>
#include "Engine/ServiceManager.h"
#include "GameService.h"
#include "Engine/GameObject.h"

namespace Game {
	static const unsigned kLinesOfLog = 4;
	
	void EventLogService::logInteraction(Engine::GameObject *subject, Engine::GameObject *object, const std::string &verb) {
		m_log.push_front(LogEntry(subject, object, verb));
	}
	
	void EventLogService::startup() {
		
	}
	
	void EventLogService::shutdown() {
		
	}
	
	void EventLogService::tick() {
		while(m_log.size() > kLinesOfLog)
			m_log.pop_back();
		
		Rocket::Core::ElementDocument *hud = ((GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game"))->playerHud();
		
		if(!hud)
			return;
		
		Engine::GameObject *player = ((GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game"))->player();
		
		for(unsigned i = 0; i < kLinesOfLog && i < m_log.size(); ++i) {
			LogEntry &entry = m_log[i];
			
			std::ostringstream S;
			if(entry.subject == player)
				S << "You have ";
			else {
				int money;
				entry.subject->props().Get("money", &money);
				S << entry.subject->name() << " ($" << money << ") has ";
			}
			S << entry.verb;
			if(entry.object == player)
				S << " you.";
			else {
				int money;
				entry.object->props().Get("money", &money);
				S << " " << entry.object->name() << " ($" << money << ").";
			}
			
			std::ostringstream id;
			id << "log-" << i;
			
			hud->GetElementById(id.str().c_str())->SetInnerRML(S.str().c_str());
		}
		// clear the rest
		for(unsigned i = m_log.size(); i < kLinesOfLog; ++i) {
			std::ostringstream id;
			id << "log-" << i;
			
			hud->GetElementById(id.str().c_str())->SetInnerRML("");
		}
	}
}
