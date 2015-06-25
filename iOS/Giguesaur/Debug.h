/*
    File: Debug.h
    Author: Ashley Manson
    
    Debug macros
 */

#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG 1

#define DEBUG_PRINT(format, ...) \
        do { if (DEBUG) fprintf(stderr, format, __VA_ARGS__); } while(0)

#endif
