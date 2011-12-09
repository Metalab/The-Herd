//|||||||||||||||||||||||||||||||||||||||||||||||

#import "EngineAppDelegate.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

int main(int argc, char **argv) {
	@autoreleasepool {
		[[NSApplication sharedApplication] setDelegate:[[EngineAppDelegate alloc] init]];
		return NSApplicationMain(argc, (const char **) argv);
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||
