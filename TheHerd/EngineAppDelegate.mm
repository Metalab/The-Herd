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
#include "SelectionService.h"
#include "Engine/Clock.h"
#include "Engine/GameObject.h"
#include "Engine/Placeable.h"
#include "Engine/ObjectTextComponent.h"
#include "GameService.h"
#include "EventLogService.h"

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
	sm->registerClock(gameClock);
	Engine::GameObjectService *gameObjectService = new Engine::GameObjectService();
	
	NSError *error;
	NSString *names = [NSString stringWithContentsOfURL:[[NSBundle mainBundle] URLForResource:@"names" withExtension:@"txt" subdirectory:@"media"] encoding:NSUTF8StringEncoding error:&error];
	if(!names)
		NSLog(@"error reading names.txt: %@", error);
	else
		namesList = [names componentsSeparatedByString:@"\n"];
	
	srandomdev();
	
	sm->registerService("input", inputService, -1);
	sm->registerService("ogre", new Engine::OgreService(inputService, gameClock));
	sm->registerService("audio", audioService);
	sm->registerService("rocket", new Engine::RocketService([[resources stringByAppendingPathComponent:@"media/ui"] fileSystemRepresentation]), 1);
	sm->registerService("gameObject", gameObjectService, 2);
	sm->registerService("game", new Game::GameService(gameClock, [NSFullUserName() UTF8String], ^{
		if(!namesList)
			return std::string("<names error>");
		else
			return std::string([[namesList objectAtIndex:random() % [namesList count]] UTF8String]);
	}), 100);
	sm->registerService("selection", new Game::SelectionService(), 101);
	sm->registerService("eventlog", new Game::EventLogService(), 102);
	
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
