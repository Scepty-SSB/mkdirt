#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#define DIRT_CHAR '#'
#define SET_TEXT_COLOR "\x1b[38;5;52m"
#define MIN(x, y)                                                                                                      \
  ({                                                                                                                   \
    const typeof(x) _x = (x);                                                                                          \
    const typeof(y) _y = (y);                                                                                          \
    (void)(&_x == &_y);                                                                                                \
    _x < _y ? _x : _y;                                                                                                 \
  })
#define MAX(x, y)                                                                                                      \
  ({                                                                                                                   \
    const typeof(x) _x = (x);                                                                                          \
    const typeof(y) _y = (y);                                                                                          \
    (void)(&_x == &_y);                                                                                                \
    _x > _y ? _x : _y;                                                                                                 \
  })

/// The composition of a line of output.
typedef struct {
  unsigned leadingSpace;
  unsigned dirtWidth;
  unsigned trailingSpace;
} Line;

/// Returns a `Line` with the given values.
Line newLine(unsigned leadingSpace, unsigned dirtWidth, unsigned trailingSpace) {
  Line r = {leadingSpace, dirtWidth, trailingSpace};
  return r;
}

/// Prints `n` copies of `c`.
void printCopies(unsigned n, char c) {
  for (unsigned i = 0; i < n; i++) {
    (void)putc(c, stdout);
  }
}

/// Prints `l`.
void printLine(Line l) {
  printCopies(l.leadingSpace, ' ');
  printCopies(l.dirtWidth, DIRT_CHAR);
  printCopies(l.trailingSpace, ' ');
  (void)putc('\n', stdout);
  (void)fflush(stdout);
}

/// Returns the width of `l`.
unsigned lineWidth(Line l) {
  return l.leadingSpace + l.dirtWidth + l.trailingSpace;
}

/// Returns `l`, with its `leadingSpace` and `trailingSpace` reduced by `leadingShrinkage`
/// and `trailingShrinkage` respectively, preserving its `lineWidth`.
Line growDirt(Line l, unsigned leadingShrinkage, unsigned trailingShrinkage) {
  unsigned s0 = MIN(l.leadingSpace, leadingShrinkage);
  unsigned s1 = MIN(l.trailingSpace, trailingShrinkage);
  l.leadingSpace -= s0;
  l.trailingSpace -= s1;
  l.dirtWidth += s0 + s1;
  return l;
}

/// Prints a pile of dirt with the tip at `tipColumn` that is `columns` wide and `<= rows` tall.
///
/// - Precondition: `tipColumn < columns`.
void printDirtPile(unsigned tipColumn, unsigned rows, unsigned columns) {
  assert(tipColumn < columns);
  Line currentLine = newLine(tipColumn, 1, columns - (1 + tipColumn));
  for (unsigned i = 1; i <= rows; i++) {
    printLine(currentLine);

    // Ternary operators used to ensure that the pile always grows from line 1 to line 2. Makes the pile look more natural.
    currentLine = growDirt(currentLine, i == 1 ? 1 : (unsigned)rand() % i, i == 1 ? 1 : (unsigned)rand() % i);
    if (currentLine.dirtWidth == lineWidth(currentLine)) {
      return;
    }
  }
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
  // this loop adds the character encoding values for every character in the
  // string to get n integer that can later be used for the position
  for (unsigned i = 0; s[i] != 0; i++) {
    r += (unsigned)(unsigned char)s[i];
  }
  return r;
}

int main(int const argc, char const *const argv[]) {
  if (argc < 2) {
    (void)fprintf(stderr, "Insufficient arguments\n");
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
