#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */

 /* This program solves the maze. The first function creates the maze; it allocates memory
 to all the data types in the maze structure and also allocates memory to all the cells in maze.
 The second function frees all the memory that was allocated to the maze to prevent data leaks.
 The third function prints the maze to the screen.
 The last function solves the maze using recursion. */

maze_t * createMaze(char * fileName)
{
    FILE *in_file;

    in_file = fopen(fileName,"r"); // opens the maze file

    maze_t * maze = malloc(sizeof(maze_t)); // allocates memory to the maze struct

    fscanf(in_file,"%d %d",&maze->height, &maze->width); // gets the height and width of the maze

    int i,j;

    maze->cells = malloc(maze->height*sizeof(char*)); // allocates memory to an array of pointers

    for(i=0; i<maze->height; i++){
        maze->cells[i] = malloc(maze->width*sizeof(char)); // allocates memory to values that the array of pointers points to
    }

    for(i=0; i<maze->height; i++){
      for(j=0; j<maze->width; j++){
        fscanf(in_file,"%c", &maze->cells[i][j]); // stores the characters inside the maze in the cells of the maze

        if(maze->cells[i][j] == START){ // if maze is pointing to start
          maze->startColumn = j; // setting start column
          maze->startRow = i; // setting start row
        }

        if(maze->cells[i][j] == END){ // if maze is pointing to end
          maze->endColumn = j;  // setting end column
          maze->endRow = i; // setting end row
        }

        if(maze->cells[i][j] == '\n'){ // if a new line character is detected
          j--; // backtrack by decrementing j
          continue; // skip this iteration
        }
      }
    }
    fclose(in_file); // close the maze file that was opened

    return maze; // return the maze pointer
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    int i;
    for(i=0;i<maze->height;i++){
      free(maze->cells[i]); // free the values that were pointed to by the array of pointers
    }

    free(maze->cells); // free the array of pointers

    free(maze); // free the maze
    return;
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
  int i,j;

  for(i=0;i<maze->height;i++){
    for(j=0;j<maze->width;j++){
      printf("%c",maze->cells[i][j]); // printing all the characters of the maze
    }
    printf("\n"); // printing new line
  }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */

int flag =0; // to make sure start is not accessed twice
int solveMazeDFS(maze_t * maze, int col, int row)
{
    if(maze->cells[row][col]==START){
      if(flag != 0){
        return 0; // if start has been accessed before,and if it is tried to be accessed again, return false
      }
    }
    if(col<0 || col>=maze->width || row<0 || row>=maze->height){
      return 0; // if out of bounds, return false
    }

    if(row == maze->endRow && col == maze->endColumn){
      return 1; // if end of maze is reached, return true
    }

    if(maze->cells[row][col] != START && maze->cells[row][col] != EMPTY){
      return 0; // if a non empty cell is found (wall), return false
    }

    if(maze->cells[row][col] != START){
      maze->cells[row][col] = PATH; // mark the cell as seen
    }
    else{
      flag++; // increment flag is start is seen the first time
   }

    if(solveMazeDFS(maze, col-1, row) == 1){
      return 1; // checking if moving left, solves maze. If yes, return true
    }

    if(solveMazeDFS(maze, col+1, row) == 1){
      return 1; // checking for right
    }

    if(solveMazeDFS(maze, col, row-1) == 1){
      return 1; // checking for up
    }

    if(solveMazeDFS(maze, col, row+1) == 1){
      return 1; // checking for down
    }

    if(maze->cells[row][col] != START){
      maze->cells[row][col] = VISITED; // backtrack if the maze has not been solved
    }

    return 0; // return false
}
