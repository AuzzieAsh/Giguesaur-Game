/*
 File: main.m
 Author: Ashley Manson
 Description: Main file for the Giguesaur puzzle game.
 */

// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// OpenGL stuff
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
// My Stuff
#include "headers/std_stuff.h"
#include "headers/Piece.h"

#define NUM_OF_PIECES 4
#define PLUS_ROTATION 15
#define DISTANCE_BEFORE_SNAP 250
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct {
    // top left
    double x0;
    double y0;
    // top right
    double x1;
    double y1;
    // bot right
    double x2;
    double y2;
    // bot left
    double x3;
    double y3;
} Points_Rotated;

// Global Varibles
int holdingPiece = -1;
bool is_connections = false;
bool do_bounding_box = false;
Piece pieces[NUM_OF_PIECES];


void Draw_Piece(Piece piece, bool draw_bounding_box, bool draw_id) {
    
    glShadeModel(GL_SMOOTH);
    int half_length = piece.side_length / 2;
    glPushMatrix();
    glTranslated(piece.x_centre, piece.y_centre, 0.0);
    glRotated(piece.rotation, 0.0, 0.0, 1.0);
    glTranslated(-piece.x_centre, -piece.y_centre, 0.0);
    
    glBegin(GL_POLYGON);
    glVertex2d(piece.x_centre, piece.y_centre);
    glVertex2d(piece.x_centre - half_length, piece.y_centre - half_length);
    if (piece.edges.down_piece >=0) {
        glVertex2d(piece.x_centre - 10, piece.y_centre - half_length);
        glVertex2d(piece.x_centre, piece.y_centre - half_length - 10);
        glVertex2d(piece.x_centre + 10, piece.y_centre - half_length);
    }
    glVertex2d(piece.x_centre + half_length, piece.y_centre - half_length);
    if (piece.edges.right_piece >= 0) {
        glVertex2d(piece.x_centre + half_length, piece.y_centre - 10);
        glVertex2d(piece.x_centre + half_length + 10, piece.y_centre);
        glVertex2d(piece.x_centre + half_length, piece.y_centre + 10);
    }
    glVertex2d(piece.x_centre + half_length, piece.y_centre + half_length);
    if (piece.edges.up_piece >= 0) {
        glVertex2d(piece.x_centre + 10, piece.y_centre + half_length);
        glVertex2d(piece.x_centre, piece.y_centre + half_length - 10);
        glVertex2d(piece.x_centre - 10, piece.y_centre + half_length);
    }
    glVertex2d(piece.x_centre - half_length, piece.y_centre + half_length);
    if (piece.edges.left_piece >= 0) {
        glVertex2d(piece.x_centre - half_length, piece.y_centre + 10);
        glVertex2d(piece.x_centre - half_length + 10, piece.y_centre);
        glVertex2d(piece.x_centre - half_length, piece.y_centre - 10);
    }
    glVertex2d(piece.x_centre - half_length, piece.y_centre - half_length);
    glEnd();
    
    glPopMatrix();
    
    if (draw_bounding_box) {
        glColor4f(1.0f, 0.8f, 0.0f, 0.5f);
        glBegin(GL_LINE_LOOP);
        glVertex2d(piece.x_centre - half_length, piece.y_centre - half_length);
        glVertex2d(piece.x_centre + half_length, piece.y_centre - half_length);
        glVertex2d(piece.x_centre + half_length, piece.y_centre + half_length);
        glVertex2d(piece.x_centre - half_length, piece.y_centre + half_length);
        glVertex2d(piece.x_centre - half_length, piece.y_centre - half_length);
        glEnd();
        
    }
    if (draw_id) {
        int x = piece.x_centre - 5;
        int y = piece.y_centre - 5;
        int letter = piece.piece_id;
        
        glColor4f(1.0f, 0.5f, 0.0f, 0.5f);
        glRasterPos2d(x, y);
        
        int div;
        for (div = 1; div <= letter; div *= 10);
        do {
            div /= 10;
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (letter == 0 ? 0 : (letter / div)) + '0');
            if (letter != 0) letter %= div;
        } while (letter);
    }
}

void Draw_Puzzle_Pieces() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        if (i != holdingPiece) {
            glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
            // Fix out of bounds
            if (pieces[i].x_centre + (pieces[i].side_length / 2) > glutGet(GLUT_WINDOW_WIDTH)) {
                pieces[i].x_centre = glutGet(GLUT_WINDOW_WIDTH) - (pieces[i].side_length / 2);
            }
            else if (pieces[i].x_centre - (pieces[i].side_length / 2) < 0) {
                pieces[i].x_centre = pieces[i].side_length / 2;
            }
            if (pieces[i].y_centre + (pieces[i].side_length / 2) > glutGet(GLUT_WINDOW_HEIGHT)) {
                pieces[i].y_centre = glutGet(GLUT_WINDOW_HEIGHT) - (pieces[i].side_length / 2);
            }
            else if (pieces[i].y_centre - (pieces[i].side_length / 2) < 0) {
                pieces[i].y_centre = pieces[i].side_length / 2;
            }
            while (pieces[i].rotation >= 360) {
                pieces[i].rotation -= 360;
            }
            while (pieces[i].rotation < 0) {
                pieces[i].rotation += 360;
            }
            Draw_Piece(pieces[i], do_bounding_box, true);
        }
    }
    if (holdingPiece >= 0) {
        glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Draw_Piece(pieces[holdingPiece], do_bounding_box, true);
        glDisable(GL_BLEND);
        while (pieces[holdingPiece].rotation >= 360) {
            pieces[holdingPiece].rotation -= 360;
        }
        while (pieces[holdingPiece].rotation < 0) {
            pieces[holdingPiece].rotation += 360;
        }
    }
    
    glFlush();
    glutSwapBuffers();
}

void MakeConnections() {
    if (NUM_OF_PIECES == 4) {
        
        // Hard coded values
        pieces[0].edges.up_piece = -1;
        pieces[0].edges.right_piece = 1;
        pieces[0].edges.down_piece = 2;
        pieces[0].edges.left_piece = -1;
        
        pieces[1].edges.up_piece = -1;
        pieces[1].edges.right_piece = -1;
        pieces[1].edges.down_piece = 3;
        pieces[1].edges.left_piece = 0;
        
        pieces[2].edges.up_piece = 0;
        pieces[2].edges.right_piece = 3;
        pieces[2].edges.down_piece = -1;
        pieces[2].edges.left_piece = -1;
        
        pieces[3].edges.up_piece = 1;
        pieces[3].edges.right_piece = -1;
        pieces[3].edges.down_piece = -1;
        pieces[3].edges.left_piece = 2;
        
        is_connections = true;
    }
    else if (NUM_OF_PIECES == 9) {
        
        // Hard coded values
        pieces[0].edges.up_piece = -1;
        pieces[0].edges.right_piece = 1;
        pieces[0].edges.down_piece = 3;
        pieces[0].edges.left_piece = -1;
        
        pieces[1].edges.up_piece = -1;
        pieces[1].edges.right_piece = 2;
        pieces[1].edges.down_piece = 4;
        pieces[1].edges.left_piece = 0;
        
        pieces[2].edges.up_piece = -1;
        pieces[2].edges.right_piece = -1;
        pieces[2].edges.down_piece = 5;
        pieces[2].edges.left_piece = 1;
        
        pieces[3].edges.up_piece = 0;
        pieces[3].edges.right_piece = 4;
        pieces[3].edges.down_piece = 6;
        pieces[3].edges.left_piece = -1;
        
        pieces[4].edges.up_piece = 1;
        pieces[4].edges.right_piece = 5;
        pieces[4].edges.down_piece = 7;
        pieces[4].edges.left_piece = 3;
        
        pieces[5].edges.up_piece = 2;
        pieces[5].edges.right_piece = -1;
        pieces[5].edges.down_piece = 8;
        pieces[5].edges.left_piece = 4;
        
        pieces[6].edges.up_piece = 3;
        pieces[6].edges.right_piece = 7;
        pieces[6].edges.down_piece = -1;
        pieces[6].edges.left_piece = -1;
        
        pieces[7].edges.up_piece = 4;
        pieces[7].edges.right_piece = 8;
        pieces[7].edges.down_piece = -1;
        pieces[7].edges.left_piece = 6;
        
        pieces[8].edges.up_piece = 5;
        pieces[8].edges.right_piece = -1;
        pieces[8].edges.down_piece = -1;
        pieces[8].edges.left_piece = 7;
 
        is_connections = true;
    }
    else {
        is_connections = false;
        fprintf(stderr, "Cannnot make connections!\n");
    }
    if (is_connections) {
        for (int i = 0; i < NUM_OF_PIECES; i++) {
            Accessible up = 0;
            Accessible down = 0;
            Accessible left = 0;
            Accessible right = 0;
            
            if (pieces[i].edges.up_piece > 0) up = opened;
            else up = invalid;
            if (pieces[i].edges.down_piece > 0) down = opened;
            else down = invalid;
            if (pieces[i].edges.left_piece > 0) left = opened;
            else left = invalid;
            if (pieces[i].edges.right_piece > 0) right = opened;
            else right = invalid;
            
            pieces[i].open_edges.up_open = up;
            pieces[i].open_edges.down_open = down;
            pieces[i].open_edges.left_open = left;
            pieces[i].open_edges.right_open = right;
        }
    }
}

Points_Rotated Get_Rotated_Points(Piece piece) {
    int x = piece.x_centre;
    int y = piece.y_centre;
    int side = piece.side_length;
    
    double theta = (double) piece.rotation * PI / 180.0;
    double xa = x-side/2;
    double ya = y+side/2;
    double xb = x+side/2;
    double yb = y+side/2;
    double xc = x+side/2;
    double yc = y-side/2;
    double xd = x-side/2;
    double yd = y-side/2;
    double xa_new = cos(theta) * (xa - x) - sin(theta) * (ya - y) + x;
    double ya_new = sin(theta) * (xa - x) + cos(theta) * (ya - y) + y;
    double xb_new = cos(theta) * (xb - x) - sin(theta) * (yb - y) + x;
    double yb_new = sin(theta) * (xb - x) + cos(theta) * (yb - y) + y;
    double xc_new = cos(theta) * (xc - x) - sin(theta) * (yc - y) + x;
    double yc_new = sin(theta) * (xc - x) + cos(theta) * (yc - y) + y;
    double xd_new = cos(theta) * (xd - x) - sin(theta) * (yd - y) + x;
    double yd_new = sin(theta) * (xd - x) + cos(theta) * (yd - y) + y;
    Points_Rotated new_piece_location = {.x0 = xa_new,
                                            .y0 = ya_new,
                                            .x1 = xb_new,
                                            .y1 = yb_new,
                                            .x2 = xc_new,
                                            .y2 = yc_new,
                                            .x3 = xd_new,
                                            .y3 = yd_new};
    return new_piece_location;
}

void CheckForConnections(int piece_num) {
    if (piece_num >= 0 && is_connections) {
        
        int up_id = pieces[piece_num].edges.up_piece;
        int right_id = pieces[piece_num].edges.right_piece;
        int down_id = pieces[piece_num].edges.down_piece;
        int left_id = pieces[piece_num].edges.left_piece;
        
        Points_Rotated new_points = Get_Rotated_Points(pieces[piece_num]);
        
        if (up_id >= 0) {
            Points_Rotated up_points = Get_Rotated_Points(pieces[up_id]);
            double distance_1 = pow((new_points.x0 - up_points.x3), 2)
                              + pow((new_points.y0 - up_points.y3), 2);
            double distance_2 = pow((new_points.x1 - up_points.x2), 2)
                              + pow((new_points.y1 - up_points.y2), 2);
            
            if (distance_1 < DISTANCE_BEFORE_SNAP && distance_2 < DISTANCE_BEFORE_SNAP) {
                double rads = pieces[up_id].rotation * PI / 180.0;
                double adj = pieces[up_id].side_length * cos(rads);
                double opp = pieces[up_id].side_length * sin(rads);
                double x_new = pieces[up_id].x_centre + opp;
                double y_new = pieces[up_id].y_centre - adj;
                pieces[piece_num].x_centre = x_new;
                pieces[piece_num].y_centre = y_new;
                pieces[piece_num].rotation = pieces[up_id].rotation;
            }
        }
        if (right_id >= 0) {
            Points_Rotated right_points = Get_Rotated_Points(pieces[right_id]);
            double distance_1 = pow((new_points.x1 - right_points.x0), 2)
                              + pow((new_points.y1 - right_points.y0), 2);
            double distance_2 = pow((new_points.x2 - right_points.x3), 2)
                              + pow((new_points.y2 - right_points.y3), 2);
            
            if (distance_1 < DISTANCE_BEFORE_SNAP && distance_2 < DISTANCE_BEFORE_SNAP) {
                double rads = pieces[right_id].rotation * PI / 180.0;
                double adj = pieces[right_id].side_length * sin(rads);
                double opp = pieces[right_id].side_length * cos(rads);
                double x_new = pieces[right_id].x_centre - opp;
                double y_new = pieces[right_id].y_centre - adj;
                pieces[piece_num].x_centre = x_new;
                pieces[piece_num].y_centre = y_new;
                pieces[piece_num].rotation = pieces[right_id].rotation;
            }
        }
        if (down_id >= 0) {
            Points_Rotated down_points = Get_Rotated_Points(pieces[down_id]);
            double distance_1 = pow((new_points.x3 - down_points.x0), 2)
                              + pow((new_points.y3 - down_points.y0), 2);
            double distance_2 = pow((new_points.x2 - down_points.x1), 2)
                              + pow((new_points.y2 - down_points.y1), 2);
            
            if (distance_1 < DISTANCE_BEFORE_SNAP && distance_2 < DISTANCE_BEFORE_SNAP) {
                double rads = pieces[down_id].rotation * PI / 180.0;
                double adj = pieces[down_id].side_length * cos(rads);
                double opp = pieces[down_id].side_length * sin(rads);
                double x_new = pieces[down_id].x_centre - opp;
                double y_new = pieces[down_id].y_centre + adj;
                pieces[piece_num].x_centre = x_new;
                pieces[piece_num].y_centre = y_new;
                pieces[piece_num].rotation = pieces[down_id].rotation;
            }
        }
        if (left_id >= 0) {
            Points_Rotated left_points = Get_Rotated_Points(pieces[left_id]);
            double distance_1 = pow((new_points.x0 - left_points.x1), 2)
                              + pow((new_points.y0 - left_points.y1), 2);
            double distance_2 = pow((new_points.x3 - left_points.x2), 2)
                              + pow((new_points.y3 - left_points.y2), 2);
            
            if (distance_1 < DISTANCE_BEFORE_SNAP && distance_2 < DISTANCE_BEFORE_SNAP) {
                double rads = pieces[left_id].rotation * PI / 180.0;
                double adj = pieces[left_id].side_length * sin(rads);
                double opp = pieces[left_id].side_length * cos(rads);
                double x_new = pieces[left_id].x_centre + opp;
                double y_new = pieces[left_id].y_centre + adj;
                pieces[piece_num].x_centre = x_new;
                pieces[piece_num].y_centre = y_new;
                pieces[piece_num].rotation = pieces[left_id].rotation;
            }
        }
        
        if (up_id >= 0) {
            double rads = pieces[up_id].rotation * PI / 180.0;
            double adj = pieces[up_id].side_length * cos(rads);
            double opp = pieces[up_id].side_length * sin(rads);
            double x_new = pieces[up_id].x_centre + opp;
            double y_new = pieces[up_id].y_centre - adj;
            
			bool x_true = pieces[piece_num].x_centre - x_new < 1 && pieces[piece_num].x_centre - x_new > -1;
			bool y_true = pieces[piece_num].y_centre - y_new < 1 && pieces[piece_num].y_centre - y_new > -1;
            if (x_true && y_true) {
                pieces[piece_num].open_edges.up_open = closed;
                pieces[up_id].open_edges.down_open = closed;
                printf("Piece %d joined piece %d\n", piece_num, up_id);
			}

        }
        if (right_id >= 0) {
            double rads = pieces[right_id].rotation * PI / 180.0;
            double adj = pieces[right_id].side_length * sin(rads);
            double opp = pieces[right_id].side_length * cos(rads);
            double x_new = pieces[right_id].x_centre - opp;
            double y_new = pieces[right_id].y_centre - adj;

			bool x_true = pieces[piece_num].x_centre - x_new < 1 && pieces[piece_num].x_centre - x_new > -1;
			bool y_true = pieces[piece_num].y_centre - y_new < 1 && pieces[piece_num].y_centre - y_new > -1;
			if (x_true && y_true) {
                pieces[piece_num].open_edges.right_open = closed;
                pieces[right_id].open_edges.left_open = closed;
				printf("Piece %d joined piece %d\n", piece_num, right_id);
			}

        }
        if (down_id >= 0) {
            double rads = pieces[down_id].rotation * PI / 180.0;
            double adj = pieces[down_id].side_length * cos(rads);
            double opp = pieces[down_id].side_length * sin(rads);
            double x_new = pieces[down_id].x_centre - opp;
            double y_new = pieces[down_id].y_centre + adj;

			bool x_true = pieces[piece_num].x_centre - x_new < 1 && pieces[piece_num].x_centre - x_new > -1;
			bool y_true = pieces[piece_num].y_centre - y_new < 1 && pieces[piece_num].y_centre - y_new > -1;
			if (x_true && y_true) {
                pieces[piece_num].open_edges.down_open = closed;
                pieces[down_id].open_edges.up_open = closed;
				printf("Piece %d joined piece %d\n", piece_num, down_id);
			}

        }
        if (left_id >= 0) {
            double rads = pieces[left_id].rotation * PI / 180.0;
            double adj = pieces[left_id].side_length * sin(rads);
            double opp = pieces[left_id].side_length * cos(rads);
            double x_new = pieces[left_id].x_centre + opp;
            double y_new = pieces[left_id].y_centre + adj;

			bool x_true = pieces[piece_num].x_centre - x_new < 1 && pieces[piece_num].x_centre - x_new > -1;
			bool y_true = pieces[piece_num].y_centre - y_new < 1 && pieces[piece_num].y_centre - y_new > -1;
			if (x_true && y_true) {
                pieces[piece_num].open_edges.left_open = closed;
                pieces[left_id].open_edges.right_open = closed;
				printf("Piece %d joined piece %d\n", piece_num, left_id);
			}

        }
    }
}

void CheckIfSolved() {
    bool solved = true;
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        Accessible up = pieces[i].open_edges.up_open;
        Accessible down = pieces[i].open_edges.down_open;
        Accessible left = pieces[i].open_edges.left_open;
        Accessible right = pieces[i].open_edges.right_open;
        if (up == opened) {
            solved = false;
            i = NUM_OF_PIECES;
        }
        if (down == opened) {
            solved = false;
            i = NUM_OF_PIECES;
        }
        if (left == opened) {
            solved = false;
            i = NUM_OF_PIECES;
        }
        if (right == opened) {
            solved = false;
            i = NUM_OF_PIECES;
        }
    }
    if (solved) printf("Solved!\n");
    else printf("Not Solved\n");
}

void Render() {
    Draw_Puzzle_Pieces();
}

void MouseListener(int button, int state, int x, int y) {
    y = glutGet(GLUT_WINDOW_HEIGHT)-y; // Fix Mouse Y
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Place piece back on board if holding a piece
        if (holdingPiece >= 0) {
            pieces[holdingPiece].x_centre = x;
            pieces[holdingPiece].y_centre = y;
            CheckForConnections(holdingPiece);
            holdingPiece = -1;
        }
        else {
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                if(x >= pieces[i].x_centre - (pieces[i].side_length/2) && x < pieces[i].x_centre + (pieces[i].side_length/2)) {
                    if (y >= pieces[i].y_centre - (pieces[i].side_length/2) && y < pieces[i].y_centre + (pieces[i].side_length/2)) {
                        pieces[i].x_centre = x;
                        pieces[i].y_centre = y;
                        holdingPiece = i;
                        if (pieces[i].open_edges.up_open == closed) pieces[i].open_edges.up_open = opened;
                        if (pieces[i].open_edges.down_open == closed) pieces[i].open_edges.down_open = opened;
                        if (pieces[i].open_edges.left_open == closed) pieces[i].open_edges.left_open = opened;
                        if (pieces[i].open_edges.right_open == closed) pieces[i].open_edges.right_open = opened;
                        printf("Picked up piece: %d\n", holdingPiece);
                        i = NUM_OF_PIECES;
                    }
                }
            }
        }
    }
    
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (holdingPiece >= 0) {
            pieces[holdingPiece].rotation += PLUS_ROTATION;
            printf("Rotated piece %d has rotation %.2f\n", holdingPiece, pieces[holdingPiece].rotation);
        }
        else {
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                if(x >= pieces[i].x_centre - (pieces[i].side_length/2) && x < pieces[i].x_centre + (pieces[i].side_length/2)) {
                    if (y >= pieces[i].y_centre - (pieces[i].side_length/2) && y < pieces[i].y_centre + (pieces[i].side_length/2)) {
                        pieces[i].rotation += PLUS_ROTATION;
                        if (pieces[i].open_edges.up_open == closed) pieces[i].open_edges.up_open = opened;
                        if (pieces[i].open_edges.down_open == closed) pieces[i].open_edges.down_open = opened;
                        if (pieces[i].open_edges.left_open == closed) pieces[i].open_edges.left_open = opened;
                        if (pieces[i].open_edges.right_open == closed) pieces[i].open_edges.right_open = opened;
                        printf("Rotated piece %d has rotation %.2f\n", i, pieces[i].rotation);
                        i = NUM_OF_PIECES;
                    }
                }
            }
        }
    }
    Draw_Puzzle_Pieces();
}

void MousePosition(int x, int y) {
    y = glutGet(GLUT_WINDOW_HEIGHT)-y; // Fix Mouse Y
     
    if (holdingPiece >= 0) {
        pieces[holdingPiece].x_centre = x;
        pieces[holdingPiece].y_centre = y;
        Draw_Puzzle_Pieces();
    }
}

void KeyboardListener(unsigned char theKey, int mouseX, int mouseY) {
    
    switch (theKey) {
        case 'p':
        case 'P':
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                printf("%d: x = %.2f, y = %.2f\n", i, pieces[i].x_centre, pieces[i].y_centre);
            }
            break;
        case 'r':
        case 'R':
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                pieces[i].rotation = 0;
            }
            Draw_Puzzle_Pieces();
            break;
        case 't':
        case 'T':
            if (do_bounding_box) do_bounding_box = false;
            else do_bounding_box = true;
            Draw_Puzzle_Pieces();
            break;
        case 32: // space
            CheckIfSolved();
            break;
        case 27: // escape
        case 'q':
        case 'Q':
            exit(0);
            break;
        default:
            break;
    }
}

void WindowResize(int w, int h) {
    Draw_Puzzle_Pieces();
}

int main(int argc, char * argv[]) {
    
    srand((unsigned)time(NULL));
    
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        Piece piece = { .piece_id = i,
                        .x_centre = rand()%SCREEN_WIDTH,
                        .y_centre= rand()%SCREEN_HEIGHT,
                        .side_length = 50,
                        .rotation = 0};
        pieces[i] = piece;
    }
    MakeConnections();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition((1920-SCREEN_WIDTH)/2, 0);
    glutCreateWindow("Giguesaur Alpha");
    
    glutDisplayFunc(Render);
    glutMouseFunc(MouseListener);
    glutPassiveMotionFunc(MousePosition);
    glutKeyboardFunc(KeyboardListener);
    glutReshapeFunc(WindowResize);
    
    // Background Colour
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    
    glutMainLoop();
    
    return 0;
}
