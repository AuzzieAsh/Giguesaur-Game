/*
    File: MatrixMath.m
    Author: Ashley Manson
 
    Description: Implementation of the MatrixMath.h file.
 */

#import "MatrixMath.h"

#define PI 3.141592653
#define degToRad(DEG) (GLfloat)(DEG * PI / 180.0)
//#define radToDeg(RAD) (GLfloat)(RAD * 180.0 / PI)

@implementation MatrixMath

- (void) identityMatrix: (GLfloat*) matOut {
    
    matOut[0] = 1.0;
    matOut[1] = 0.0;
    matOut[2] = 0.0;
    matOut[3] = 0.0;
    
    matOut[4] = 0.0;
    matOut[5] = 1.0;
    matOut[6] = 0.0;
    matOut[7] = 0.0;
    
    matOut[8] = 0.0;
    matOut[9] = 0.0;
    matOut[10] = 1.0;
    matOut[11] = 0.0;
    
    matOut[12] = 0.0;
    matOut[13] = 0.0;
    matOut[14] = 0.0;
    matOut[15] = 1.0;
}

- (void) projectionFromFrustum: (GLfloat*) matOut
                       andLeft: (GLfloat) left
                      andRight: (GLfloat) right
                     andBottom: (GLfloat) bottom
                        andTop: (GLfloat) top
                       andNear: (GLfloat) near
                        andFar: (GLfloat) far {
 
    matOut[0] = (2*near)/(right-left);
    matOut[1] = 0;
    matOut[2] = (right+left)/(right-left);
    matOut[3] = 0;
    
    matOut[4] = 0;
    matOut[5] = (2*near)/(top-bottom);
    matOut[6] = (top+bottom)/(top-bottom);
    matOut[7] = 0;
    
    matOut[8] = 0;
    matOut[9] = 0;
    matOut[10] = -(far+near)/(far-near);
    matOut[11] = -2*(far*near)/(far-near);
    
    matOut[12] = 0;
    matOut[13] = 0;
    matOut[14] = -1;
    matOut[15] = 0;
}

- (void) translationMatrix: (GLfloat*) matOut
                    alongX: (GLfloat) x
                    alongY: (GLfloat) y
                    alongZ: (GLfloat) z {

    matOut[0] = 1.0;
    matOut[1] = 0.0;
    matOut[2] = 0.0;
    matOut[3] = 0.0;
    
    matOut[4] = 0.0;
    matOut[5] = 1.0;
    matOut[6] = 0.0;
    matOut[7] = 0.0;
    
    matOut[8] = 0.0;
    matOut[9] = 0.0;
    matOut[10] = 1.0;
    matOut[11] = 0.0;
    
    matOut[12] = x;
    matOut[13] = y;
    matOut[14] = z;
    matOut[15] = 1.0;
}

- (void) rotationMatrix: (GLfloat*) matOut
                aroundX: (GLfloat) x
                aroundY: (GLfloat) y
                aroundZ: (GLfloat) z {
    
    GLfloat xRadians = degToRad(x);
    GLfloat yRadians = degToRad(y);
    GLfloat zRadians = degToRad(z);
    
    GLfloat cx = cosf(xRadians);
    GLfloat sx = sinf(xRadians);
    GLfloat cy = cosf(yRadians);
    GLfloat sy = sinf(yRadians);
    GLfloat cz = cosf(zRadians);
    GLfloat sz = sinf(zRadians);

    matOut[0] = (cy * cz) + (sx * sy * sz);
    matOut[1] = cx * sz;
    matOut[2] = (cy * sx * sz) - (cz * sy);
    matOut[3] = 0.0;
    
    matOut[4] = (cz * sx * sy) - (cy * sz);
    matOut[5] = cx * cz;
    matOut[6] = (cy * cz * sx) + (sy * sz);
    matOut[7] = 0.0;
    
    matOut[8] = cx * sy;
    matOut[9] = -sx;
    matOut[10] = cx * cy;
    matOut[11] = 0.0;
    
    matOut[12] = 0.0;
    matOut[13] = 0.0;
    matOut[14] = 0.0;
    matOut[15] = 1.0;
}

- (void) multiplyMatrix: (GLfloat*) matOut
                  first: (GLfloat*) m1
                 second: (GLfloat*) m2 {
    
    matOut[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    matOut[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    matOut[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    matOut[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
    
    matOut[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    matOut[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    matOut[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    matOut[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
    
    matOut[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    matOut[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    matOut[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    matOut[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
    
    matOut[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    matOut[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    matOut[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    matOut[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
}


@end
