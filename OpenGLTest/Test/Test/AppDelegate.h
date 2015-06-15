//
//  AppDelegate.h
//  Test
//
//  Created by Local Ash on 6/15/15.
//  Copyright (c) 2015 Local Ash. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "OpenGLView.h"

@interface AppDelegate : NSObject <UIApplicationDelegate> {
    OpenGLView* _glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet OpenGLView *glView;


@end

