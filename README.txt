Game of Life Read Me

Created by Derek Ng
WPI username: djng
CS 2303
Created Jan 23, 2019

The Game of Life is a game created by John Conway in 1970. The game consists of
a board with cells on it. Cells can be either empty or occupied with organisms.
The board is filled with an initial configuration of organisms (generation 0). 
After the initial configuration, new generations are made under the following
rules:

 - An organism dies if it has 0 or 1 neighbors (because of loneliness) or if it
   has 4-8 neighbors (because of overcrowding).
   
 - An organism survives if it has 2 or 3 neighbors.
 
 - An organism is born in an empty cell if that cell has 3 neighbors.
 
New generations are generated until one of the following termination conditions
is reached:

 - There are no more organisms on the board.
 
 - The configuration of organisms for a specific generation is a repeat of an
   earlier configuration.
   
 - The predefined number of generations is reached.
 
Theoretically, The Game of Life is played on an infinite board. However, this
version is played on a finite board. Cells beyond the board are considered
always unoccupied. This version also only terminates if repeating generations
occur within two generations.

To play the game, command line arguments are required in the following order: 

 - Number of row: This is the number of rows for the board. It must be a
   positive integer that is equal or larger than the number of rows in the 
   input file.
   
 - Number of columns: This is the number of columns for the board. It must be a
   positive integer that is equal or larger than the number of columns in the
   input file.
   
 - Number of generations: This is the maximum number of generations to play the
   game for. If terminating conditions have not been reached by this number of
   generations, the game will terminate. This value must be a positive integer.
   
 - Input file: This is the file path to the initial configuration of the board.
 
 - Print: This indicates whether to print each generation. A 'y' denotes that
   each generation should be printed, while a 'n' denotes that generations
   should not be printed. If 'n' is denoted, only the final generation will be
   printed. Organisms are represented by an 'x'. This argument is optional.
   
 - Pause: This indicates whether the game should pause between generations. A
   'y' denotes that there should be a pause between generations, while a 'n'
   denotes that there should not be a pause. If 'y' is denoted, pressing the
   enter key will advance the game to the next generation. This argument is
   optional and cannot be specified without specifying the Print argument.
   
The game will begin after valid inputs are received. An error message will
display if invalid inputs are detected.