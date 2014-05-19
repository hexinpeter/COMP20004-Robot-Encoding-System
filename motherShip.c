/* 
 COMP20007 Design of Algorithms 

 Read in an estimate (upper bound) of terrain heights (2D in row-major
 form), a list of locations to visit in order, and the bitfile.
 Output the actual heights to stdout.

 Author: Andrew Turpin (aturpin@unimelb.edu.au)
 Date: April 2014

 Usage: see usage()

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "grid.h"
#include "bitfile.h"

/*
** Print usage message
*/
void
usage(char *exeName) { 
    fprintf(stderr,"Usage: %s sonarFileName plotterFileName bitFileName\n",exeName);
}

/*
** Read command line params, call search
*/
int 
main(int argc, char *argv[]) {

    if (argc != 4) {
        usage(argv[0]);
        return(-1);
    }

    grid_t *sonar   = readGrid(strcmp(argv[1],"-")?fopen(argv[1],"r"):stdin, TRUE);
    grid_t *plotter = readGrid(strcmp(argv[2],"-")?fopen(argv[2],"r"):stdin, FALSE);

    bit_file_t *bitfile = BitFileOpen(argv[3], BF_READ);

    assert(sonar);
    assert(plotter);
    assert(bitfile);

        // temporary home for terrain heights
    data_t *result = (data_t *)malloc(sizeof(data_t) * plotter->width * plotter->height); 
    assert(result);

    for(uint32_t pos = 0 ; pos < plotter->width * plotter->height ; pos++) {
        data_t temp = 0, len = 0, tempLen = 0;
        
        //get the length of the data part of the PBF from the length defining part, store it into "len"
        do {
            tempLen = 0;
            BitFileGetBitsInt(bitfile, &tempLen, 3, sizeof(len));
            len += tempLen;
        } while (tempLen == 7);
        
        //get the stored data from the data part
        for (int i = 0; i < len; i++) {
            int lastbit = BitFileGetBit(bitfile);
            temp = temp*2 + lastbit;
        }
        
        //actual height = sonar reading - inaccuracy
        temp = sonar->data[plotter->data[pos]] - temp;
        
        result[plotter->data[pos]] = temp;
    }

    assert(!BitFileClose(bitfile));

    printf("%d %d\n", plotter->width, plotter->height);
    for(uint32_t pos = 0 ; pos < plotter->width * plotter->height ; pos++)
        printf("%d\n",result[pos]);

    free(result);

    return 0;
}
