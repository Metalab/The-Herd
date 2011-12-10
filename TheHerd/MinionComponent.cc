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
		if(amount > 0) { // privatize profits, sozialize losses
			Engine::GameObject *stakeholder = stakeHolder();
			if(stakeholder) {
				unsigned stakecount;
				gameObject()->props().Get("stakecount", &stakecount);
				
				// careful with rounding errors here, amount * taxrate + amount * (1.0 - taxrate) != amount
				float taxrate = MIN(kMoneyTaxRatePerStake * stakecount, 1.0);
				int afterTax = amount * (1.0 - taxrate);
				
				gameObject()->props().Set("money", money() + afterTax);
				
				stakeholder->getComponent<MinionComponent>()->changeMoney(amount - afterTax);
				return;
			}
		}
		
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
	
	Engine::GameObject *MinionComponent::stakeHolder() {
		if(!gameObject()->props().Exists("stakeholder"))
			return NULL;
		Engine::GameObject *stakeholder;
		gameObject()->props().Get("stakeholder", &stakeholder);
		return stakeholder;
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
