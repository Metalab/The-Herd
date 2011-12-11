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
#include "Engine/ObjectOverlayComponent.h"
#include <Rocket/Core.h>
#include "Engine/ServiceManager.h"
#include "GameService.h"
#include "Engine/Clock.h"
#include "InteractionComponent.h"

namespace Game {
	MinionComponent::MinionComponent(Engine::GameObject *gameObject) : GameComponent(gameObject) {
		gameObject->props().Set("stakeholder", (Engine::GameObject*)NULL);
	}
	
	void MinionComponent::tick() {
		float l = life();
		
		// reduce due to hunger
		GameService *gameService = (GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game");
		l -= kHunger * gameService->clock()->lastIncrement();
		gameObject()->props().Set("life", l);
		
		Engine::GameObject *stakeholder = stakeHolder();
		int m = money();
		if(m > kMoneyThresholdOccupy && stakeholder) {
			InteractionComponent *interactionComponent = gameObject()->getComponent<InteractionComponent>();
			if(interactionComponent)
				interactionComponent->repay(gameObject());
		}

		Engine::ObjectOverlayComponent *objectOverlayComponent = gameObject()->getComponent<Engine::ObjectOverlayComponent>();
		if(!objectOverlayComponent)
			return;
		Rocket::Core::ElementDocument *doc = objectOverlayComponent->document();
		if(!doc)
			return;

		std::ostringstream S;
		S << "$" << m;
		doc->GetElementById("money")->SetInnerRML(S.str().c_str());
		
		S.str("");
		S << l * 100.0 << "%";
		Rocket::Core::Element *lifeBar = doc->GetElementById("life");
		
		lifeBar->SetProperty("width", S.str().c_str());
		lifeBar->SetProperty("background-color", (l < .3)?"#f00a":(l < .6)?"#ff0a":"#0f0a");
		lifeBar->SetProperty("width", S.str().c_str());

		if(stakeholder) {
			bool playerIsStakeholder = stakeholder == gameService->player();
			unsigned count = stakecount();
			for(unsigned i = 0; i < count; ++i) {
				std::ostringstream S;
				S << "stake-" << i;
				doc->GetElementById(S.str().c_str())->SetProperty("background-color", playerIsStakeholder?"#0f0a":"#f00a");
			}
			for(unsigned i = count; i < 5; ++i) {
				std::ostringstream S;
				S << "stake-" << i;
				doc->GetElementById(S.str().c_str())->SetProperty("background-color", "#0000");
			}
		} else {
			for(unsigned i = 0; i < 5; ++i) {
				std::ostringstream S;
				S << "stake-" << i;
				doc->GetElementById(S.str().c_str())->SetProperty("background-color", "#0000");
			}
		}
	}
	
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
		Engine::GameObject *oldStakeholder = stakeHolder();
		if(!oldStakeholder) {
			gameObject()->props().Set("stakeholder", stakeholder);
			gameObject()->props().Set("stakecount", 1U);
			return;
		}
		
		if(oldStakeholder != stakeholder) {
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Trying to add a stake with someone who is already occupied! (" + gameObject()->name() + ")");
		} else {
			unsigned stakecount;
			gameObject()->props().Get("stakecount", &stakecount);
			gameObject()->props().Set("stakecount", stakecount + 1U);
		}
	}
	
	Engine::GameObject *MinionComponent::stakeHolder() {
		Engine::GameObject *stakeholder;
		gameObject()->props().Get("stakeholder", &stakeholder);
		return stakeholder;
	}

	unsigned MinionComponent::stakecount() {
		if(!stakeHolder())
			return 0;
		unsigned stakecount;
		gameObject()->props().Get("stakecount", &stakecount);
		return stakecount;
	}

	void MinionComponent::buyOutStake() {
		Engine::GameObject *stakeholder = stakeHolder();
		if(!stakeholder) {
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Trying to buy out someone who is not occupied! (" + gameObject()->name() + ")");
			return;
		}
		
		unsigned stakecount;
		gameObject()->props().Get("stakecount", &stakecount);
		if(stakecount == 1) {
			gameObject()->props().Set("stakeholder", (Engine::GameObject*)NULL);
			gameObject()->props().Delete("stakecount");
		} else
			gameObject()->props().Set("stakecount", stakecount - 1U);
		
		stakeholder->getComponent<MinionComponent>()->changeMoney(kMoneyForBuyOut);
	}
	
	void MinionComponent::stakeholderDied() {
		// it's our lucky day!
		gameObject()->props().Set("stakeholder", (Engine::GameObject*)NULL);
		gameObject()->props().Delete("stakecount");
	}
}
