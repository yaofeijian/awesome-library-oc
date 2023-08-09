
#ifdef RCT_NEW_ARCH_ENABLED
#import "RNAwesomeLibraryOcSpec.h"

@interface AwesomeLibraryOc : NSObject <NativeAwesomeLibraryOcSpec>
#else
#import <React/RCTBridgeModule.h>

@interface AwesomeLibraryOc : NSObject <RCTBridgeModule>
#endif

@end
