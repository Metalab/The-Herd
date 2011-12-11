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
		Engine::GameObject *player = ((GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game"))->player();
		std::ostringstream S;
		if(subject == player)
			S << "You have ";
		else {
			int money;
			subject->props().Get("money", &money);
			S << subject->name() << " ($" << money << ") has ";
		}
		S << verb;
		if(object) {
			if(object == player)
				S << " you.";
			else {
				int money;
				object->props().Get("money", &money);
				S << " " << object->name() << " ($" << money << ").";
			}
		}
		
		m_log.push_front(S.str());
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
		
		for(unsigned i = 0; i < kLinesOfLog && i < m_log.size(); ++i) {
			std::ostringstream id;
			id << "log-" << i;
			
			hud->GetElementById(id.str().c_str())->SetInnerRML(m_log[i].c_str());
		}
		// clear the rest
		for(unsigned i = m_log.size(); i < kLinesOfLog; ++i) {
			std::ostringstream id;
			id << "log-" << i;
			
			hud->GetElementById(id.str().c_str())->SetInnerRML("");
		}
	}
}
