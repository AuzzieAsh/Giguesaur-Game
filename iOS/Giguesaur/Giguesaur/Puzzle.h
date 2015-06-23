/*
    File: Puzzle.h
    Author: Ashley Manson
 
    Header File for what a puzzle is.
 */

#ifndef Giguesaur_Puzzle_h
#define Giguesaur_Puzzle_h

#include "Piece.h"

#define NUM_OF_ROWS 2
#define NUM_OF_COLS 2
#define NUM_OF_PIECES (NUM_OF_ROWS*NUM_OF_COLS)
#define DISTANCE_BEFORE_SNAP 0
// iPad 2 Simulator size, W=1024, H=768
#define BOARD_WIDTH 1024
#define BOARD_HIEGHT 768

struct Piece pieces[NUM_OF_PIECES];

#endif
