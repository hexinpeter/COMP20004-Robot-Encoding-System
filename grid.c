/* 
 COMP20007 Design of Algorithms 

 Grid of data.

 Author: Andrew Turpin (aturpin@unimelb.edu.au)
 Date: April 2014

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "grid.h"


/*
** Read in grid form text file file which is in row-major order.
** First line is width and height space separated.
** INPUTS: file handle to read
**         checkSize - if true, check no entries are bigger than 2^MAX_BITS_GRID_HEIGHT
** OUTPUTS: grid
*/
grid_t *
readGrid(FILE *file, char checkSize) {
    assert(file);
    
    char buffer[MAX_LINE_LEN+1];  // +1 for \0

    uint32_t width;
    uint32_t height;

    if ((fgets(buffer, MAX_LINE_LEN, file) == NULL) 
    ||  (sscanf(buffer, "%d %d", &width, &height) != 2)) {
        fprintf(stderr, "Cannot get grid width and height from file\n");
        return NULL;
    } 

        // allocate memory for grid and grid->data
    grid_t *grid = (grid_t *)malloc(sizeof(grid_t));
    assert(grid != NULL);
    grid->data = (data_t *)malloc(sizeof(data_t)*width*height);
    assert(grid->data != NULL);
    grid->width = width;
    grid->height = height;
    
    data_t *p = grid->data;
    while (fgets(buffer, MAX_LINE_LEN, file) != NULL) {

        if (sscanf(buffer, "%d", p++) != 1)
            fprintf(stderr, "Trouble reading line %ld\n",p-grid->data+1+1);

        if (checkSize && (*(p-1) > (1 << MAX_BITS_GRID_HEIGHT)))
            fprintf(stderr, "WARNING: %d bigger than 2^MAX_BITS_GRID_HEIGHT\n",*(p-1));
    }

    return grid;
}//readGrid()


/*
** Return the Euclidian distance between two points on 
** grid.
** INPUTS: positions p1 and p2, and the grid
** OUTPUTS: distance
*/
float 
gridDistance(grid_t *g, data_t p1, data_t p2) {
    assert(g);

    data_t y1 = (data_t)floor(p1 / g->width);
    data_t y2 = (data_t)floor(p2 / g->width);
    data_t x1 = p1 - y1 * g->width;
    data_t x2 = p2 - y2 * g->width;

    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

// destructor
void
freeGrid(grid_t *g) {
    assert(g);
    free(g->data);
    free(g);
}
