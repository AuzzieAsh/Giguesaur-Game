/*
    File: OpenGLView.h
    Author: Ashley Manson
 
    Description: OpenGL stuff.
 */

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import <GLKit/GLKit.h>

@interface OpenGLView : UIView {
    
    CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
    
    GLuint _colorRenderBuffer;
    GLuint _positionSlot;
    GLuint _colorSlot;
    GLuint _projectionUniform;
    GLuint _modelViewUniform;
    GLuint _depthRenderBuffer;
    
    float _currentRotation;
    
}

@end
