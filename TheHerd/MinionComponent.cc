//
//  MinionComponent.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "MinionComponent.h"
#include "OgreFramework.h"
#include "GameConstants.h"

namespace Game {
	float MinionComponent::life() {
		float life;
		gameObject()->props().Get("life", &life);
		return life;
	}
	
	void MinionComponent::changeLife(float amount) {
		float l = life() + amount;
		gameObject()->props().Set("life", l);
	}
	
	int MinionComponent::money() {
		int money;
		gameObject()->props().Get("money", &money);
		return money;
	}
	
	void MinionComponent::changeMoney(int amount) {
		int m = money() + amount;
		gameObject()->props().Set("money", m);
	}
	
	void MinionComponent::addStake(Engine::GameObject *stakeholder) {
		if(gameObject()->props().Exists("stakeholder")) {
			Engine::GameObject *oldStakeholder;
			gameObject()->props().Get("stakeholder", &oldStakeholder);
			if(oldStakeholder != stakeholder) {
				std::string name;
				gameObject()->props().Get("name", &name);
				OgreFramework::getSingletonPtr()->m_pLog->logMessage("Trying to add a stake with someone who is already occupied! (" + name + ")");
			} else {
				unsigned stakecount;
				gameObject()->props().Get("stakecount", &stakecount);
				gameObject()->props().Set("stakecount", stakecount + 1U);
			}
		} else {
			gameObject()->props().Set("stakeholder", stakeholder);
			gameObject()->props().Set("stakecount", 1U);
		}
	}
	
	void MinionComponent::buyOutStake() {
		if(gameObject()->props().Exists("stakeholder")) {
			Engine::GameObject *stakeholder;
			gameObject()->props().Get("stakeholder", &stakeholder);
			
			unsigned stakecount;
			gameObject()->props().Get("stakecount", &stakecount);
			if(stakecount == 1) {
				gameObject()->props().Delete("stakeholder");
				gameObject()->props().Delete("stakecount");
			} else
				gameObject()->props().Set("stakecount", stakecount - 1U);
			
			stakeholder->getComponent<MinionComponent>()->changeMoney(kMoneyForBuyOut);
		} else {
			std::string name;
			gameObject()->props().Get("name", &name);
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Trying to buy out someone who is not occupied! (" + name + ")");
		}
	}
}
