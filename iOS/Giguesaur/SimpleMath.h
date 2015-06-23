/*
    File: SimpleMath.h
    Author: Ashley Manson
 
    Some simple Math stuff that is used alot.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Giguesaur/Piece.h"

#define PI 3.141592653
#define degToRad(DEG) (float)(DEG * PI / 180.0)
#define radToDeg(RAD) (float)(RAD * 180.0 / PI)

typedef enum {P_UP, P_DOWN, P_LEFT, P_RIGHT} pieceCompare;

@interface SimpleMath : NSObject

// Get the rotated corner points of a piece
- (NSArray*) pointsRotated: (struct Piece) piece;

// Get the distance of two pieces
- (NSArray*) distanceBetweenPiece: (struct Piece) originalPiece
                    andOtherPiece: (struct Piece) otherPiece
                      whichPoints: (pieceCompare) sideToCompare;

@end