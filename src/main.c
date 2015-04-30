//
//  main.c
//  Jigsaw
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "headers/stdfun.h"

#define NUM_OF_PIECES 9
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct {
    int up_Piece;
    int down_Piece;
    int left_Piece;
    int right_Piece;
} Edge;

typedef struct {
    int up_open;
    int down_open;
    int left_open;
    int right_open;
} OpenEdges;

typedef struct {
    int pieceID;
    GLint x_Location;
    GLint y_Location;
    int size;
    int rotation;
    Edge edges;
    OpenEdges open_edges;
} Piece;

typedef struct {
    double x0;
    double y0;
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
} Rotation_Location;

// Global Varibles
int holdingPiece = -1;
bool is_connections = false;
bool do_bounding_box = false;
Piece pieces[NUM_OF_PIECES];
int plus_rotation = 15;

void DrawPiece(Piece piece) {
    glPushMatrix();
    glTranslated(piece.x_Location + (piece.size /2), piece.y_Location + (piece.size /2), 0.0);
    glRotated(piece.rotation, 0.0, 0.0, 1.0);
    glTranslated(-(piece.x_Location + (piece.size /2)), -(piece.y_Location + (piece.size /2)), 0.0);
    
    glBegin(GL_POLYGON);
    glVertex2i(piece.x_Location + piece.size/2, piece.y_Location + piece.size/2);
    glVertex2i(piece.x_Location, piece.y_Location);
    if (piece.edges.down_Piece >=0) {
        glVertex2i(piece.x_Location + (piece.size/2)-10, piece.y_Location);
        glVertex2i(piece.x_Location + (piece.size/2), piece.y_Location-10);
        glVertex2i(piece.x_Location + (piece.size/2)+10, piece.y_Location);
    }
    glVertex2i(piece.x_Location + piece.size, piece.y_Location);
    if (piece.edges.right_Piece >= 0) {
        glVertex2i(piece.x_Location + piece.size, piece.y_Location + piece.size/2-10);
        glVertex2i(piece.x_Location + piece.size+10, piece.y_Location + piece.size/2);
        glVertex2i(piece.x_Location + piece.size, piece.y_Location + piece.size/2+10);
    }
    glVertex2i(piece.x_Location + piece.size, piece.y_Location + piece.size);
    if (piece.edges.up_Piece >= 0) {
        glVertex2i(piece.x_Location + piece.size/2+10, piece.y_Location + piece.size);
        glVertex2i(piece.x_Location + piece.size/2, piece.y_Location + piece.size-10);
        glVertex2i(piece.x_Location + piece.size/2-10, piece.y_Location + piece.size);
    }
    glVertex2i(piece.x_Location, piece.y_Location + piece.size);
    if (piece.edges.left_Piece >= 0) {
        glVertex2i(piece.x_Location, piece.y_Location + piece.size/2+10);
        glVertex2i(piece.x_Location + 10, piece.y_Location + piece.size/2);
        glVertex2i(piece.x_Location, piece.y_Location + piece.size/2-10);
    }
    glVertex2i(piece.x_Location, piece.y_Location);
    glEnd();
    glPopMatrix();
    if (do_bounding_box) {
        glColor4f(1.0f, 0.8f, 0.0f, 0.5f);
        glBegin(GL_LINE_LOOP);
        glVertex2i(piece.x_Location, piece.y_Location);
        glVertex2i(piece.x_Location + piece.size, piece.y_Location);
        glVertex2i(piece.x_Location + piece.size, piece.y_Location + piece.size);
        glVertex2i(piece.x_Location, piece.y_Location + piece.size);
        glVertex2i(piece.x_Location, piece.y_Location);
        glEnd();

    }
}

void DrawLetter(Piece piece) {
    int x = piece.x_Location + (piece.size/2)-5;
    int y = piece.y_Location + (piece.size/2)-5;
    int letter = piece.pieceID;
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

void DrawPuzzlePieces() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        if (i != holdingPiece) {
            glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
            // Fix out of bounds
            if (pieces[i].x_Location + pieces[i].size > glutGet(GLUT_WINDOW_WIDTH)) {
                pieces[i].x_Location = glutGet(GLUT_WINDOW_WIDTH) - pieces[i].size;
            }
            else if (pieces[i].x_Location < 0) {
                pieces[i].x_Location = 0;
            }
            if (pieces[i].y_Location + pieces[i].size > glutGet(GLUT_WINDOW_HEIGHT)) {
                pieces[i].y_Location = glutGet(GLUT_WINDOW_HEIGHT) - pieces[i].size;
            }
            else if (pieces[i].y_Location < 0) {
                pieces[i].y_Location = 0;
            }
            while (pieces[i].rotation >= 360) {
                pieces[i].rotation -= 360;
            }
            while (pieces[i].rotation < 0) {
                pieces[i].rotation += 360;
            }
            DrawPiece(pieces[i]);
            DrawLetter(pieces[i]);
        }
    }
    if (holdingPiece >= 0) {
        glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        DrawPiece(pieces[holdingPiece]);
        DrawLetter(pieces[holdingPiece]);
        glDisable(GL_BLEND);
        while (pieces[holdingPiece].rotation >= 360) {
            pieces[holdingPiece].rotation -= 360;
        }
        while (pieces[holdingPiece].rotation < 0) {
            pieces[holdingPiece].rotation += 360;
        }
    }
    
    glFlush();
}

void MakeConnections() {
    if (NUM_OF_PIECES == 4) {
        
        // Hard coded values
        pieces[0].edges.up_Piece = -1;
        pieces[0].edges.right_Piece = 1;
        pieces[0].edges.down_Piece = 2;
        pieces[0].edges.left_Piece = -1;
        
        pieces[1].edges.up_Piece = -1;
        pieces[1].edges.right_Piece = -1;
        pieces[1].edges.down_Piece = 3;
        pieces[1].edges.left_Piece = 0;
        
        pieces[2].edges.up_Piece = 0;
        pieces[2].edges.right_Piece = 3;
        pieces[2].edges.down_Piece = -1;
        pieces[2].edges.left_Piece = -1;
        
        pieces[3].edges.up_Piece = 1;
        pieces[3].edges.right_Piece = -1;
        pieces[3].edges.down_Piece = -1;
        pieces[3].edges.left_Piece = 2;
        
        is_connections = true;
    }
    else if (NUM_OF_PIECES == 9) {
        
        // Hard coded values
        pieces[0].edges.up_Piece = -1;
        pieces[0].edges.right_Piece = 1;
        pieces[0].edges.down_Piece = 3;
        pieces[0].edges.left_Piece = -1;
        
        pieces[1].edges.up_Piece = -1;
        pieces[1].edges.right_Piece = 2;
        pieces[1].edges.down_Piece = 4;
        pieces[1].edges.left_Piece = 0;
        
        pieces[2].edges.up_Piece = -1;
        pieces[2].edges.right_Piece = -1;
        pieces[2].edges.down_Piece = 5;
        pieces[2].edges.left_Piece = 1;
        
        pieces[3].edges.up_Piece = 0;
        pieces[3].edges.right_Piece = 4;
        pieces[3].edges.down_Piece = 6;
        pieces[3].edges.left_Piece = -1;
        
        pieces[4].edges.up_Piece = 1;
        pieces[4].edges.right_Piece = 5;
        pieces[4].edges.down_Piece = 7;
        pieces[4].edges.left_Piece = 3;
        
        pieces[5].edges.up_Piece = 2;
        pieces[5].edges.right_Piece = -1;
        pieces[5].edges.down_Piece = 8;
        pieces[5].edges.left_Piece = 4;
        
        pieces[6].edges.up_Piece = 3;
        pieces[6].edges.right_Piece = 7;
        pieces[6].edges.down_Piece = -1;
        pieces[6].edges.left_Piece = -1;
        
        pieces[7].edges.up_Piece = 4;
        pieces[7].edges.right_Piece = 8;
        pieces[7].edges.down_Piece = -1;
        pieces[7].edges.left_Piece = 6;
        
        pieces[8].edges.up_Piece = 5;
        pieces[8].edges.right_Piece = -1;
        pieces[8].edges.down_Piece = -1;
        pieces[8].edges.left_Piece = 7;
 
        is_connections = true;
    }
    else {
        is_connections = false;
        fprintf(stderr, "Cannnot make connections!\n");
    }
    if (is_connections) {
        for (int i = 0; i < NUM_OF_PIECES; i++) {
            bool up = 0;
            bool down = 0;
            bool left = 0;
            bool right = 0;
            
            if (pieces[i].edges.up_Piece > 0) up = 1;
            else up = -1;
            if (pieces[i].edges.down_Piece > 0) down = 1;
            else down = -1;
            if (pieces[i].edges.left_Piece > 0) left = 1;
            else left = -1;
            if (pieces[i].edges.right_Piece > 0) right = 1;
            else right = -1;
            
            pieces[i].open_edges.up_open = up;
            pieces[i].open_edges.down_open = down;
            pieces[i].open_edges.left_open = left;
            pieces[i].open_edges.right_open = right;
        }
    }
}

Rotation_Location Get_New_Rotation(Piece piece) {
    int x = piece.x_Location + (piece.size/2);
    int y = piece.y_Location + (piece.size/2);
    int side = piece.size;
    
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
    Rotation_Location new_piece_location = {.x0 = xa_new,
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
        
        int up_p = pieces[piece_num].edges.up_Piece;
        int right_p = pieces[piece_num].edges.right_Piece;
        int down_p = pieces[piece_num].edges.down_Piece;
        int left_p = pieces[piece_num].edges.left_Piece;
        
        int x1, x2, y1, y2;
        int distance = pieces[piece_num].size/2;
        
        Rotation_Location orginal_piece = Get_New_Rotation(pieces[piece_num]);
        
        if (up_p >= 0) {
            Rotation_Location up_piece = Get_New_Rotation(pieces[up_p]);
            x1 = orginal_piece.x0;
            x2 = up_piece.x3;
            y1 = orginal_piece.y0;
            y2 = up_piece.y3;
            double distance_1 = pow((x1 - x2), 2)+ pow((y1 - y2), 2);
            x1 = orginal_piece.x1;
            x2 = up_piece.x2;
            y1 = orginal_piece.y1;
            y2 = up_piece.y2;
            double distance_2 = pow((x1 - x2), 2)+ pow((y1 - y2), 2);
            
            if (distance_1 < 200 && distance_2 < 200) {
                double rads = pieces[up_p].rotation * PI / 180.0;
                double adj = pieces[up_p].size * cos(rads);
                double opp = pieces[up_p].size * sin(rads);
                int x_new = up_piece.x3 + opp;
                int y_new = up_piece.y3 - adj;
                
                int x = pieces[up_p].x_Location + (pieces[up_p].size/2);
                int y = pieces[up_p].y_Location + (pieces[up_p].size/2);
                x = x + pieces[up_p].size * sin(rads);
                y = y - pieces[up_p].size * cos(rads);
                double x_t = cos(-rads) * (x_new - x) - sin(-rads) * (y_new - y) + x;
                double y_t = sin(-rads) * (x_new - x) + cos(-rads) * (y_new - y) + y;
                pieces[piece_num].x_Location = x_t;
                pieces[piece_num].y_Location = y_t;
                pieces[piece_num].rotation = pieces[up_p].rotation;
            }
        }
        if (right_p >= 0) {
            x1 = pieces[piece_num].x_Location + pieces[piece_num].size;
            x2 = pieces[right_p].x_Location;
            y1 = pieces[piece_num].y_Location;
            y2 = pieces[right_p].y_Location;
            
            if (x1 - x2 < distance && x1 - x2 > -distance) {
                if (y1 - y2 < distance && y1 - y2 > -distance) {
                    printf("Piece %d joined piece %d\n", piece_num, right_p);
                    pieces[piece_num].x_Location = pieces[right_p].x_Location - pieces[piece_num].size;
                    pieces[piece_num].y_Location = pieces[right_p].y_Location;
                    
                    pieces[piece_num].open_edges.right_open = 0;
                    pieces[right_p].open_edges.left_open = 0;
                }
            }

        }
        if (down_p >= 0) {
            x1 = pieces[piece_num].x_Location;
            x2 = pieces[down_p].x_Location;
            y1 = pieces[piece_num].y_Location;
            y2 = pieces[down_p].y_Location + pieces[down_p].size;
            
            if (x1 - x2 < distance && x1 - x2 > -distance) {
                if (y1 - y2 < distance && y1 - y2 > -distance) {
                    printf("Piece %d joined piece %d\n", piece_num, down_p);
                    pieces[piece_num].x_Location = pieces[down_p].x_Location;
                    pieces[piece_num].y_Location = pieces[down_p].y_Location + pieces[piece_num].size;
                    
                    pieces[piece_num].open_edges.down_open = 0;
                    pieces[down_p].open_edges.up_open = 0;
                }
            }
        }
        if (left_p >= 0) {
            x1 = pieces[piece_num].x_Location;
            x2 = pieces[left_p].x_Location + pieces[left_p].size;
            y1 = pieces[piece_num].y_Location;
            y2 = pieces[left_p].y_Location;
            
            if (x1 - x2 < distance && x1 - x2 > -distance) {
                if (y1 - y2 < distance && y1 - y2 > -distance) {
                    printf("Piece %d joined piece %d\n", piece_num, left_p);
                    pieces[piece_num].x_Location = pieces[left_p].x_Location + pieces[piece_num].size;
                    pieces[piece_num].y_Location = pieces[left_p].y_Location;
                    
                    pieces[piece_num].open_edges.left_open = 0;
                    pieces[left_p].open_edges.right_open = 0;
                }
            }
        }
    }
}

void CheckIfSolved() {
    bool solved = true;
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        int up = pieces[i].open_edges.up_open;
        int down = pieces[i].open_edges.down_open;
        int left = pieces[i].open_edges.left_open;
        int right = pieces[i].open_edges.right_open;
        if (up == 1) {
            solved = false;
            i = NUM_OF_PIECES;
        }
        if (down == 1) {
            solved = false;
            i = NUM_OF_PIECES;
        }
        if (left == 1) {
            solved = false;
            i = NUM_OF_PIECES;
        }
        if (right == 1) {
            solved = false;
            i = NUM_OF_PIECES;
        }
    }
    if (solved) printf("Solved!\n");
    else printf("Not Solved\n");
}

void Render() {
    DrawPuzzlePieces();
}

void MouseListener(int button, int state, int x, int y) {
    y = glutGet(GLUT_WINDOW_HEIGHT)-y; // Fix Mouse Y
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Place piece back on board if holding a piece
        if (holdingPiece >= 0) {
            pieces[holdingPiece].x_Location = x - (pieces[holdingPiece].size/2);
            pieces[holdingPiece].y_Location = y - (pieces[holdingPiece].size/2);
            CheckForConnections(holdingPiece);
            holdingPiece = -1;
        }
        else {
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                if(x >= pieces[i].x_Location && x < pieces[i].x_Location + pieces[i].size) {
                    if (y >= pieces[i].y_Location && y < pieces[i].y_Location + pieces[i].size) {
                        pieces[i].x_Location = x - (pieces[i].size/2);
                        pieces[i].y_Location = y - (pieces[i].size/2);
                        holdingPiece = i;
                        if (pieces[i].open_edges.up_open == 0) pieces[i].open_edges.up_open = 1;
                        if (pieces[i].open_edges.down_open == 0) pieces[i].open_edges.down_open = 1;
                        if (pieces[i].open_edges.left_open == 0) pieces[i].open_edges.left_open = 1;
                        if (pieces[i].open_edges.right_open == 0) pieces[i].open_edges.right_open = 1;
                        printf("Picked up piece: %d\n", holdingPiece);
                        i = NUM_OF_PIECES;
                    }
                }
            }
        }
    }
    
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (holdingPiece >= 0) {
            pieces[holdingPiece].rotation += plus_rotation;
            printf("Rotated piece %d has rotation %d\n", holdingPiece, pieces[holdingPiece].rotation);
        }
        else {
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                if(x >= pieces[i].x_Location && x < pieces[i].x_Location + pieces[i].size) {
                    if (y >= pieces[i].y_Location && y < pieces[i].y_Location + pieces[i].size) {
                        pieces[i].rotation += plus_rotation;
                        if (pieces[i].open_edges.up_open == 0) pieces[i].open_edges.up_open = 1;
                        if (pieces[i].open_edges.down_open == 0) pieces[i].open_edges.down_open = 1;
                        if (pieces[i].open_edges.left_open == 0) pieces[i].open_edges.left_open = 1;
                        if (pieces[i].open_edges.right_open == 0) pieces[i].open_edges.right_open = 1;
                        printf("Rotated piece %d has rotation %d\n", i, pieces[i].rotation);
                        i = NUM_OF_PIECES;
                    }
                }
            }
        }
    }
    DrawPuzzlePieces();
}

void MousePosition(int x, int y) {
    y = glutGet(GLUT_WINDOW_HEIGHT)-y; // Fix Mouse Y
     
    if (holdingPiece >= 0) {
        pieces[holdingPiece].x_Location = x - (pieces[holdingPiece].size/2);
        pieces[holdingPiece].y_Location = y - (pieces[holdingPiece].size/2);
        DrawPuzzlePieces();
    }
}

void KeyboardListener(unsigned char theKey, int mouseX, int mouseY) {
    
    switch (theKey) {
        case 'p':
        case 'P':
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                printf("%d: x = %d, y = %d\n", i, pieces[i].x_Location, pieces[i].y_Location);
            }
            break;
        case 'r':
        case 'R':
            for (int i = 0; i < NUM_OF_PIECES; i++) {
                pieces[i].rotation = 0;
            }
            DrawPuzzlePieces();
            break;
        case 't':
        case 'T':
            if (do_bounding_box) do_bounding_box = false;
            else do_bounding_box = true;
            DrawPuzzlePieces();
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
    DrawPuzzlePieces();
}

int main(int argc, char * argv[]) {
    
    srand((unsigned)time(NULL));
    
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        Piece piece = { .pieceID = i,
                        .x_Location = rand()%SCREEN_WIDTH,
                        .y_Location = rand()%SCREEN_HEIGHT,
                        .size = 50,
                        .rotation = 0};
        pieces[i] = piece;
    }
    MakeConnections();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition((1920-SCREEN_WIDTH)/2, 0);
    glutCreateWindow("Giguesaur Alpha");
    
    glutDisplayFunc(Render);
    glutMouseFunc(MouseListener);
    glutPassiveMotionFunc(MousePosition);
    glutKeyboardFunc(KeyboardListener);
    glutReshapeFunc(WindowResize);
    
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(1.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    
    glutMainLoop();
    
    return 0;
}
