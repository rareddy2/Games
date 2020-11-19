#include <stdio.h>

/* This program creates a game of life board. The first function counts the number of live_neighbours
around a given cell. The second function updates the board for the next step. The last function
checks whether or not the number of alive cells changes for the next step. If it doesn't change,
then the game ends.*/

/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
    int live_neighbours = 0;
    int i =0;
    int j=0;

    for(i = row-1; i <= row+1; i++){ // iterating through row-1 to row+1
        if(i>=0 && i < boardRowSize){ // checking boundary conditions of row
            for(j = col-1; j <= col+1; j++){ // iterating through col-1 to col+1
                if(j>=0 && j < boardColSize){ // checking boundary conditions of col
                    if(i != row || j != col){ // checking whether current cell is being checked
                        if(board[(i*boardColSize)+j] == 1){ // checking for alive cells
                            live_neighbours++; // increment live neighbors
                        }
                    }
                }
            }
        }

    }
    return live_neighbours; // return live_neighbours
}


/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
    int i, j, a;
    int new_board[boardRowSize*boardColSize]; // initialise new board array

    for(i = 0; i < boardRowSize ; i++){ // iterating through all rows
        for(j = 0; j < boardColSize ; j++){ // iterating through all cols
            if(board[(i*boardColSize)+j] == 1){
                if(countLiveNeighbor(board, boardRowSize, boardColSize, i, j) == 2 || countLiveNeighbor(board, boardRowSize, boardColSize, i, j) == 3){
                    new_board[(i*boardColSize)+j] = 1; // if number of alive cells are 2 or 3, cell remains alive
                }
                else {
                    new_board[(i*boardColSize)+j] = 0; // else, it dies
                }
            }
            else {
                if(countLiveNeighbor( board, boardRowSize, boardColSize, i, j) == 3){
                    new_board[(i*boardColSize)+j] = 1; // if number of alive cells are 3, cell becomes alive
            }
                else {
                    new_board[(i*boardColSize)+j] = 0; // else, cell remains dead
            }
        }
    }
}
for(a = 0; a < boardRowSize*boardColSize; a++){
    board[a] = new_board[a]; // transfer all the values from the new board to the old board.
}
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int* board, int boardRowSize, int boardColSize){
  int current_alive_cells = 0;
  int next_alive_cells = 0;
  int i,j;

  for(i = 0; i < boardRowSize ; i++){
        for(j = 0; j < boardColSize ; j++){
            if(board[(i*boardColSize)+j] == 1){
                current_alive_cells += 1; // if cell is alive, increment by 1
                if(countLiveNeighbor(board, boardRowSize, boardColSize, i, j) == 2 || countLiveNeighbor(board, boardRowSize, boardColSize, i, j) == 3){
                    next_alive_cells += 1; // if cell remains alive, increment by 1
                    }
                }
            else {
                if(countLiveNeighbor(board, boardRowSize, boardColSize, i, j) == 3){
                    next_alive_cells += 1; // if cell is originally dead and becomes alive, increment by 1
            }
            }
            }
        }
        if(current_alive_cells == next_alive_cells){
            return 1; // if current alive cells = next alive cells, program returns 1
        }
        else {
            return 0; // else return 0
        }
}
