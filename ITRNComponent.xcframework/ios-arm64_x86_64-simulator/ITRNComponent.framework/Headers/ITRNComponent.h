//
//  ITRNComponent.h
//  ITRNComponent
//
//  Created by junlei on 2020/6/14.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef void(^SDWebImageNoParamsBlock)(void);
//! Project version number for ITRNComponent.
FOUNDATION_EXPORT double ITRNComponentVersionNumber;

//! Project version string for ITRNComponent.
FOUNDATION_EXPORT const unsigned char ITRNComponentVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <ITRNComponent/PublicHeader.h>
@protocol RCTBridgeModule <NSObject>
+ (NSString *)moduleName;
+ (BOOL)requiresMainQueueSetup;
- (NSDictionary *)constantsToExport;
@end

#define RCT_EXTERN_MODULE(objc_name, objc_supername) \
  RCT_EXTERN_REMAP_MODULE(, objc_name, objc_supername)


#define RCT_REMAP_METHOD(js_name, method)       \
  _RCT_EXTERN_REMAP_METHOD(js_name, method, NO) \
  -(void)method RCT_DYNAMIC;

#define RCT_EXPORT_METHOD(method) RCT_REMAP_METHOD(, method)

#define RCT_EXPORT_MODULE(js_name)          \
  RCT_EXTERN void RCTRegisterModule(Class); \
  +(NSString *)moduleName                   \
  {                                         \
    return @ #js_name;                      \
  }                                         \
  +(void)load                               \
  {                                         \
    RCTRegisterModule(self);                \
  }

#if !defined RCT_DYNAMIC
#if __has_attribute(objc_dynamic)
#define RCT_DYNAMIC __attribute__((objc_dynamic))
#else
#define RCT_DYNAMIC
#endif
#endif


#define RCT_EXPORT_VIEW_PROPERTY(name, type)            \
  +(NSArray<NSString *> *)propConfig_##name RCT_DYNAMIC \
  {                                                     \
    return @[ @ #type ];                                \
  }

/**
 * Like RCT_EXTERN_MODULE, but allows setting a custom JavaScript name.
 */
#if defined(__cplusplus)
#define RCT_EXTERN extern "C" __attribute__((visibility("default")))
#define RCT_EXTERN_C_BEGIN extern "C" {
#define RCT_EXTERN_C_END }
#else
#define RCT_EXTERN extern __attribute__((visibility("default")))
#define RCT_EXTERN_C_BEGIN
#define RCT_EXTERN_C_END
#endif


#define RCT_EXTERN_REMAP_MODULE(js_name, objc_name, objc_supername) \
  objc_name : objc_supername \
  @end \
  @interface objc_name (RCTExternModule) <RCTBridgeModule> \
  @end \
  @implementation objc_name (RCTExternModule) \
  RCT_EXPORT_MODULE_NO_LOAD(js_name, objc_name)

#define RCT_EXPORT_MODULE_NO_LOAD(js_name, objc_name) \
RCT_EXTERN void RCTRegisterModule(Class); \
+ (NSString *)moduleName { return @#js_name; } \
__attribute__((constructor)) static void \
RCT_CONCAT(initialize_, objc_name)() { RCTRegisterModule([objc_name class]); }

#define RCT_CONCAT2(A, B) A ## B
#define RCT_CONCAT(A, B) RCT_CONCAT2(A, B)

#define RCT_EXTERN_METHOD(method) \
_RCT_EXTERN_REMAP_METHOD(, method, NO)

RCT_EXTERN_C_BEGIN

typedef struct RCTMethodInfo {
  const char *const jsName;
  const char *const objcName;
  const BOOL isSync;
} RCTMethodInfo;

RCT_EXTERN_C_END


#define _RCT_EXTERN_REMAP_METHOD(js_name, method, is_blocking_synchronous_method) \
+ (const RCTMethodInfo *)RCT_CONCAT(__rct_export__, RCT_CONCAT(js_name, RCT_CONCAT(__LINE__, __COUNTER__))) { \
  static RCTMethodInfo config = {#js_name, #method, is_blocking_synchronous_method}; \
  return &config; \
}

@class RCTBridge;

@protocol RCTBridgeDelegate <NSObject>
- (NSURL *)sourceURLForBridge:(RCTBridge *)bridge;
@end

typedef NSArray<id<RCTBridgeModule>> * (^RCTBridgeModuleListProvider)(void);

@class RCTDevSettings;

@interface RCTBridge:NSObject

@property (atomic, strong) RCTBridge *batchedBridge;

@property (nonatomic, readonly, getter=isLoading) BOOL loading;

@property (nonatomic, readonly) RCTDevSettings *devSettings;

- (instancetype)initWithDelegate:(id<RCTBridgeDelegate>)delegate
launchOptions:(NSDictionary *)launchOptions;

- (instancetype)initWithBundleURL:(NSURL *)bundleURL
                   moduleProvider:(RCTBridgeModuleListProvider)block
                    launchOptions:(NSDictionary *)launchOptions;

@end

@interface RCTBridge (RNLoadJS) // RN私有类 ，这里暴露他的接口
- (void)executeSourceCode:(NSData *)sourceCode sync:(BOOL)sync;
@end


@interface RCTRootView : UIView
- (instancetype)initWithBridge:(RCTBridge *)bridge
                    moduleName:(NSString *)moduleName
             initialProperties:(nullable NSDictionary *)initialProperties NS_DESIGNATED_INITIALIZER;
@end

@interface RCTEventEmitter : NSObject <RCTBridgeModule>

@property (nonatomic, weak) RCTBridge *bridge;

- (NSArray<NSString *> *)supportedEvents;

- (void)sendEventWithName:(NSString *)name body:(id)body;

- (void)startObserving;
- (void)stopObserving;

- (void)addListener:(NSString *)eventName;
- (void)removeListeners:(double)count;

@end

@interface RCTDevSettings : RCTEventEmitter
/*
 * Whether shaking will show RCTDevMenu. The menu is enabled by default if RCT_DEV=1, but
 * you may wish to disable it so that you can provide your own shake handler.
 */
@property (nonatomic, assign) BOOL isShakeToShowDevMenuEnabled;
@end


@class RCTShadowView;


@interface RCTViewManager : NSObject <RCTBridgeModule>

@property (nonatomic, weak) RCTBridge *bridge;

- (UIView *)view;

- (RCTShadowView *)shadowView;

@end


typedef void (^RCTPromiseResolveBlock)(id result);

typedef void (^RCTPromiseRejectBlock)(NSString *code, NSString *message, NSError *error);

@interface RCTBundleURLProvider : NSObject
@property (nonatomic, copy) NSString *jsLocation;
+ (instancetype)sharedSettings;
- (NSURL *)jsBundleURLForBundleRoot:(NSString *)bundleRoot
fallbackResource:(NSString *)resourceName;
@end

NS_ASSUME_NONNULL_BEGIN

@interface ITRNComponent : NSObject
+ (NSURL *)jsbundle;
@end
// cache
@interface ITRNCacheManager : NSObject
+ (NSUInteger)getCacheSize;
+ (void)clearCacheOnCompletion:(nullable SDWebImageNoParamsBlock)completion;
@end

NS_ASSUME_NONNULL_END
