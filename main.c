#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>


int main (int argc, char ** argv) {
    // get terminal size
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    printf("%i\n", w.ws_col);
    // get position for first dirt piece
    char * tempStr = argv[1];
    int num = 0;
    // this loop adds the ASCII values for every character in the string to get n integer that can later
    // be used for the position
    for (int i = 0; strcmp(&tempStr[i], "\0") != 0; i++) {
        if (strcmp(&tempStr[i], " ") != 0){
            num += (int)tempStr[i];
        }
    }
    // mod num by the number of columns in the terminal to ensure position will always be
    // within the terminal
    int position = num % w.ws_col;
}