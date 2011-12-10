//
//  EngineAppDelegate.m
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#import "EngineAppDelegate.h"
#include "Engine/ServiceManager.h"
#include "Engine/InputService.h"
#include "Engine/OgreService.h"
#include "Engine/AudioService.h"
#include "Engine/RocketService.h"
#include "Engine/GameObjectService.h"
#include "Engine/Clock.h"
#include "Engine/GameObject.h"
#include "Engine/Placeable.h"
#include "Engine/ObjectTextComponent.h"
#include "GameService.h"

#include "OgreFramework.h"

// All this does is suppress some messages in the run log.  NSApplication does not
// implement buttonPressed and apps without a NIB have no target for the action.
@implementation NSApplication (_suppressUnimplementedActionWarning)
- (void) buttonPressed:(id)sender
{
    /* Do nothing */
}
@end

@implementation EngineAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)application {
	NSString *resources = [[NSBundle mainBundle] resourcePath];
	new Engine::ServiceManager();
	new OgreFramework([resources fileSystemRepresentation]);
	
	Engine::ServiceManager *sm = Engine::ServiceManager::getSingletonPtr();
	Engine::InputService *inputService = new Engine::InputService();
	Engine::AudioService *audioService = new Engine::AudioService([[resources stringByAppendingPathComponent:@"audio"] fileSystemRepresentation]);
	gameClock = new Engine::Clock();
	Engine::GameObjectService *gameObjectService = new Engine::GameObjectService();
	
	sm->registerService("input", inputService, -1);
	sm->registerService("ogre", new Engine::OgreService(inputService, gameClock));
	sm->registerService("audio", audioService);
	sm->registerService("rocket", new Engine::RocketService([[resources stringByAppendingPathComponent:@"media/ui"] fileSystemRepresentation]), 1);
	sm->registerService("gameObject", gameObjectService, -1);
	sm->registerService("game", new Game::GameService(), 100);
	
	sm->startup();
	
	[self performSelector:@selector(tick) withObject:nil afterDelay:0.0];
	
	OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
	OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,75,75);
}

- (void)tick {
	Engine::ServiceManager::getSingletonPtr()->tick();
	if(!Engine::ServiceManager::getSingletonPtr()->shouldTerminate())
		[self performSelector:_cmd withObject:nil afterDelay:0.0];
	else {
		Engine::ServiceManager::getSingletonPtr()->shutdown();
		delete gameClock;
		
		[NSApp terminate:nil];
	}
}

@end
