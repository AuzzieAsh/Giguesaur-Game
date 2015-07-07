/*
    File: Puzzle.h
    Author: Ashley Manson
 
    Defines what a puzzle is, and declares some methods to operate on the puzzle.
 */

#include "Piece.h"

#ifndef Giguesaur_Puzzle_h
#define Giguesaur_Puzzle_h

#define NUM_OF_ROWS 5
#define NUM_OF_COLS 5
#define NUM_OF_PIECES (NUM_OF_ROWS*NUM_OF_COLS)
#define TEXTURE_WIDTH (1.0/NUM_OF_COLS)
#define TEXTURE_HEIGHT (1.0/NUM_OF_ROWS)
#define DISTANCE_BEFORE_SNAP 300
#define NO_NEIGHBOUR -1
#define BOARD_WIDTH 1024
#define BOARD_HIEGHT 768

// Generate each piece of the puzzle
void generatePieces(Piece *pieces);

// Return 1 if the puzzle has been solved, 0 if not
int checkIfSolved(Piece *pieces);

#endif
