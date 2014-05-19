/* 
 COMP20007 Design of Algorithms 

 Read in an estimate (upper bound) of terrain heights (2D in row-major
 form), formulate a traversal strategy, and then write out the 
 position numbers in order of the traversal.

 Author: Andrew Turpin (aturpin@unimelb.edu.au) && Xin He
 Date: May 2014

 Usage: see usage()
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "grid.h"

void plotter(int width, int height);

/*
** Print usage message
*/
void
usage(char *exeName) { 
    fprintf(stderr,"Usage: %s sonarFileName\n",exeName);
}

/*
** Read command line params, call search
*/
int 
main(int argc, char *argv[]) {

    if (argc != 2) {
        usage(argv[0]);
        return(-1);
    }

    grid_t *sonar = readGrid(strcmp(argv[1],"-")?fopen(argv[1],"r"):stdin, TRUE);
    assert(sonar);

    printf("%d %d\n", sonar->width, sonar->height);

    plotter(sonar->width, sonar->height);

    return 0;
}


/*
** Print the squares to be visited in row-major order
** The robot moves in a zig-zag motion, visiting row by row
*/
void plotter(int width, int height) {
    int x, y; //the x-coordinate, y-coordinate
    
    for (y=0; y<height; y++) {
        for (x=0; x<width; x++) {
            printf("%d\n", (y%2)?(width-1-x+y*width):(x+y*width));
        }
    }
}





