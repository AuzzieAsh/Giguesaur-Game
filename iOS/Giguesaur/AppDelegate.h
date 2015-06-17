/*
    File: AppDelegate.h
    Author: Ashley Manson
 
    Description: AppDelegate does delegate things.
 */

#import <UIKit/UIKit.h>
#import "OpenGLView.h"

@interface AppDelegate : NSObject <UIApplicationDelegate> {
    OpenGLView* _glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet OpenGLView *glView;

@end

