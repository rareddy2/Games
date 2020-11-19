#include "game.h"

/* This program makes the game 2048 for any number of columns and new_rows.
The make_game function sets up the game, the remake_game remakes the game once it is finished.
The get_cell function creates a helper function that checks for bounds and finds the elements in the
table. The move functions move the elements up/down/left/right. The legal move function
decides when the game ends (when no cell is empty, and when there is no legal move possible). */

game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure)
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    int i,j;
    mygame->rows = rows;
    mygame->cols = cols;

    for(i=0;i<rows;i++){
      for(j=0;j<cols;j++){
        mygame->cells[(i*cols)+j] = -1; // initialises all cells to -1 (empty)
      }
    }
    mygame->score = 0; // initialise score to 0

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any
	memory previously allocated is not lost in this function.
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

  int i,j;
  (*_cur_game_ptr)->rows = new_rows;
  (*_cur_game_ptr)->cols = new_cols;

  for(i=0;i<new_rows;i++){
    for(j=0;j<new_cols;j++){
      (*_cur_game_ptr)->cells[(i*new_cols)+j] = -1; // makes all the cells empty
    }
  }
  (*_cur_game_ptr)->score = 0; // same as make_game, but uses a double pointer instead, so that it can be changed in main.c


	return;
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    int *curr_cell;
    if(row>=0 && row<cur_game->rows && col>=0 && col<cur_game->cols){ // checks to make sure row and col are inside the boundary
      curr_cell = &cur_game->cells[(row*cur_game->cols)+col]; // if condition is true, finds the cell in a 1D array
      return curr_cell; //returns a pointer
    }
    else{
      return NULL; // if out of bounds, returns a NULL pointer
    }
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can
   not merge twice in one turn. If sliding the tiles up does not cause any
   cell to change value, w is an invalid move and return 0. Otherwise, return 1.
*/
{
  int i,j,k,a=-1; // i = cols, j = rows, k is used to find the first non empty cell, a is used to keep track of merged cells
  int flag = 0;

  for(i=0;i<cur_game->cols;i++){
    for(j=0;j<cur_game->rows;j++){
      if(*get_cell(cur_game,j,i)>0){ // finds the first non empty cell
        if((j+1)<cur_game->rows && *get_cell(cur_game,j+1,i) == *get_cell(cur_game,j,i)){ // checks if element right below is equal
          flag++; // flag != 0 if merge occurs
          a = j;

          *get_cell(cur_game,j,i) = 2**get_cell(cur_game,j+1,i); // merges the two cells
          cur_game->score += *get_cell(cur_game,j,i); // increments the score
          *get_cell(cur_game,j+1,i) = -1; // makes the cell below an empty cell
        }
        for(k=0;k<j;k++){ // iterates from 0 to the cell we are looking at
          if(*get_cell(cur_game,k,i)<0){ // finds the first non empty cell
            if(k-1 != a && k-1>=0 && a!=j && *get_cell(cur_game,k-1,i) == *get_cell(cur_game,j,i)){ // checks if element right above is equal
              flag++; // flag != 0 if a cell is changed
              a = k-1; // to make sure merge is not repeated

              *get_cell(cur_game,k-1,i) = 2**get_cell(cur_game,j,i); //merge
              cur_game->score += *get_cell(cur_game,k-1,i); // increment score
              *get_cell(cur_game,j,i) = -1; // make the cell empty
              break; // once a merge occurs, we do not loop back

            }
            else{
              flag++;
              *get_cell(cur_game,k,i) = *get_cell(cur_game,j,i); // fill in empty cell with the first non empty cell below it
              *get_cell(cur_game,j,i) = -1; //make that non empty cell empty
              break;
            }

          }
        }
      }
    }
  }

  if(flag==0)
{
return 0; // if no cell has changed
}
  return 1; // if any cell has changed
};

int move_s(game * cur_game) //slide down
{
  int i,j,k,a=-1;
  int flag =0; // to check whether or not any cell has changed

  for(i=cur_game->cols-1;i>=0;i--){
    for(j=cur_game->rows-1;j>=0;j--){
      if(*get_cell(cur_game,j,i)>0){
        if((j-1)>=0 && *get_cell(cur_game,j-1,i) == *get_cell(cur_game,j,i)){ // if cell right above is equal, then merge
          flag++;
          a = j;
          *get_cell(cur_game,j,i) = 2**get_cell(cur_game,j-1,i);
          cur_game->score += *get_cell(cur_game,j,i);
          *get_cell(cur_game,j-1,i) = -1;
        }
        for(k=cur_game->rows-1;k>=j;k--){ // iterate from bottom to current cell
          if(*get_cell(cur_game,k,i)<0){ // find first non empty cell
            if(k+1 != a && k+1<cur_game->rows && a!=j && *get_cell(cur_game,k+1,i) == *get_cell(cur_game,j,i)){ // if cell below non empty cell equals to the current non empty cell, then merge
              flag++;
              a = k+1;

              *get_cell(cur_game,k+1,i) = 2**get_cell(cur_game,j,i);
              cur_game->score += *get_cell(cur_game,k+1,i);
              *get_cell(cur_game,j,i) = -1;
              break;
            }
            else{
              flag++;
              *get_cell(cur_game,k,i) = *get_cell(cur_game,j,i);
              *get_cell(cur_game,j,i) = -1;
              break;
            }

          }
        }
      }
    }
  }

  if(flag==0)
{
return 0;
}
  return 1;
};

int move_a(game * cur_game) //slide left
{
  int i,j,k,a=-1;
  int flag = 0;

  for(i=0;i<cur_game->cols;i++){
    for(j=0;j<cur_game->rows;j++){
      if(*get_cell(cur_game,j,i)>0){ // find the first non empty cell
        if((i+1)<cur_game->cols && *get_cell(cur_game,j,i+1) == *get_cell(cur_game,j,i)){ // check if cell to the right is the same, if yes, then merge
          flag++;
          a = i;

          *get_cell(cur_game,j,i) = 2**get_cell(cur_game,j,i+1);
          cur_game->score += *get_cell(cur_game,j,i);
          *get_cell(cur_game,j,i+1) = -1;
        }
        for(k=0;k<i;k++){ // iterate from 0 to the first non empty element
          if(*get_cell(cur_game,j,k)<0){ // find the first empty cell
            if(k-1 != a && k-1>=0 && a!=i && *get_cell(cur_game,j,k-1) == *get_cell(cur_game,j,i)){ // merge cell to the left of the empty cell with current cell if they are equal
              flag++;
              a = k-1;

              *get_cell(cur_game,j,k-1) = 2**get_cell(cur_game,j,i);
              cur_game->score += *get_cell(cur_game,j,k-1);
              *get_cell(cur_game,j,i) = -1;
              break;
            }
            else{
              flag++;

              *get_cell(cur_game,j,k) = *get_cell(cur_game,j,i);
              *get_cell(cur_game,j,i) = -1;
              break;
            }

          }
        }
      }
    }
  }

  if(flag==0)
{
return 0;
}
  return 1;
};


int move_d(game * cur_game){

  int i,j,k,a=-1;
  int flag = 0;

  for(i=cur_game->cols-1;i>=0;i--){
    for(j=cur_game->rows-1;j>=0;j--){
      if(*get_cell(cur_game,j,i)>0){
        if((i-1)>=0 && (i-1)>=0 && *get_cell(cur_game,j,i-1) == *get_cell(cur_game,j,i)){
          flag++;
          a = i;

          *get_cell(cur_game,j,i) = 2**get_cell(cur_game,j,i-1);
          cur_game->score += *get_cell(cur_game,j,i);
          *get_cell(cur_game,j,i-1) = -1;
        }
        for(k=cur_game->cols-1;k>=i;k--){
          if(*get_cell(cur_game,j,k)<0){ // find the first empty cell in between first cell and the current cell
            if(k+1 != a && a != i && k+1<cur_game->cols && *get_cell(cur_game,j,k+1) == *get_cell(cur_game,j,i)){ // if cell to the right of the empty cell is equal to current cell, then merge

              flag++;
              a = k+1;

              *get_cell(cur_game,j,k+1) = 2**get_cell(cur_game,j,i);
              cur_game->score += *get_cell(cur_game,j,k+1);
              *get_cell(cur_game,j,i) = -1;
              break;
            }
            else{
              flag++;

              *get_cell(cur_game,j,k) = *get_cell(cur_game,j,i);
              *get_cell(cur_game,j,i) = -1;
              break;
            }

          }
        }
      }
    }
  }

  if(flag==0)
{
return 0;
}
  return 1;
};


int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.*/

{

  game * copygame = malloc(sizeof(game));
  copygame->cells = malloc(cur_game->rows*cur_game->cols*sizeof(cell)); // dynamically allocation memory to a copy of the game and its cells

  int i,j;
  copygame->rows = cur_game->rows; // copying the rows from cur_game
  copygame->cols = cur_game->cols; // copying cols from cur_game

  for(i=0;i<cur_game->rows;i++){
    for(j=0;j<cur_game->cols;j++){
      copygame->cells[(i*cur_game->cols)+j] = *get_cell(cur_game,i,j); // copying the values in each cell into copygame
    }
  }
  copygame->score = 0; // initialise score of the copy to 0

    int a,b;
    for(a=0;a<cur_game->rows;a++){
      for(b=0;b<cur_game->cols;b++){
        if(cur_game->cells[(a*cur_game->cols)+b]<0){
          return 1; // if any empty cell is found, then return 1
        }
        else if(move_w(copygame) == 1 || move_a(copygame) == 1 || move_s(copygame) == 1 || move_d(copygame) == 1){
          return 1; // if any legal move is possible, return 1
        }

        }
      }

free(copygame); //deallocate the memory of the copy game

    return 0; //return 0 if there are no empty cells and if there is no legal move allowed
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{

	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    if (cell_ptr == NULL){
        printf("Bad Cell Pointer.\n");
        exit(0);
    }


	//check for an empty cell
	int emptycheck = 0;
	int i;

	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}

    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game)
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;

	printf("\n\n\nscore:%d\n",cur_game->score);


	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char


    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  ");
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }

	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char

    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;

    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}

		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		}

		remake_game(&cur_game,rows,cols);

		move_success = 1;

    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }




    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game);
		 print_game(cur_game);
    }

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
