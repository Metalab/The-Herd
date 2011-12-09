//
//  EngineAppDelegate.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#import <Cocoa/Cocoa.h>

namespace Engine {
	class Clock;
	class GameObject;
}

@interface EngineAppDelegate : NSObject <NSApplicationDelegate> {
	Engine::Clock *gameClock;
	Engine::GameObject *head;
}

@end
