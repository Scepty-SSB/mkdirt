#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "vec.h"
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DIRT_CHAR '#'
#define SET_TEXT_COLOR "\x1b[38;5;52m"

/// Given the column number of the tip of a dirt pile and the size of the terminal,
/// prints a pile of dirt.
void printDirtPile(unsigned pos, unsigned rows, unsigned columns) {
    char *output = vector_create();
    vector_add(&output, columns);
    for (vec_size_t outputPos = 0; outputPos < columns; outputPos++) {
        assert(outputPos < vector_size(output));
        output[outputPos] = ' ';
    }
    assert(pos < vector_size(output));
    output[pos] = DIRT_CHAR;
    char *changes =  malloc((columns+1)* sizeof(char));
    size_t changes_size = columns + 1;
    strcpy(changes, output);
    for (unsigned i = 0; i < rows; i++) {
        if (i < floor((double)rows/5)){
            printf("\n");
        } else {
            printf("%s\n", output);
            bool isBottomFull = true;
            unsigned checkIter = 0;
            while (isBottomFull && checkIter < columns) {
                assert(checkIter < vector_size(output));
                if (output[checkIter] != DIRT_CHAR) {
                    isBottomFull = false;
                }
                checkIter++;
            } 
            if (isBottomFull) {
                return;
            }
            // set up next row
            for (unsigned vecPos = 0; vecPos < columns; vecPos++) {
                assert(vecPos < vector_size(output));
                if (output[vecPos] == DIRT_CHAR) {
                    switch (rand() % 3) {
                        case 1:
                            assert(vecPos + 1 < changes_size);
                            changes[vecPos - 1] = DIRT_CHAR;
                            changes[vecPos + 1] = DIRT_CHAR;
                            if (rand() % rows > rows/i) {
                                changes[vecPos - 2] = DIRT_CHAR;
                            }
                            break;
                        case 2: 
                            if (vecPos < columns) {
                               assert(vecPos + 1 < changes_size);
                                changes[vecPos + 1] = DIRT_CHAR;
                                if (rand() % rows > rows/i && vecPos < columns - 1) {
                                  changes[vecPos - 2] = DIRT_CHAR;
                                }
                            }
                            assert(vecPos - 1 < changes_size);
                            changes[vecPos - 1] = DIRT_CHAR;
                            break;
                        default:
                            assert(vecPos - 1 < changes_size);
                            changes[vecPos - 1] = DIRT_CHAR;
                            unsigned consitentRand = (unsigned)rand();
                            if (consitentRand % rows > rows/i) {
                                changes[vecPos - 2] = DIRT_CHAR;
                            }
                            if (vecPos < columns) {
                                assert(vecPos + 1 < changes_size);
                                changes[vecPos + 1] = DIRT_CHAR;
                                if (consitentRand % rows > rows/i && vecPos < columns - 1) {
                                    assert(vecPos + 2 < changes_size);
                                    changes[vecPos + 2] = DIRT_CHAR;
                                }
                            }
                            break;
                    }
                } else if (rand() % (10*columns) == 1) {
                    assert(vecPos < changes_size);
                    changes[vecPos] = DIRT_CHAR;
                }
            }
            // prevents a bug where one extra DIRT_CHAR was being printed
            assert(changes_size > 0);
            vector_pop(changes);
            assert(columns < changes_size);
            changes[columns] = '\0';
            // prepare to output next row
            strcpy(output, changes);
        }
    }
    free(changes);
    return;
}

typedef struct winsize winsize;

winsize terminalSize() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w;
}

/// Returns a hash of `s`.
unsigned hash(char const *const s) {
    // get position for first dirt piece
    unsigned r = 0;
    // this loop adds the character encoding values for every character in the string to get n integer that can later
    // be used for the position
    for (unsigned i = 0; s[i] != 0; i++) {
       r += (unsigned)(unsigned char)s[i];
    }
    return r;
}

int main (int const argc, char const* const argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Insufficient arguments\n");
        return -1;
    }
    // set output color on ANSI terminals
    printf(SET_TEXT_COLOR);

    winsize w = terminalSize();
    
    // an arbitrary but deterministic value that fits in the line
    unsigned tipColumn = hash(argv[1]) % w.ws_col;    

    printDirtPile(tipColumn, w.ws_row, w.ws_col);
    
    // reset text color
    printf("\x1b[38;5;0m");
}
