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
#include "ServiceManager.h"

namespace Game {
	void InteractionComponent::interactWith(Engine::GameObject *minion) {
		MinionComponent *minionComponent = minion->getComponent<MinionComponent>();
		if(!minionComponent)
			return; // not a minion
		
		int myMoney, otherMoney;
		float myLife;
		gameObject()->props().Get("money", &myMoney);
		gameObject()->props().Get("life", &myLife);
		
		minion->props().Get("money", &otherMoney);
		
		if((myLife < kLifeAIThresholdWantMore && myMoney > kMoneyForFood) ||
		   (myLife < kLifeAIThresholdFull && myMoney > kMoneyForFood * 10 && random() < (RAND_MAX/8))) {
			// trade
			GameService *gameService = (GameService*)Engine::ServiceManager::getSingletonPtr()->getService("game");
			
			minionComponent->changeMoney(kMoneyForFood);
			gameObject()->props().Set("money", myMoney - kMoneyForFood);
			gameObject()->props().Set("life", MIN(myLife + gameService->exchangeRate(), 1.0));
			return;
		}
		if(myMoney < kMoneyThresholdAttack) {
			if(otherMoney > kMoneyThresholdOccupy && random() < (RAND_MAX/16)) {
				// attack
				minionComponent->changeMoney(kMoneyForAttack);
				gameObject()->props().Set("money", myMoney + kMoneyForAttack);
				gameObject()->props().Set("life", MAX(myLife - kLifeForAttack * (random() / (float)RAND_MAX), 0.0));
				return;
			}
		} else {
			if(myMoney > kMoneyThresholdOccupy) {
				if(otherMoney < kMoneyThresholdOccupy && random() < (RAND_MAX/64)) {
					Engine::GameObject *stakeholder = minionComponent->stakeHolder();
					if((!stakeholder || stakeholder == gameObject()) && myMoney > kMoneyThresholdOccupy + kMoneyForStake/2) {
							// occupy
						minionComponent->addStake(gameObject());
						gameObject()->props().Set("money", myMoney - kMoneyForStake);
						return;
					} else {
						// different stakeholder, buy out?
						if(myMoney > kMoneyThresholdOccupy + kMoneyForBuyOut) {
							int stakeholderMoney;
							stakeholder->props().Get("money", &stakeholderMoney);
							
							if(stakeholderMoney > myMoney - kMoneyForBuyOut) {
								// avoid feeding richer minions
								MinionComponent *stakeholderMinionComponent = stakeholder->getComponent<MinionComponent>();
								if(!stakeholderMinionComponent)
									return; // not a minion (BUG!)

								minionComponent->buyOutStake();
								gameObject()->props().Set("money", myMoney - kMoneyForBuyOut);
								stakeholderMinionComponent->changeMoney(kMoneyForBuyOut);
								return;
							}
						}
					}
				}
			}
			if(myMoney > kMoneyThresholdPolice) {
				if(otherMoney > kMoneyThresholdAttack && random() < (RAND_MAX/8)) {
					// police
					minionComponent->changeLife(-kLifeForAttack);
					return;
				}
			}
		}
	}
}
