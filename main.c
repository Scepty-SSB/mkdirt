#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>


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
}