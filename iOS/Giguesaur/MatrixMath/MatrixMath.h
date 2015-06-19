/*
    File: MatrixMath.h
    Author: Ashley Manson
 
    Description: Some matrix stuff, and math functions for them.
                For health and saftey reasons, all inputed arrays should be of
                size 16, or it might cause a black hole. You've been warned.
 */

#import <Foundation/Foundation.h>
#include <OpenGLES/ES2/gl.h>

#define PI 3.141592653
#define degToRad(DEG) (GLfloat)(DEG * PI / 180.0)
//#define radToDeg(RAD) (GLfloat)(RAD * 180.0 / PI)

@interface MatrixMath : NSObject

// Saves an identity matrix in the inputed array.
- (void) identityMatrix: (GLfloat*) matOut;

// Creates and stores a projection matrix in the inputed array.
- (void) projectionFromFrustum: (GLfloat*) matOut
                       andLeft: (GLfloat) left
                      andRight: (GLfloat) right
                     andBottom: (GLfloat) bottom
                        andTop: (GLfloat) top
                       andNear: (GLfloat) near
                        andFar: (GLfloat) far;
/*
// Creates and stores a projection matrix in the inputed array.
- (void) projectionFromPerspective: (GLfloat*) matOut
                            andFOV: (GLfloat) fov
                         andAspect: (GLfloat) aspect
                           andNear: (GLfloat) near
                            andFar: (GLfloat) far;
*/
// Creates and stores a translation matrix in the inputed array.
- (void) translationMatrix: (GLfloat*) matOut
                    alongX: (GLfloat) x
                    alongY: (GLfloat) y
                    alongZ: (GLfloat) z;

// Creates and stores a rotation matrix in the inputed array.
// x, y, z should be in degrees.
- (void) rotationMatrix: (GLfloat*) matOut
                aroundX: (GLfloat) x
                aroundY: (GLfloat) y
                aroundZ: (GLfloat) z;

// Allows two matrices to be multplied together, output stored in inputed array.
- (void) multiplyMatrix: (GLfloat*) matOut
                  first: (GLfloat*) m1
                 second: (GLfloat*) m2;
@end
