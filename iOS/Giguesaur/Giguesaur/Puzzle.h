/*
    File: Puzzle.h
    Author: Ashley Manson
 
    Defines what a puzzle is, and declares some methods to generate the puzzle.
 */

#include "Piece.h"

#ifndef Giguesaur_Puzzle_h
#define Giguesaur_Puzzle_h

#define NUM_OF_ROWS 2
#define NUM_OF_COLS 2
#define NUM_OF_PIECES (NUM_OF_ROWS*NUM_OF_COLS)
#define DISTANCE_BEFORE_SNAP 500
#define NO_NEIGHBOUR -1
// iPad 2 Simulator size, W=1024, H=768
#define BOARD_WIDTH 1024
#define BOARD_HIEGHT 768

// Generate each piece of the puzzle, calls makeConnections
void generatePieces(Piece *pieces);
// Assigns neigbours to each piece
void makeConnections(Piece *pieces);

#endif
