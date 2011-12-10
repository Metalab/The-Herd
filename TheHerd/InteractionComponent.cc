//
//  InteractionComponent.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "InteractionComponent.h"
#include "GameConstants.h"
#include "MinionComponent.h"
#include "GameService.h"
#include "EventLogService.h"
#include "ServiceManager.h"

namespace Game {
	void InteractionComponent::trade(Engine::GameObject *minion) {
		MinionComponent *minionComponent = minion->getComponent<MinionComponent>();
		GameService *gameService = (GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game");
		EventLogService *eventLogService = (EventLogService*)Engine::ServiceManager::getSingletonPtr()->getService("eventlog");

		int myMoney;
		float myLife;
		gameObject()->props().Get("money", &myMoney);
		gameObject()->props().Get("life", &myLife);

		minionComponent->changeMoney(kMoneyForFood);
		gameObject()->props().Set("money", myMoney - kMoneyForFood);
		gameObject()->props().Set("life", (float)MIN(myLife + gameService->exchangeRate(), 1.0));
		
		eventLogService->logInteraction(gameObject(), minion, "traded with");
	}
	
	void InteractionComponent::attack(Engine::GameObject *minion) {
		MinionComponent *minionComponent = minion->getComponent<MinionComponent>();
		EventLogService *eventLogService = (EventLogService*)Engine::ServiceManager::getSingletonPtr()->getService("eventlog");
		int myMoney;
		float myLife;
		gameObject()->props().Get("money", &myMoney);
		gameObject()->props().Get("life", &myLife);
		
		minionComponent->changeMoney(kMoneyForAttack);
		gameObject()->props().Set("money", myMoney + kMoneyForAttack);
		gameObject()->props().Set("life", (float)MAX(myLife - kLifeForAttack * (random() / (float)RAND_MAX), 0.0));
		
		eventLogService->logInteraction(gameObject(), minion, "attacked");
	}
	
	void InteractionComponent::occupy(Engine::GameObject *minion) {
		MinionComponent *minionComponent = minion->getComponent<MinionComponent>();
		EventLogService *eventLogService = (EventLogService*)Engine::ServiceManager::getSingletonPtr()->getService("eventlog");

		int myMoney;
		gameObject()->props().Get("money", &myMoney);
		minionComponent->addStake(gameObject());
		gameObject()->props().Set("money", myMoney - kMoneyForStake);
		
		eventLogService->logInteraction(gameObject(), minion, "increased the stake in");
	}
	
	void InteractionComponent::repay(Engine::GameObject *minion) {
		MinionComponent *minionComponent = minion->getComponent<MinionComponent>();
		EventLogService *eventLogService = (EventLogService*)Engine::ServiceManager::getSingletonPtr()->getService("eventlog");
		Engine::GameObject *stakeholder = minionComponent->stakeHolder();
		MinionComponent *stakeholderMinionComponent = stakeholder->getComponent<MinionComponent>();
		if(!stakeholderMinionComponent)
			return; // not a minion (BUG!)

		int myMoney;
		gameObject()->props().Get("money", &myMoney);

		minionComponent->buyOutStake();
		gameObject()->props().Set("money", myMoney - kMoneyForBuyOut);
		stakeholderMinionComponent->changeMoney(kMoneyForBuyOut);
		
		eventLogService->logInteraction(gameObject(), minion, "repaid a stake of");
	}
	
	void InteractionComponent::police(Engine::GameObject *minion) {
		MinionComponent *minionComponent = minion->getComponent<MinionComponent>();
		EventLogService *eventLogService = (EventLogService*)Engine::ServiceManager::getSingletonPtr()->getService("eventlog");
		
		minionComponent->changeLife(-kLifeForAttack);
		eventLogService->logInteraction(gameObject(), minion, "victimized");
	}
}
