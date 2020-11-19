#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.

/* The following program solves any sudoku puzzle using 5 different functions.
The first function makes sure the same number is not present in the same row.
The second function makes sure the same number is not present in the same column.
The third function makes sure the same number is not present in the same 3*3 zone.
The fourth function validates the function. The fifth function checks to see if
all the cells are filled by numbers.
The final function, solve sudoku, is the most important function, as it actually solves
your sudoku. It uses recursion and backtracking to make sure all the conditions
in the functions prior to it are met.
*/
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {
  int j;
  assert(i>=0 && i<9); //to make sure i is in range
  for(j=0;j<9;j++){ // iterate through columns
    if(sudoku[i][j]==val){ //checks if val is present in the same row.
      return 1; // if val is in the same row, returns true
    }
  }
  return 0; //else, returns false.
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {
  int i;
  assert(j>=0 && j<9); //to make sure j is in range
  for(i=0;i<9;i++){ // iterate through rows
    if(sudoku[i][j]==val){ //checks if val is present in the same col.
      return 1; // if val is in the same col, returns true
    }
  }
  return 0; //else, returns false
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
  int a,b,x,y;
  assert(i>=0 && i<9);

  for(a=0;a<3;a++){
    for(b=0;b<3;b++){
      if(a==i/3 && b==j/3){ // determines which 3*3 zone we are looking at
        for(x=a*3;x<=a*3+2;x++){ // iterates through the rows in that 3*3 zone
          for(y=b*3;y<=b*3+2;y++){ // iterates through the cols in that 3*3 zone
            if(sudoku[x][y] == val){
              return 1; // if val is present in any of the cells in the 3*3 zone, return true
            }
          }
        }
      }
    }
  }

  return 0; // else return false
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  int col = is_val_in_col(val, j, sudoku);
  int row = is_val_in_row(val, i, sudoku);
  int threebythree = is_val_in_3x3_zone(val, i, j, sudoku);
  assert(i>=0 && i<9 && j>=0 && j<9);

  if(col != 1 && row != 1 && threebythree != 1){
    return 1; //returns true if the value can be filled in the given entry
  }
  else{
    return 0; //returns false if all three functions are satisfied
  }

}

int check_cells_for_numbers(int sudoku[9][9]){
  int i,j;
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      if (sudoku[i][j] == 0){
        return 0; // if 0 is found, returns false
      }
    }
  }
  return 1; // if all cells are filled, return true
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {
  int i, j;
  int flag =0; //initialise flag for later use
  if (check_cells_for_numbers(sudoku) == 1) {
    return 1; // if all cells are filled, return true
  }
  else {
    for(i=0;i<9;i++){
      for(j=0;j<9;j++){
        if (sudoku[i][j] == 0){
          flag++; // if non filled cell is found, make flag non-zero
          break; // break to make sure only 1 non-filled cell is found at a time
        }
      }
      if(flag != 0){
        break; // if non-filled cell is found, break out of the outer for loop as well
      }
    }
  }

  for (int num = 1; num <= 9; num++) { // possible numbers that can be filled in the given entry
    if (is_val_valid(num, i, j, sudoku)){
      sudoku[i][j] = num; // if the val can be filled in the given entry, fill it
      if (solve_sudoku(sudoku)) {
        return 1; // if sudoku is solved, return true
      }
      sudoku[i][j] = 0; //if the sudoku is not solved, it means that the num filled in is wrong, so it repeats the process again.
    }
  }
  return 0; //end the function.

}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
