#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

//#include <stdlib.h>
int moveCount; //total moves made
char moves[42]; //Array storing the moves made

/* Sets gameBoard to all .'s, takes in a gameBoard array of 
characters to reset the board to its original state */
void resetBoard(char gameBoard[7][8]){
  for(int i = 1; i < 7; i++){ 
    for(int j = 1; j < 8; j++){
      gameBoard[i][j] = '.';
    }
  }
}

/* Prints gameBoard in current state, takes in an array of columns 
that are used to print A-G above the gameboard, the gameBoard is 
also passed in so that it will be able to print specific states of 
the board */
void printBoard(char columns[], char gameBoard[7][8]) {
  printf("\n");
    for(int i=7; i>0; --i) {
      for(int j=0; j<8; ++j){
        if(i==7 && j==0){
          printf("  ");
          continue;
        }else if(j==0){
          printf("%2i", i);
          continue;
        }else if(i==7){
          printf("%3c", columns[j]);
          continue;
        }
        printf("%3c", gameBoard[i][j]);
      }
      printf("\n");
    }
    printf("\n");
}

/* Helper for playerMove method, takes in a column location and uses 
the columns array of all the possible columns to return the index 
(int) of the column inputted */
int findIndex(char column, char columns[]) {
  int index = 1;
  while(index < 8){
    if(column == columns[index]){
      return index;
    }
    index++;
  }
  return 0;
}

/* Adds a player piece to the specified column, takes in player
 (either X or O), columnChoice which is the column the player 
 chooses, the array columns to know the index of the columnChoice 
 using the helper method, and the gameBoard to edit the gameBoard 
 with the new player move, and returns the row of the piece that was 
 placed */
int playerMove(char player, char columnChoice, char columns[], char gameBoard[7][8]){

  int index = findIndex(toupper(columnChoice), columns);
  int row = 1;
  //gets the next available row for the given column
  while(gameBoard[row][index] != '.' && row<=7){ 
    row++;
  }
  //Checks if the row is full, if it is it returns 0(invalid row)
  if(row>=7){
    printf("\nError: Please choose a column that isn't full");
    return 0;
  }

  gameBoard[row][index] = player;
  moves[moveCount] = toupper(columnChoice);
  moveCount++;

  return row;
}

/* Checks the whole board (vertically, horizontally and diagonally) for a four in a row, takes in the 
location of the piece that is being checked for a win (row and columnChoice),columns[] which the array that 
stores the columns of the board,player which is the game piece of the player being checked for a win, and 
gameBoard which is just the 2d array that stores the game state. */
char checkWinner(int row, char columnChoice, char columns[], char player, char gameBoard[7][8]) {

  int column = findIndex(toupper(columnChoice), columns);   // column letter as a numerical index
  int consec = 0;  // used to count the amount of consecutive similar pieces in a potential game winning direction

  if(moveCount >= 42) {
    return 'd';
    printf("Draw");
  }

  // Checks vertical column
  for(int i=6; i>0; --i){
    if(gameBoard[i][column] == player){
      consec++;
      if(consec>=4){
        return player;
      }
    }else{
      consec = 0;
    }
  }
  consec = 0;

  // Checks horizontally
  for(int i=1; i<8; ++i){
    if(gameBoard[row][i] == player) {
      consec++;
      if(consec>=4){
        return player;
      }
    }else{
      consec = 0;
    }
  }
  consec = 0;

  // Checks diagonal top right to bottom left 
  for(int i = 1; i < 4; i++){
    if(gameBoard[row+i][column+i] == player){
      consec++;
    }else{
      break;
    }
  }
  for(int i = 1; i < 4; i++){
    if(gameBoard[row-i][column-i] == player){
      consec++;
    }else{
      break;
    }
  }
  if(consec>=3){
    return player;
  }
  consec = 0;

// Checks diagonal top left to bottom right
  for(int i = 1; i < 4; i++){
    if(gameBoard[row+i][column-i] == player){
      consec++;
    }else{
      break;
    }
  }
  for(int i = 1; i < 4; i++){
    if(gameBoard[row-i][column+i] == player){
      consec++;
    }else{
      break;
    }
  }
  if(consec>=3){
    return player;
  }

  //return null since winner was not found
  return '\0';
}

/* AI method, evaluates board and assesses where to make the next
 move, takes in the computer piece (X or O),takes in the player piece
 (opposite of the computer piece), the array of columns is used for
 the checkWinner method, and gameBoard is used to edit the gameBoard
 with the computer move, returns true if the computer wins,
 or false otherwise */
bool computerMove(char computer, char player, char columns[], char gameBoard[7][8]){

  // runs through each column consecutively to check if it can win
  for(int i=1; i<8; ++i){
    
    int row = 1;
    /* finds next open row for the column from the above
     for loop */
    while(gameBoard[row][i] != '.' && row <= 6) { 
      row++;
    }
    if(row>=7){
      continue;
    }

    gameBoard[row][i] = computer;
    bool computerWin = checkWinner(row, columns[i], columns, computer, gameBoard);

    if(computerWin){
      gameBoard[row][i] = computer;
      moves[moveCount] = toupper(columns[i]);
      moveCount++;
      return true;
    }

    gameBoard[row][i] = '.';
  }

  /* if it can't win anywhere on the board, it is going to check if
   the player can win anywhere and if so, it blocks that location
    with a computer piece */
  for(int i=1; i<8; ++i){

    int row = 1;
    while(gameBoard[row][i] != '.' && row <= 6) { 
      row++;
    }
    if(row>=7){
      continue;
    }

    gameBoard[row][i] = player;
    bool playerWin = checkWinner(row, columns[i], columns, player, gameBoard);

    if(playerWin){
      gameBoard[row][i] = computer;
      moves[moveCount] = toupper(columns[i]);
      moveCount++;
      return false;
    }
    gameBoard[row][i] = '.';
  }
  

  /* if there is neither a winning nor a blocking move, it checks if
   there are consecutive computer pieces to form a line */
  for(int i=1; i<8; ++i){
    int row = 1;
    while(gameBoard[row][i] != '.' && row <= 6) {
      row++;
    }
    if(row>=7){
      continue;
    }
    if(gameBoard[row-1][i]==computer || gameBoard[row][i-1]== computer || gameBoard[row-1][i-1]==computer || gameBoard[row+1][i+1]==computer || gameBoard[row+1][i-1]==computer || gameBoard[row-1][i+1]==computer){
          gameBoard[row][i] = computer;
          moves[moveCount] = toupper(columns[i]);
          moveCount++;
          return false;
    }
  }
  
  /* when there is no other good move or when it starts out can't
   use rand(), so created our own "random" implementation */
  int randColumns[] = {1, 2, 7, 4, 3, 1, 6};
  int randChoice;
  int row;
  for(int i=0; i<7; ++i){
    randChoice = randColumns[i];
    row = 1; 
    while(gameBoard[row][randChoice] != '.' && row <= 6) {
      row++;
    }
    if(row>=7){
      continue;
    }
  }

  gameBoard[row][randChoice] = computer;
  moves[moveCount] = toupper(columns[randChoice]);
  moveCount++;
  return false;
}


// Helper method to check if user entered a valid column choice, else return false
bool validColumn(char columnChoice, char columns[]){
  for(int i = 1; i < 8; i++){
    if(toupper(columnChoice) == columns[i]){
      return true;
    }
  }
  return false;
}

int main(void) {
  char gameBoard[7][8]; //Connect four game board
  char winner; //Winner of the game
  char columnChoice; //the column the player chooses
  int row; //row of the most recent location of the piece placed
  char columns[] = {' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G'}; //top part of the game board and used for indexing columns
  char player; // Player
  char computer; // AI/Computer
  int orderChoice; //Order player chooses in beginning
  bool compWin = false; //Whether the computer has won or not

  resetBoard(gameBoard);
// prompts user if they want to go first or second, stores the choice in orderChoice
  do{
    printf("Choose 1 to go first, 2 to go second.\n");
    scanf("%i", &orderChoice);

  } while(orderChoice > 2 || orderChoice < 1);

  //if order choice is 1, player becomes X and computer becomes O, if order choice is 2, it becomes vice versa
  player = (orderChoice == 1) ? 'X' : 'O'; 
  computer = (orderChoice == 1) ? 'O' : 'X';
  moveCount = 0; //counts the total amount of moves made

  while(true){
    if(orderChoice == 1){ //if Player chooses to go first

      do{
        do {
          // prompts user for what column to choose
          printf("\nChoose a column to drop your piece (A - G): ");
          scanf(" %c", &columnChoice);
        }//Ensures the player chooses a valid column 
        while(!validColumn(columnChoice, columns));
        row = playerMove(player, toupper(columnChoice), columns, gameBoard);
      } //Ensures the player does not pick a full column
      while(row == 0);
      // clears the terminal
      printf("\e[2J\e[0;0H");
      printBoard(columns, gameBoard);

      winner = checkWinner(row, columnChoice, columns, player, gameBoard);
      
      if(winner == player){
        printf("You win!\n");
        break;
      }else if(winner == 'd'){
        printf("You tied the game.\n");
        break;
      }
      
      compWin = computerMove(computer, player, columns, gameBoard);
      printf("\e[2J\e[0;0H");
      printBoard(columns, gameBoard);

      if(compWin){
        printf("The computer won!\n");
        break;
      }else if(moveCount >= 42){
        printf("The computer tied the game.\n");
        break;
      }

    } else { // Player chooses to go second
      
      compWin = computerMove(computer, player, columns, gameBoard);
      printf("\e[2J\e[0;0H");
      printBoard(columns, gameBoard);

      if(compWin){
        printf("The computer won!\n");
        break;
      }else if(moveCount >= 42){
        printf("The computer tied the game.\n");
        break;
      }

      do{
        do {
          printf("\nChoose a column to drop your piece (A - G): ");
          scanf(" %c", &columnChoice);
        }
        while(!validColumn(columnChoice, columns));
        row = playerMove(player, toupper(columnChoice), columns, gameBoard);
      }
      while(row == 0);
  
      printf("\e[2J\e[0;0H");
      printBoard(columns, gameBoard);

      winner = checkWinner(row, columnChoice, columns, player, gameBoard);
      
      if(winner == player){
        printf("You win!\n");
        break;
      } else if(winner == 'd'){
        printf("You tied the game.\n");
        break;
      }
    }
  }
  //Prints out all of the game moves
  printf("Game moves: [");
    for(int i=0; i<moveCount; ++i){
      if(moves[i+1]!='\0'){
        printf("%c, ", moves[i]);
      }else{
        printf("%c", moves[i]);
      }
    }
  printf("]");

}