#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include "vec.h"
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

int makeTheDirt(int pos, int rows, int columns) {
    char *output = vector_create();
    vector_add(&output, columns);
    for (int outputPos = 0; outputPos < columns; outputPos++) {
        output[outputPos] = ' ';
    }
    output[pos] = 'A';
    char *unmodified =  malloc((columns+1)* sizeof(char));
    strcpy(unmodified, output);
    for (int i = 0; i < rows; i++) {
        if (i < floor((double)rows/5)){
            printf("\n");
        } else {
            printf("%s\n", output);
            for (int vecPos = 0; vecPos < columns; vecPos++) {
                if (output[vecPos] == '#') {
                    switch (rand()% 3) {
                        case 1:
                            unmodified[vecPos - 1] = '#';
                            break;
                        case 2: 
                            unmodified[vecPos + 1] = '#';
                            break;
                        default:
                            unmodified[vecPos - 1] = '#';
                            unmodified[vecPos + 1] = '#';
                            break;
                    }
                }
                strcpy(output, unmodified);
            }
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