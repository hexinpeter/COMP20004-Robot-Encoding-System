/* 
 COMP20007 Design of Algorithms 

 Grid of data simply stored in row-major order.

 Author: Andrew Turpin (aturpin@unimelb.edu.au)
 Date: April 2014

*/

#ifndef _GRID_H_
#define _GRID_H_

#include <stdint.h>
#include <stdio.h>

#define MAX_LINE_LEN 64    // max characters per line (+2) in a grid file

#define MAX_BITS_GRID_HEIGHT 10  // ceiling(log2(max height))

#define TRUE 1
#define FALSE (! TRUE)

    // convert an (x,y) coo-ord on grid of width _w to a position
#define POS(_x, _y, _w) ((_y)*(_w) + (_x))

typedef uint32_t data_t;    // each cell is a 32-bit number

typedef struct grid {
    uint32_t width;   // number of cells across
    uint32_t height;  // number of cells down
    data_t *data;     // cells in row-major order
} grid_t;
    
grid_t *readGrid(FILE *file, char checkSize);
float  gridDistance(grid_t *g, data_t p1, data_t p2);
void freeGrid(grid_t *g);

#endif
