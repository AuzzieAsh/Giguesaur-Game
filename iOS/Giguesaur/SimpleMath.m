/*
    File: SimpleMath.h
    Author: Ashley Manson
 
    Implementation of SimpleMath.h
 */

#import "SimpleMath.h"

@implementation SimpleMath

- (NSArray*) pointsRotated: (struct Piece) piece {
    
    int x = piece.x_location;
    int y = piece.y_location;
    
    float theta = degToRad(piece.rotation);
    
    // Top Left, index 0
    float xa = x - SIDE_HALF;
    float ya = y + SIDE_HALF;
    float xa_new = cos(theta) * (xa - x) - sin(theta) * (ya - y) + x;
    float ya_new = sin(theta) * (xa - x) + cos(theta) * (ya - y) + y;
    
    // Top Right, index 1
    float xb = x + SIDE_HALF;
    float yb = y + SIDE_HALF;
    float xb_new = cos(theta) * (xb - x) - sin(theta) * (yb - y) + x;
    float yb_new = sin(theta) * (xb - x) + cos(theta) * (yb - y) + y;
    
    // Bot Right, index 2
    float xc = x + SIDE_HALF;
    float yc = y - SIDE_HALF;
    float xc_new = cos(theta) * (xc - x) - sin(theta) * (yc - y) + x;
    float yc_new = sin(theta) * (xc - x) + cos(theta) * (yc - y) + y;
    
    // Bot Left, index 3
    float xd = x - SIDE_HALF;
    float yd = y - SIDE_HALF;
    float xd_new = cos(theta) * (xd - x) - sin(theta) * (yd - y) + x;
    float yd_new = sin(theta) * (xd - x) + cos(theta) * (yd - y) + y;
    
    NSArray *newPoints = [NSArray arrayWithObjects:
                          [NSValue valueWithCGPoint:CGPointMake(xa_new, ya_new)],
                          [NSValue valueWithCGPoint:CGPointMake(xb_new, yb_new)],
                          [NSValue valueWithCGPoint:CGPointMake(xc_new, yc_new)],
                          [NSValue valueWithCGPoint:CGPointMake(xd_new, yd_new)],
                          nil];
    return newPoints;
}

- (NSArray*) distanceBetweenPiece: (struct Piece) originalPiece
                    andOtherPiece: (struct Piece) otherPiece
                      whichPoints: (pieceCompare) sideToCompare {
    
    NSArray *originalPieceRotated = [self pointsRotated:originalPiece];
    NSArray *otherPieceRotated = [self pointsRotated:otherPiece];
    
    CGPoint pieceTopLeft = [[originalPieceRotated objectAtIndex:0] CGPointValue];
    CGPoint pieceTopRight = [[originalPieceRotated objectAtIndex:1] CGPointValue];
    CGPoint pieceBotRight = [[originalPieceRotated objectAtIndex:2] CGPointValue];
    CGPoint pieceBotLeft = [[originalPieceRotated objectAtIndex:3] CGPointValue];
    
    float distance_1 = -1, distance_2 = -1;
    
    if (sideToCompare == P_UP) {
        CGPoint upBotLeft = [[otherPieceRotated objectAtIndex:3] CGPointValue];
        CGPoint upBotRight = [[otherPieceRotated objectAtIndex:2] CGPointValue];
        
        distance_1 =
            powf((pieceTopLeft.x - upBotLeft.x), 2) +
            powf((pieceTopLeft.y - upBotLeft.y), 2);
        
        distance_2 =
            powf((pieceTopRight.x - upBotRight.x), 2) +
            powf((pieceTopRight.y - upBotRight.y), 2);
    }
    
    else if (sideToCompare == P_DOWN) {
        CGPoint downTopLeft = [[otherPieceRotated objectAtIndex:0] CGPointValue];
        CGPoint downTopRight = [[otherPieceRotated objectAtIndex:1] CGPointValue];
        
        distance_1 =
            powf((pieceBotLeft.x - downTopLeft.x), 2) +
            powf((pieceBotLeft.y - downTopLeft.y), 2);
        
        distance_2 =
            powf((pieceBotRight.x - downTopRight.x), 2) +
            powf((pieceBotRight.y - downTopRight.y), 2);
    }
    
    else if (sideToCompare == P_LEFT) {
        CGPoint leftTopRight = [[otherPieceRotated objectAtIndex:1] CGPointValue];
        CGPoint leftBotRight = [[otherPieceRotated objectAtIndex:2] CGPointValue];
        
        distance_1 =
            powf((pieceTopLeft.x - leftTopRight.x), 2) +
            powf((pieceTopLeft.y - leftTopRight.y), 2);
        
        distance_2 =
            powf((pieceBotLeft.x - leftBotRight.x), 2) +
            powf((pieceBotLeft.y - leftBotRight.y), 2);
    }
    
    else if (sideToCompare == P_RIGHT) {
        CGPoint rightTopLeft = [[otherPieceRotated objectAtIndex:0] CGPointValue];
        CGPoint rightBotLeft = [[otherPieceRotated objectAtIndex:3] CGPointValue];
        
        distance_1 =
            powf((pieceTopRight.x - rightTopLeft.x), 2) +
            powf((pieceTopRight.y - rightTopLeft.y), 2);
        
        distance_2 =
            powf((pieceBotRight.x - rightBotLeft.x), 2) +
            powf((pieceBotRight.y - rightBotLeft.y), 2);
    }
    
    NSArray *distances = [NSArray arrayWithObjects:
                          [NSNumber numberWithFloat:distance_1],
                          [NSNumber numberWithFloat:distance_2],
                          nil];
    
    return distances;
}

@end
