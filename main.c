#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include "vec.h"
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

#define DIRT_CHAR '#'

int makeTheDirt(int pos, int rows, int columns) {
    char *output = vector_create();
    vector_add(&output, columns);
    for (int outputPos = 0; outputPos < columns; outputPos++) {
        output[outputPos] = ' ';
    }
    output[pos] = DIRT_CHAR;
    char *changes =  malloc((columns+1)* sizeof(char));
    strcpy(changes, output);
    for (int i = 0; i < rows; i++) {
        if (i < floor((double)rows/5)){
            printf("\n");
        } else {
            printf("%s\n", output);
            // set up next row
            for (int vecPos = 0; vecPos < columns; vecPos++) {
                if (output[vecPos] == DIRT_CHAR) {
                    switch (rand() % 3) {
                        case 1:
                            changes[vecPos - 1] = DIRT_CHAR;
                            break;
                        case 2: 
                            changes[vecPos + 1] = DIRT_CHAR;
                            break;
                        default:
                            changes[vecPos - 1] = DIRT_CHAR;
                            changes[vecPos + 1] = DIRT_CHAR;
                            break;
                    }
                }
            }
            // prepare to output next row
            strcpy(output, changes);
        }
    }
    return 0;
}


int main (int argc, char ** argv) {
    if (argc < 2) {
        printf("Insufficient arguments\n");
        return 0;
    }
    // get terminal size
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    // get position for first dirt piece
    char * tempStr = malloc(sizeof(argv[1]));
    strcpy(tempStr, argv[1]);
    int position = 0;
    // this loop adds the ASCII values for every character in the string to get n integer that can later
    // be used for the position
    for (int i = 0; strcmp(&tempStr[i], "\0") != 0; i++) {
       position += (int)tempStr[i];
    }
    // mod num by the number of columns in the terminal to ensure position will always be
    // within the terminal
    position %= w.ws_col;
    makeTheDirt(position, w.ws_row, w.ws_col);
}