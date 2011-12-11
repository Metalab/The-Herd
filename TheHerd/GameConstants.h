//
//  GameConstants.h
//  TheHerd
//
//  Created by Andreas Monitzer on 2011-12-10.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef TheHerd_GameConstants_h
#define TheHerd_GameConstants_h

namespace Game {
	static const unsigned kMinionCount = 20;
	static const unsigned kFieldWidth = 50;
	static const unsigned kFieldHeight = 50;
	
	static const int kMoneyForBuyOut = 150;
	static const int kMoneyForStake = 100;
	static const int kMoneyForFood = 50;
	static const float kMoneyFractionForAttack = 0.1;
	static const float kMoneyTaxRatePerStake = 0.2;
	
	static const int kMoneyThresholdAttack = 100;
	static const int kMoneyThresholdOccupy = 1000;
	static const int kMoneyThresholdPolice = 3000;
	static const int kMoneyThresholdInvested = 150;

	static const int kMoneyMaxStartup = 2000;

	static const float kLifeForPolice = 0.3;
	static const float kLifeForAttack = 0.5;
	
	static const float kLifeMinStartup = 0.5;
	
	static const float kLifeAIThresholdWantMore = 0.3;
	static const float kLifeAIThresholdFull = 0.8;
	
	static const float kHunger = 0.0001;
	
	static const float kMinionPrivacyRadius = 4.0;
	static const float kMinionWalkSpeed = 0.01;
}

#endif
