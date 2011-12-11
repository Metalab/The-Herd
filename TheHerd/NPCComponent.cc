//
//  NPCComponent.cc
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#include "NPCComponent.h"
#include "MinionComponent.h"
#include "Engine/ServiceManager.h"
#include "EventLogService.h"
#include "GameService.h"
#include "GameConstants.h"
#include "InteractionComponent.h"

namespace Game {
	void NPCComponent::interactWith(Engine::GameObject *minion) {
		MinionComponent *minionComponent = minion->getComponent<MinionComponent>();
		if(!minionComponent)
			return; // not a minion
		InteractionComponent *interactionComponent = gameObject()->getComponent<InteractionComponent>();
		if(!interactionComponent)
			return; // we can't interact!
		
		int myMoney, otherMoney;
		float myLife;
		gameObject()->props().Get("money", &myMoney);
		gameObject()->props().Get("life", &myLife);
		
		minion->props().Get("money", &otherMoney);
		
		EventLogService *eventLogService = (EventLogService*)Engine::ServiceManager::getSingletonPtr()->getService("eventlog");
		
		if((myLife < kLifeAIThresholdWantMore && myMoney > kMoneyForFood) ||
		   (myLife < kLifeAIThresholdFull && myMoney > kMoneyForFood * 10 && random() < (RAND_MAX/8))) {
			interactionComponent->trade(minion);
			return;
		}
		if(myMoney < kMoneyThresholdAttack) {
			if(random() < (RAND_MAX/2)) {
				interactionComponent->attack(minion);
				return;
			}
		} else {
			if(myMoney > kMoneyThresholdOccupy) {
				if(otherMoney < kMoneyThresholdOccupy && random() < (RAND_MAX/64)) {
					Engine::GameObject *stakeholder = minionComponent->stakeHolder();
					if((!stakeholder || (stakeholder == gameObject() && minionComponent->stakecount() < 4)) && myMoney > kMoneyThresholdOccupy + kMoneyForStake/2) {
						interactionComponent->occupy(minion);
						return;
					} else {
						// different stakeholder, buy out?
						if(myMoney > kMoneyThresholdOccupy + kMoneyForBuyOut) {
							int stakeholderMoney;
							stakeholder->props().Get("money", &stakeholderMoney);
							
							if(stakeholderMoney > myMoney - kMoneyForBuyOut) {
								// avoid feeding richer minions
								interactionComponent->repay(minion);
								return;
							}
						}
					}
				}
			}
			if(myMoney > kMoneyThresholdPolice && otherMoney < kMoneyThresholdOccupy) {
				if(otherMoney > kMoneyThresholdAttack && random() < (RAND_MAX/8)) {
					interactionComponent->police(minion);
					return;
				}
			}
		}
	}
}
