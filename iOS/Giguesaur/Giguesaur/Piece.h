/*
    File: Piece.h
    Author: Ashley Manson
 
    Header File for what a piece is.
 */

#ifndef Giguesaur_Piece_h
#define Giguesaur_Piece_h

#define SIDE_LENGTH 50
#define SIDE_HALF (SIDE_LENGTH/2)

struct Piece{
    int piece_id;
    double x_location;
    double y_location;
    double rotation;
    
    struct {
        int up_piece;
        int down_piece;
        int left_piece;
        int right_piece;
    } Edge_piece;
    
    struct {
        BOOL up_open;
        BOOL down_open;
        BOOL left_open;
        BOOL right_open;
    } Edge_open;
    
};

#endif
