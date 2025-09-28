# mkdirt: a joke program for when you make a typo
The idea for mkdirt was spawned when I accidentally added a 't' at the end of a 'mkdir'. I thought
"Wouldn't it be funny if typing 'mkdirt' actually printed a pile of dirt in your terminal?"
And now here we are.

mkdirt will generate a pile of dirt in your terminal, with the tip of the pile being at a position
determined by taking the sum of the character encoding numbers in the first argument (where the name of the new directory would be had you typed 'mkdir').
That number mod the number of columns in your terminal is then the starting position of the pile.

# Customization
Currently the only customization options are the character used to represent a piece of dirt, defined as DIRT_CHAR in main.c, and the output text color, defined as TEXT_COLOR in main.c.

# Build and install
(Note: Windows is not currently supported)
```bash
git clone https://github.com/Scepty-SSB/mkdirt
cd mkdirt
# if you want to customize your DIRT_CHAR or TEXT_COLOR do so before this next step
cc main.c vec.c -o mkdirt
sudo mv mkdirt /usr/bin/
# /usr/bin/ could be replaced with anywhere in your $PATH
```