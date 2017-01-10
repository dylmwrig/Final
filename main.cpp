//main of this program

#include "checker.h" //my headers
#include "node.h"

#include <iostream> //library headers
#include <sstream> //stringstream
#include <cctype> //isdigit
#include <limits> //used for flushing cin

//globals sorry :   ^   (
//allows for easy manipulation of the amount of pieces
int redPieces = 12;
int blackPieces = 12;

void printBoard(Checker board[][8]) //pass in the checkerboard w/ initialized checkers
{
  char lineSep; //lineSep separates the identity character for each tile to make the board nicer to look at (in my opinion)
 
  std :: cout << "     Pieces left\n";
  if (redPieces > 9) //just for consistent printing
  {
    std :: cout << "Red: " << redPieces << "\t       Black: " << blackPieces << std :: endl;
  } //end if

  else 
  {
    std :: cout << "Red: " << redPieces << " \t       Black: " << blackPieces << std :: endl;
  } //end else

  for (int a = 0; a < 8; a++) //for rows
  {
    for (int b = 0; b < 8; b++) //for columns
    {
        if (isdigit(board[a][b].getIdentity())) 
        {
        //if the piece's identity has been given a value, aka when displaying moves to the player, or if the                                       
        //piece is selected (marked by an x), make sure the number isn't overwritten with a letter in the beginning parts of this function
          lineSep = ' ';
          if (((board[a][b].getRank() == 2) || (board[a][b].getRank() == 4)) && !isdigit(board[a][b].getLineSep2()))
          {
            board[a][b].setLineSep2('!');
          } //end if
        } //end if

        else if ((board[a][b].getIdentity() == 'x') || (board[a][b].getIdentity() == 'K')) //don't change it if it's x or K because this is how I'm representing the chosen piece
        {

        } //end else if

        else //not a number so it's not a choice
        {
           if (board[a][b].getRank() == 0) //if there's no piece here, represent it as either a "black" or a "white" tile (represented by + and - respectively
           {
              if ((((a%2) == 0) && ((b%2) != 0)) || (((a%2) != 0) && (((b%2) == 0)))) //if a is cleanly divisible by 2 (even) and b is odd...
              {                                                                       //this is a "white" tile
                board[a][b].setIdentity('-'); 
                lineSep = ' '; //if this tile is empty, it is separated by spaces so as not to overload the eyes with '|' characters
              } //end if

              else //otherwise, it's "black"
              {
                board[a][b].setIdentity('+');
                lineSep = ' ';
              } //end if 
            } //end if

            else if (board[a][b].getRank() == 1)
            {
              board[a][b].setIdentity('b'); //b for black
              lineSep = '|'; //if this tile has a character, separate with '|' characters so as to draw the eye a little more
            } //end elif
  
            else if (board[a][b].getRank() == 2)
            {
              board[a][b].setIdentity('B');
              lineSep = '|';
            } //end elif    

            else if (board[a][b].getRank() == 3)
            {
              board[a][b].setIdentity('r'); //r for red
              lineSep = '|';
            } //end elif

            else if (board[a][b].getRank() == 4)
            {
              board[a][b].setIdentity('R');
              lineSep = '|';
            } //end elif
        } //end else
      //if the linesep2 has been set to a number from the moveCheck function, do nothing
      if (isdigit(board[a][b].getLineSep2()) || (board[a][b].getLineSep2() == '!'))
      {
        
      }

      //otherwise, set linesep2 = linesep
      else
      {
        board[a][b].setLineSep2(lineSep);
      }

      std :: cout << lineSep << board[a][b].getIdentity() <<  board[a][b].getLineSep2();
      if (board[a][b].getLineSep2() == '!')
      {
        board[a][b].setLineSep2(' '); //set line sep 2 to ' ' after printing so the '!' doesn't persist. I only need the value for printing anyway
      } //end if
    } //end for
    std :: cout << std :: endl; //illustrate rows
  } //end for
} //end printBoard

void heuristic(Checker board[][8], int pY, int pX, int bestLoc[5])
{
  bool farLeft, farRight, farTop, farBottom;
  bool jumpCheck = true;
  bool king = false;
  bool leftReg = false, rightReg = false;
  bool leftKing = false, rightKing = false;
  bool middle = false; //really just shorthand for later
  bool evade = false, block = false; //booleans to check for defensive moves
  bool isJumping = false; 

  int jumpCount = 0; //used for counting jump possibilities: multijumps are more valuable than singles after all
  int y, x; //holds the x and y location of the empty tile we're generating a heuristic for
  int jumpX, jumpY, movedX = -1, movedY = -1; //used in the jump heuristic section, movedX and movedY are to indicate what was just jumped over so that tile isn't counted again
  int weakPoints = 0;
//I will start by checking only for moves for black plaeyr as that is the default bot color
//if you want to add red functionality as well, just do the stuff you did in moveCheck and move to make those compatible both ways
//  std :: cout << "BESTLOC 0 TO 5 in the start " << bestLoc[0] << " " << bestLoc[1] << " " << bestLoc[2] << " " << bestLoc[3] << " " << bestLoc[4] << std :: endl;
    for (int i = 0; i < 4; i++)
    {
      y = -1; //reset each x and y value to -1 
      x = -1; 
      if (i == 0 && pY < 7 && pX < 7 && board[pY + 1][pX + 1].getRank() == 0)
        {
          y = pY + 1;
          x = pX + 1;
          board[y][x].setHeuristic(1);
        } //end if

        //if there is a piece to the bottom right and an empty one behind it, that tile behind it is able to be jumped on
        //check if i is equal to 1 so we don't check this tile twice
        else if (i == 0 && pY < 6 && pX < 6 && board[pY + 1][pX + 1].getRank() > 2 && board[pY + 2][pX + 2].getRank() == 0)
        {
          y = pY + 2;
          x = pX + 2;
          board[y][x].setHeuristic(1);
        }

        if (i == 1 && pY < 7 && pX > 0 && board[pY + 1][pX - 1].getRank() == 0)
        {
          y = pY + 1;
          x = pX - 1;
          board[y][x].setHeuristic(1);
        } //end if

        else if (i == 1 && pY < 6 && pX > 1 && board[pY + 1][pX - 1].getRank() > 2 && board[pY + 2][pX - 2].getRank() == 0)
        {
          y = pY + 2;
          x = pX - 2;
          board[y][x].setHeuristic(1);
        }

        if (i == 2 && pY > 0 && pX < 7 && board[pY][pX].getRank() == 2 && board[pY - 1][pX + 1].getRank() == 0)
        {
          y = pY - 1;
          x = pX + 1;
          board[y][x].setHeuristic(1);
        } //end if

        else if (i == 2 && pY > 1 && pX > 6 && board[pY - 1][pX + 1].getRank() > 2 && board[pY - 2][pX + 2].getRank() == 0)
        {
          y = pY - 2;
          x = pX + 2;
          board[y][x].setHeuristic(1);
        }

        if (i == 3 && pY > 0 && pX > 0 && board[pY][pX].getRank() == 2 && board[pY - 1][pX - 1].getRank() == 0)
        {
          y = pY - 1;
          x = pX - 1;
          board[y][x].setHeuristic(1);
        } //end if

        else if (i == 3 && pY > 1 && pX > 1 && board[pY - 1][pX - 1].getRank() > 2 && board[pY - 2][pX - 2].getRank() == 0)
        {
          y = pY - 2;
          x = pX - 2;
          board[y][x].setHeuristic(1);
        }
       
        //these booleans are just shorthand so I don't have to type it out every time
        if (x == 0) //are we in the far left column?
        {
          farLeft = true;
        } //end if
      
        else if (x == 7) //are we in the far right column?
        {
          farRight = true;
        } //end else if
      
        if (y == 0)
        {
          farTop = true;
        } //end if
        
        else if (y == 7)
        {
          farBottom = true;
        } //end if

        if (x != -1)
        {
          if (!farTop && !farLeft && (board[y - 1][x - 1].getRank() == 1 || board[y - 1][x - 1].getRank() == 2))
          {
            leftReg = true;
          } //end if

          if (!farTop && !farRight && (board[y - 1][x + 1].getRank() == 1 || board[y - 1][x + 1].getRank() == 2))
          {
            rightReg = true;
          } //end if

          if (!farBottom && !farLeft && (board[y + 1][x - 1].getRank() == 2))
          {
            leftKing = true;
          } //end if

          if (!farBottom && !farRight && (board[y + 1][x + 1].getRank() == 2))
          {
            rightKing = true;
          } //end if
        } //end if

       //if we've just established that this is a tile you can move to
        if (board[y][x].getHeuristic() == 1)
        {
          jumpY = y; //set these equal to the tile we're checking for as this could be possibly jumped to
          jumpX = x; //and we need to check for subsequent jumps if that's the case

          if (x > 1 && y > 1 && (board[y - 1][x - 1].getRank() > 2))
          { //is the piece that would hypothetically be able to jump where our piece is located?
            if ((y - 2 == pY) && (x - 2 == pX))
            {
              jumpCount++;
              if (board[pY][pX].getRank() == 2)
              {
                king = true;
              } //end if
            } //end if
          } //end if

          //using else ifs because we only want to know if this tile can be jumped on from one piece's perspective, not multiple
          else if (y > 1 && x < 6 && (board[y - 1][x + 1].getRank() > 2))
          {
            if ((y - 2 == pY) && (x + 2 == pX))
            {
              jumpCount++;
              if (board[pY][pX].getRank() == 2)
              {
                king = true;
              } //end if
            } //end if
          } //end else if

          else if (x > 1 && y < 6 && (board[y + 1][x - 1].getRank() > 2))
          {
            if ((y + 2 == pY) && (x - 2 == pX))
            {
              jumpCount++;
              king = true;
            } //end if
          } //end if

          else if (x < 6 && y < 6 && (board[y + 1][x + 1].getRank() > 2))
          {
            if ((y + 2 == pY) && (x + 2 == pX))
            {
              jumpCount++;
              king = true;
            } //end if
          } //end else if

          if(jumpCount > 0)
          { //basically, can this piece jump again?
            while (jumpCheck)
            { //this loop is breaking my program so else is not being reached 
              if (jumpY < 6 && jumpX < 6 && ((jumpY + 1 != movedY) || (jumpX + 1 != movedX)) &&  board[jumpY + 1][jumpX + 1].getRank() > 2 && board[jumpY + 2][jumpX + 2].getRank() == 0)
              { //check that the piece we're trying to jump over wasn't jumped over already, as this causes an infinite logic loop
                jumpCount++;
                movedY = jumpY + 1;
                movedX = jumpX + 1;
                jumpY += 2;
                jumpX += 2;
              } //end if

              else if (jumpY < 6 && jumpX > 1 && ((jumpY + 1 != movedY) || (jumpX - 1 != movedX)) && board[jumpY + 1][jumpX - 1].getRank() > 2 && board[jumpY + 2][jumpX - 2].getRank() == 0)
              {
                jumpCount++;
                movedY = jumpY + 1;
                movedX = jumpX - 1; //could also solve this by setting the identity to something specific and checking for that but will have to wipe it afterwards
                jumpY += 2;
                jumpX -= 2;
              } //end if

//these next 2 else ifs are being executed every single time
//I belive this is because this constantly re-marks the piece we jumped over as jumpable again, as the conditions for it are never changed
              else if (jumpY > 1 && king && jumpX < 6 &&((jumpY - 1 != movedY) || (jumpX + 1 != movedX)) &&  board[jumpY - 1][jumpX + 1].getRank() > 2 && board[jumpY - 2][jumpX + 2].getRank() == 0)
              {
                jumpCount++;
                movedY = jumpY - 1;
                movedX = jumpX + 1;
                jumpY -= 2;
                jumpX += 2;
              } //end if

              else if (jumpY > 1 && king && jumpX > 1 && ((jumpY - 1 != movedY) || (jumpX - 1 != movedX)) && board[jumpY - 1][jumpX - 1].getRank() > 2 && board[jumpY - 2][jumpX - 2].getRank() == 0)
              {
                jumpCount++;
                movedY = jumpY - 1;
                movedX = jumpX - 1;
                jumpY -= 2;
                jumpX -= 2;
              } //end if

              else
              {
                jumpCheck = false;
              } //end else
            } //end while
          } //end if

          if (pY > 0 && pX > 0 && pY < 7 && pX < 7) 
          { //boolean so I don't have to write this ungodly conditional over and over again
            middle = true;
          } //end if

          //check if the piece is in danger
          if (middle && board[pY - 1][pX - 1].getRank() == 0 && board[pY + 1][pX + 1].getRank() > 2)
          {
            evade = true;
          } //end if

          else if (middle && board[pY - 1][pX + 1].getRank() == 0 && board[pY + 1][pX - 1].getRank() > 2)
          {
            evade = true;
          } //end if
 
          else if (middle && board[pY + 1][pX - 1].getRank() == 0 && board[pY - 1][pX + 1].getRank() == 4)
          {
            evade = true;
          } //end if

          else if (middle && board[pY + 1][pX + 1].getRank() == 0 && board[pY - 1][pX - 1].getRank() == 4)
          {
            evade = true;
          } //end if

          if (evade && ((y + 1 == pY) && (x - 1 == pX))) //is the piece in danger/is this tile in range of the piece?
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 7);
          } //end if
          
          else if (evade && ((y + 1 == pY) && (x + 1 == pX))) //is the piece in danger/is this tile in range of the piece?
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 7);            
          }

          else if (evade && ((y - 1 == pY) && (x - 1 == pX))) //is the piece in danger/is this tile in range of the piece?
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 7);
          } //end if
 
          else if (evade && ((y - 1 == pY) && (x + 1 == pX))) //is the piece in danger/is this tile in range of the piece?
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 7);
          } //end if

          //if this can block an enemy jump
          //setting it to add 9. I believe that blocking jumps is more valuable than evading jumps as it creates a defense that lasts rather than just moving
          //away from an enemy piece
          //I'm making this its own if statement because a piece could be evading a jump while also blocking another
          
          //if there is a friendly piece adjacent to this tile and there is an enemy piece ready to jump it, add to this position's value because it is a block tile
          //we must also make sure that the piece we're checking against isn't the piece passed in itself, as that's covered under the evade heuristic
          if (y < 6 && x < 6 && (y + 1 != pY) && (x + 1 != pX) && (board[y + 1][x + 1].getRank() == 1 || board[y + 1][x + 1].getRank() == 2) && board[y + 2][x + 2].getRank() > 2)
          {
            block = true;
          } //end if

          else if (y < 6 && x > 1 && (y + 1 != pY) && (x - 1 != pX) && (board[y + 1][x - 1].getRank() == 1 || board[y + 1][x - 1].getRank() == 2) && board[y + 2][x - 2].getRank() > 2)
          {
            block = true;
          } //end if

          else if (y > 1 && x < 6 && (y - 1 != pY) && (x + 1 != pX) && (board[y - 1][x + 1].getRank() == 1 || board[y - 1][x + 1].getRank() == 2) && board[y - 2][x + 2].getRank() > 2)
          {
            block = true;
          } //end if

          else if (y > 1 && x > 1 && (y - 1 != pY) && (x + 1 != pX) && (board[y - 1][x - 1].getRank() == 1 || board[y - 1][x - 1].getRank() == 2) && board[y - 2][x - 2].getRank() > 2)
          {
            block = true;
          } //end if

          if (block && ((y + 1 == pY) && (x - 1 == pX)))
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 8);
          } //end if

          else if (block && ((y + 1 == pY) && (x + 1 == pX)))
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 8);
          } //end if
          
          else if (block && ((y - 1 == pY) && (x - 1 == pX)))
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 8);
          } //end if

          else if (block && ((y - 1 == pY) && (x + 1 == pX)))
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 8);
          } //end if 

          //will this be threatening another piece, and will it be safe itself?
          //so I am checking if there is a piece to one side (which will presumably be moved), if there is a piece to that same side but forwards on the board to be jumped
          //(which also entails checking if the tile behind it is empty), and if there is any piece to the other side
          //the reason I am only checking for an owned piece on one side is because, if we are approaching a piece from the same diagonal they are on, they'll just jump it immediately.
          if (y < 6 && x < 6 && rightReg && board[y + 1][x + 1].getRank() > 2 && board[y + 2][x + 2].getRank() == 0)
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 6);
          } //end if

          else if (y < 6 && x > 1 && leftReg && board[y + 1][x - 1].getRank() > 2 && board[y + 2][x - 2].getRank() == 0)
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 6);
          } //end if
 
          //realizing I probably don't need rightKing and leftKing to be separate but I'll roll with it for now, will change later if deemed unnecessary
          else if (y > 1 && x < 6 && (rightKing || leftKing) && board[y - 1][x + 1].getRank() > 2 && board[y - 2][x + 2].getRank() == 0)
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 6);
          } //end if

          else if (y > 1 && x > 1 && (rightKing || leftKing) && board[y - 1][x - 1].getRank() > 2 && board[y - 2][x - 2].getRank() == 0)
          {
            board[y][x].setHeuristic(board[y][x].getHeuristic() + 6);
          } //end if  

          //check if the tile we're moving to would put us in immediate danger: if so, make the heuristic negative so we never move here.
          //check if that tile will be empty or if it was previously containing the piece we're moving
          if (!farBottom && !farRight && !farLeft && !farTop && board[y + 1][x + 1].getRank() > 2 && (board[y - 1][x - 1].getRank() == 0 || ((y - 1 == pY) && (x - 1 == pX))))
          {
            board[y][x].setHeuristic(-10000000);
          } //end if

          else if (!farBottom && !farRight && !farLeft && !farTop && board[y + 1][x - 1].getRank() > 2 && (board[y - 1][x + 1].getRank() == 0 || ((y + 1 == pY) && (x - 1 == pX))))
          {
            board[y][x].setHeuristic(-10000000);
          } //end if

          else if (!farBottom && !farRight && !farLeft && !farTop && board[y - 1][x + 1].getRank() == 4 && (board[y + 1][x - 1].getRank() == 0 || ((y - 1 == pY) && (x + 1 == pX))))
          {
            board[y][x].setHeuristic(-10000000);
          } //end if

          else if (!farBottom && !farRight && !farLeft && !farTop && board[y - 1][x - 1].getRank() == 4 && (board[y + 1][x + 1].getRank() == 0 || ((y - 1 == pY) && (x - 1 == pX))))
          {
            board[y][x].setHeuristic(-10000000);
          } //end if  
        } //end else if
    
      if (jumpCount > 0)
      {
        board[y][x].setHeuristic(board[y][x].getHeuristic() + (jumpCount * 20));
      } //end if

      if (y == 7 && (board[pY][pX].getRank() == 1))
      {
        board[y][x].setHeuristic(board[y][x].getHeuristic() + 7); 
      } //end if

      else if (x == 0 || x == 7 || y == 0 || y == 7)
      { //if this piece is moving to the edge, it's worth more (because it has less options be jumped
        board[y][x].setHeuristic(board[y][x].getHeuristic() + 6);
      } //end if

      //if this is not on any edge of the board, check how many "weak points" (empty adjacent tiles it has
      //check if a jump is going to happen: if it is, don't count that piece we're jumping in the weakPoints check heuristic
      else
      {
        if (x < 6 && y > 1 && (y - 2 == pY) && (x + 2 == pX) && board[y - 1][x + 1].getRank() > 2)
        {
          isJumping = true;
        } //end if

        if (isJumping || ((!farRight && !farTop && (board[y - 1][x + 1].getRank() == 0 || ((y - 1) == pY && (x + 1) == pX)))))
        {
          weakPoints++;
        } //end if
        isJumping = false;

        if (x > 1 && y > 1 && (y - 2 == pY) && (x - 2 == pX) && board[y - 1][x - 1].getRank() > 2)
        {
          isJumping = true;
        } //end if

        if (isJumping ||(!farLeft && !farTop && (board[y - 1][x - 1].getRank() == 0 || ((y - 1) == pY && (x - 1) == pX))))
        {
          weakPoints++;
        } //end if
        isJumping = false;  
 
        if (x < 6 && y < 6 && (y + 2 == pY) && (x + 2 == pX) && board[y + 1][x + 1].getRank() > 1)
        {
          isJumping = true;
        } //end if

        if (isJumping || (!farRight && !farBottom && (board[y + 1][x + 1].getRank() == 0 || ((y + 1) == pY && (x + 1) == pX))))
        {
          weakPoints++;
        } //end if
        isJumping = false;

        if (x > 1 && y < 6 && (y + 2 == pY) && (x - 2 == pX) && board[y + 1][x - 1].getRank() > 2)
        {
          isJumping = true;
        } //end if
 
        if (isJumping || (!farLeft && !farBottom && (board[y + 1][x - 1].getRank() == 0 || ((y + 1) == pY && (x - 1) == pX))))
        {
          weakPoints++;
        } //end if
  
        if (y == 7 && (board[pY][pX].getRank() == 1))
        {
          board[y][x].setHeuristic(board[y][x].getHeuristic() + 7); 
        } //end if
  
        if (x == 0 || x == 7)
        { //if this piece is moving to the edge, it's worth more (because it has less options be jumped
          board[y][x].setHeuristic(board[y][x].getHeuristic() + 6);
        } //end if

        board[y][x].setHeuristic(board[y][x].getHeuristic() + (4 - weakPoints));
      } //end else       

      if (y > -1 && board[y][x].getHeuristic() > 0)
      { //used for testing
        if (board[y][x].getHeuristic() > bestLoc[4])
        { //if this value is better than the old one's, set index 2 of bestLoc (which holds the highest heuristic)
          bestLoc[4] = board[y][x].getHeuristic();
          bestLoc[0] = y;
          bestLoc[1] = x; 
        } //end if
      } //end if

      jumpCount = 0;
      weakPoints = 0;

      jumpCheck = true; 
      king = false;
      leftReg = false;
      rightReg = false;
      leftKing = false;
      rightKing = false;
      farLeft = false;
      farRight = false;
      farBottom = false;
      farTop = false;
      middle = false;
      evade = false;
      block = false;
      isJumping = false;
    } //end for
  
//  std :: cout << "BESTLOC 0 TO 5 in the end " << bestLoc[0] << " " << bestLoc[1] << " " << bestLoc[2] << " " << bestLoc[3] << " " << bestLoc[4] << std :: endl;
 
	
} //end heuristic



void move(Checker board[][8], int y, int x, int location, int bestLoc[5]) //called by moveCheck, actually "moves" the piece (really just makes the empty checker piece representing
{ //empty tiles a replica of the original piece and makes the original piece a null tile
  //location checks where the piece came from: 
  //if == 1, the piece is coming from moveCheck so print to the screen and take input for movement
  //if == 2, the piece is jumping: check if there is another possible jump at that location: if that is, run move with that location passed in
  //if == 3, the piece is called by the bot
  //if == 4, the piece is called by the bot and is jumping (jump acts a little differently if called by the bot so this step is necessary
  bool farLeft = false;
  bool farRight = false;
  bool farTop = false;
  bool farBottom = false;
  bool inputCheck = true; //checks if the user's input is okay
  bool yLocCheck = false;
  bool red;

  int prevHeur = 0; //used by the heuristic value for when the bot is jumping

  char countConv;

  std :: stringstream ss, ss2, ss3;

  //initializing this to a high number just to easily check the arrays
  int xLoc [4]; //initializing to 4 for when I incorporate backwards movement
  int yLoc [4]; //these hold the x and y locations of chosen tiles because I don't feel great about using/storing locaitons of two dimensional arrays
  int choice;
  int counter = 0; //counter will never be greater than 4: this is the maximum amount of options for each piece in checkers  

  if ((board[y][x].getRank() == 1) || (board[y][x].getRank() == 2))
  { //if this is a red piece and this is player 1, display player 1's options
    red = false;
  } //end if

  else if (board[y][x].getRank() > 2)
  { //if this is a black piece and this is player 2, display player 2's options
    red = true;
  } //end else if

  if (x == 0) //are we in the far left column?
  {
    farLeft = true;
  } //end if

  else if (x == 7) //are we in the far right column?
  {
    farRight = true;
  } //end else if

  if (y == 0)
  {
    farTop = true;
  } //end if

  else if (y == 7)
  {
    farBottom = true;
  } //end if

//in this case, farRight and farLeft if blocks don't have to be separated from the usual piece assignment blocks, because
//the usual assignment blocks have to do both. Check if not on extreme side due to this 


  if (!farLeft)
  {
    if (board[y][x].getRank() > 1)
    { //if the piece is a black king or red
      if (location %2 != 0 && board[y - 1][x - 1].getRank() == 0)
      {
        xLoc[counter] = (x - 1);
        yLoc[counter] = (y - 1);

        counter++; 
        ss << counter;
        ss >> countConv;
        board[y - 1][x - 1].setIdentity(countConv);
        ss.clear();
      } 
//*******************************************************************************************
//if you ever run into problems with this part of the program, EXAMINE THE STRINGSTREAM REUSE
      if (x > 1 && y > 1 && ((red && ((board[y - 1][x - 1].getRank() == 1) || (board[y - 1][x - 1].getRank() == 2))) || (!red && (board[y - 1][x - 1].getRank() > 2))) && board[y - 2][x - 2].getRank() == 0)
      { //check the back left for jumps
        xLoc[counter] = (x - 2);
//the reason I'm making both this and the next jump tile y-location negative is so that I can
//represent that the tile has been jumped to while still using integers
        yLoc[counter] = (y - 2) - 30;

        counter++;
        ss << counter;
        ss >> countConv;
        board[y - 2][x - 2].setIdentity(countConv);
        ss.clear();
      } //end if
    } //end if

   if (!red || board[y][x].getRank() == 4)
    {  //if black or red king
      if (!farBottom)
      {
        if (location %2 != 0 && board[y + 1][x - 1].getRank() == 0)
        {
          xLoc[counter] = (x - 1);
          yLoc[counter] = (y + 1);

          counter++;
          ss << counter;
          ss >> countConv;
          board[y + 1][x - 1].setIdentity(countConv);
          ss.clear();
        } //end if

        if (x > 1 && y < 6)
        {
          if ((red && ((board[y + 1][x - 1].getRank() == 1) || (board[y + 1][x - 1].getRank() == 2))) || (!red && board[y + 1][x - 1].getRank() > 2))
          { //if this is a red king and it has a black piece to jump, or if this is a black piece and it has a red piece to jump
            if (board[y + 2][x - 2].getRank() == 0)
            { //xLoc is set to negative to demonstrate that this was a backwards move. now that I look at it it would have made more logical sense to switch what xLoc and yLoc represent when negative but whateva
              yLoc[counter] = (y + 2) - 30;
              xLoc[counter] = (x - 2) - 30;

              counter++;
              ss << counter;
              ss >> countConv;
              board[y + 2][x - 2].setIdentity(countConv);
              ss.clear();
            } //end if
          } //end if
        } //end if
      } //end if
    } //end if
  } //end if
  //} //check here if something goes wrong

  if (!farRight)
  {
    if (board[y][x].getRank() > 1)
    { //if a black king or red piece
      if (!farTop)
      {
        if (location%2 != 0 && board[y - 1][x + 1].getRank() == 0)
        {
          xLoc[counter] = (x + 1);
          yLoc[counter] = (y - 1);

          counter++;
          ss << counter;
          ss >> countConv;
          board[y - 1][x + 1].setIdentity(countConv);
          ss.clear();
        } //end if

        if (x < 6 && y > 1 && ((red && (board[y - 1][x + 1].getRank() == 1 || board[y - 1][x + 1].getRank() == 2)) || (!red && board[y - 1][x + 1].getRank() > 2)))
        {
          if (board[y - 2][x + 2].getRank() == 0)
          {
            xLoc[counter] = (x + 2);
//I distinguish between tiles jumped to the left side and to the right side by subtracting by 100
            yLoc[counter] = (y - 2) - 100;

            counter++;
            ss << counter;
            ss >> countConv;
            board[y - 2][x + 2].setIdentity(countConv);
            ss.clear();
          } //end if
        } //end if
      } //end if
    } //end if

    if (!red || board[y][x].getRank() == 4)
    { //if black or red king
      if (location%2 != 0 && !farBottom && !farRight && board[y + 1][x + 1].getRank() == 0)
      //if (board[y + 1][x + 1].getRank() == 0)
      {
        xLoc[counter] = x + 1;
        yLoc[counter] = y + 1;

        counter++;
        ss << counter;
        ss >> countConv;
        board[y + 1][x + 1].setIdentity(countConv);
        ss.clear();
      } //end if

      else if (x < 6 && y < 6 && ((!red && board[y + 1][x + 1].getRank() > 2) || (red && board[y][x].getRank() == 4)))
      { //if this is a jumpable red piece
          if (board[y + 2][x + 2].getRank() == 0)
          {
            xLoc[counter] = (x + 2) - 30;
            yLoc[counter] = ((y + 2) - 100);

            counter++;
            ss << counter;
            ss >> countConv;
            board[y + 2][x + 2].setIdentity(countConv);
            ss.clear();
          } //end if
      } //end else if
    } //end if
  } //end if

  for (int i = 0; i < 4; i++)
  {
    if (location == 4 && yLoc[i] < 1000)
    {
/* 
        if (yLoc[i] < 0)
        {
          if (yLoc[i] <= -50)
          {
            yLoc[i] += 100;
            if (xLoc[i] < 0)
            {
              xLoc[i] += 30; 
              heuristic(board, y, x, bestLoc);
              xLoc[i] -= 30;
            } //end if
           
            else
            {
              heuristic(board, y, x, bestLoc);
            } //end else
            yLoc[i] -= 100; //reset all algebraic manipulations (which is why I have such clunky/repetitive conditional blocks over here)
          } //end if

          else
          {
            yLoc[i] += 30;
            if (xLoc[i] < 0)
            {
              xLoc[i] += 30;
              heuristic(board, y, x, bestLoc);
              xLoc[i] -= 30;
            } //end if

            else
            {
              heuristic(board, y, x, bestLoc);
            } //end else
            yLoc[i] -= 30;
          } //end else 
        } //end if

        else if (xLoc[i] < 0)
        {
          xLoc[i] += 30;
          heuristic(board, y, x, bestLoc);
          xLoc[i] -= 30;
        } //end else if
*/
      heuristic(board, y, x, bestLoc);
        
      if (bestLoc[4] > prevHeur) //store the best y and x piece location in addition to the best heuristic (for future comparison)
      {
        prevHeur = bestLoc[4];
        bestLoc[2] = yLoc[i];
        bestLoc[3] = xLoc[i];
      } //end if
    } //end if
  } //end for

  if (location < 3)
  {
    if (board[y][x].getRank() == 4)
    {
      board[y][x].setIdentity('K');
    } //end if
  
    else
    {
      board[y][x].setIdentity('x');
    } //end if
  
    while (inputCheck)
    {
      printBoard(board);
      std :: cin >> choice;
      if (std :: cin.fail())
      {
        std :: cout << "\nThat's not funny.\n";
        std :: cin.clear(); //clean tha stream
        std :: cin.ignore(std :: numeric_limits<std :: streamsize> :: max(), '\n');
        //the above two lines are used for flushing cin after a cin.fail. I got them from stack overflow, but as far as I understand:
        //the first one obviously clears the stream, then the second ignores everything left in the buffer to the end of the line. I think this is how it works anyway.
      } //end if
  
      else //check that the user isn't being a jerk
      {
        if ((0 < choice) && (choice <= counter))
        {
          inputCheck = false;
        } //end if
  
        else
        {
          std :: cout << "\nhahaha what a prankster ; ^ )))))\n\n";
        } //end else
      } //end else
    } //end while
  } //end if 

//TODO
//
//changed location from " > 2" to "== 3", I think this should work but just keep that in mind

  else if (location == 3) //if this is the bot
  {
    for (int i = 0; i < 4; i++)
    {     
      
      if ((yLoc[i] == 0) && (xLoc[i] == 0))
      {
        //if both are zero (which should never logically happen)
        //I was getting a really annoying bug where the bot would just randomly jump to position 0, 0 which should never happen and I don't know why it was happening but this seems to work so I'm content
      } //end if

      else if (((yLoc[i]%2 == 0) && (xLoc[i]%2 == 0)) || ((yLoc[i]%2 != 0) && (xLoc[i]%2 != 0)))
      {

      } //end else if

      else
      {
        if (yLoc[i] < 0)
        {
          if (yLoc[i] <= -50)
          {
            yLoc[i] += 100;
            if (yLoc[i] == bestLoc[0])
            {
              yLocCheck = true;
            } //end if
            yLoc[i] -= 100;
          } //end if
    
          else
          {
            yLoc[i] += 30;
            if (yLoc[i] == bestLoc[0]);
            {
              yLocCheck = true;
            } //end if
            yLoc[i] -= 30;
          } //end else
        } //end if
    
        else if (yLoc[i] == bestLoc[0])
        {
          yLocCheck = true;
        } //end else if
    
        if (xLoc[i] < 0)
        {
          xLoc[i] += 30;
          if (xLoc[i] == bestLoc[1])
          {
            if (yLocCheck) //if the value in yLoc[i] is also the same as the ideal move space
            {
              choice = 1 + i; //whatever location we're in + 1 (because below we're checking choice - 1 as the user will see index 0 as choice 1 for readability)
            } //end if
          } //end if
          xLoc[i] -= 30;
        } //end if
    
        else if (xLoc[i] == bestLoc[1])
        {
          if (yLocCheck)
          {
            choice = 1 + i;
          } //end if
        } //end else if
      } //end for
      } //end if
    } //end else
  if (location == 4) //if this is a bot jump, there is no need for input: set choice = 1 so that index 0 is checked, and set index zero to the best possible jump
  {
    choice = 1;
    yLoc[0] = bestLoc[2];
    xLoc[0] = bestLoc[3];
  } //end if

  if (yLoc[choice - 1] < 0) //is this number negative? if so, the piece jumped to get here
  {
    if (location == 1)
    {
      location = 2; //note that we're jumping
    } //end if

    else
    {
      location = 4;
    } //end if
    if (yLoc[choice - 1] <= -50) //was this number subtracted from 100? If so, the jump tile was to the right
    {
      yLoc[choice - 1] += 100; //undo the integer manipulation
      //I know this looks ugly. Because we're on the right, set the tile to the bottom right equal to zero
      //the piece has been jumped
      if (xLoc[choice - 1] > -1) //if it jumped from the bottom rather than the top (therefore the location will be different)
      {
        if (board[yLoc[choice - 1] + 1][xLoc[choice - 1] - 1].getRank() > 2)
        {
          redPieces--;
        } //end if

        else
        {
          blackPieces--;
        } //end else
        board[(yLoc[choice - 1] + 1)][(xLoc[choice - 1] - 1)].setRank(0); 
      } //end if

      else //if it jumped from the top
      { 
        xLoc[choice - 1] += 30;
        if (board[yLoc[choice - 1] - 1][xLoc[choice - 1] - 1].getRank() > 2)
        {
          redPieces--;
        } //end if

        else
        {
          blackPieces--;
        } //end else
        board[(yLoc[choice - 1] - 1)][(xLoc[choice - 1] - 1)].setRank(0);
      } //end else
    } //end if

    else //the tile is to the left side
    {
      yLoc[choice - 1] += 30;
      if (xLoc[choice - 1] > -1) //did this jump from the bottom?
      {
        if (board[yLoc[choice - 1] + 1][xLoc[choice - 1] + 1].getRank() > 2)
        {
          redPieces--;
        } //end if

        else
        {
          blackPieces--;
        } //end else
        board[(yLoc[choice - 1] + 1)][(xLoc[choice - 1] + 1)].setRank(0);
      } //end if

      else //this jumped from the top
      { 
        xLoc[choice - 1] += 30;
        if (board[yLoc[choice - 1] - 1][xLoc[choice - 1] + 1].getRank() > 2)
        {
          redPieces--;
        } //end if

        else
        {
          blackPieces--;
        } //end else
        board[(yLoc[choice - 1] - 1)][(xLoc[choice - 1] + 1)].setRank(0);
      } //end else
    } //end else
  } //end if

  if (board[y][x].getRank() == 3 && yLoc[choice - 1] == 0) //king me
  {
    board[y][x].setRank(4);
  } //end if

  else if (board[y][x].getRank() == 1 && yLoc[choice - 1] == 7)
  {
    board[y][x].setRank(2);
  } //end else if
//  std :: cout << "STATZ: yLoc, xLoc, y, x, and rank " << yLoc[(choice - 1)] << " " << xLoc[(choice - 1)] << " " << y << " " << x << " " << board[y][x].getRank() << std :: endl;
  board[yLoc[(choice - 1)]][xLoc[(choice - 1)]].setRank(board[y][x].getRank());
  board[yLoc[(choice - 1)]][xLoc[(choice - 1)]].setIdentity(' ');
  board[y][x].setRank(0); //the piece is moved
  board[y][x].setIdentity(' ');

  for (int i = 0; i < counter; i++) //reset the options to empty tiles
  {
    if (yLoc[i] < 0)
    {
      //to be perfectly honest I don't understand why the code above, which is the same thing, doesn't achieve this effect
      //so I put it here too. Reconverts the y value so the tile identity is successfuly reset
      if (yLoc[i] < -50)
      {
        yLoc[i] = (yLoc[i] + 100);
      } //end if

      else
      {
        yLoc[i] = (yLoc[i] + 30);
      } //end else
    } //end if
   
    if (xLoc[i] < 0)
    {
      xLoc[i] = (xLoc[i] + 30);
    } //end if
    board[yLoc[i]][xLoc[i]].setIdentity(' '); 
  } //end for

  //multijump section of the code: calls move with new jump location passed in if that new jump location has other jump options
  //it will pass in jumping = true which will make it so non-jump possibilities are not displayed
  if (location % 2 == 0) //if the location is even, so there is a jump happening either by the player or by the bot
  { //I could incorporate both colors into one block of code, but it already looks ugly enough that I'll just separate them by color.
    if (board[yLoc[(choice - 1)]][xLoc[(choice - 1)]].getRank() > 2) //if this piece is red
    { //TODO
      if (yLoc[(choice - 1)] > 1)
      {
        if ((xLoc[(choice - 1)] > 1) && ((board[yLoc[(choice - 1)] - 1][xLoc[(choice - 1)] - 1].getRank() == 1) || (board[yLoc[(choice - 1)] - 1][xLoc[(choice - 1)] - 1].getRank() == 2)))
        { //if the new location is past the second column and the tile to the upper left of this new location is an enemy piece, then if the tile behind that is empty, 
          if (board[yLoc[(choice - 1)] - 2][xLoc[(choice - 1)] - 2].getRank() == 0)
          {
            move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
          } //end if
        } //end if

        if ((xLoc[(choice - 1)] < 6) && ((board[yLoc[(choice - 1)] - 1][xLoc[(choice - 1)] + 1].getRank() == 1) || (board[yLoc[(choice - 1)] - 1][xLoc[(choice - 1)] + 1].getRank() == 2)))
        { //if the new location is past the second column and the tile to the upper left of this new location is an enemy piece, then if the tile behind that is empty, 
          if (board[yLoc[(choice - 1)] - 2][xLoc[(choice - 1)] + 2].getRank() == 0)
          {
            move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
          } //end if
        } //end if
      } //end if
    } //end if

    if (board[yLoc[(choice - 1)]][xLoc[(choice - 1)]].getRank() == 4) //if this is a king it has its own special jump opportunities
    { //check left side


/*
        moveCheck(board, 0, yOptions, xOptions);
        
        for (int i = 0; i < 12; i++)
        {
          if(yOptions[i] != -1) //if this was altered (because yLoc will never be filled with a negative 
          {
            std :: cout << "HEURISTIC\n";
            heuristic(board, yOptions[i], xOptions[i], bestLoc);
            std :: cout << "\n\nHERE ARE YOUR Y AND X OPTIONS IN MAIN " << yOptions[i] << " " << xOptions[i] << std :: endl;
            if (bestLoc[4] > prevHeur) //store the best y and x piece location in addition to the best heuristic (for future comparison)
            {
              prevHeur = bestLoc[4];
              bestLoc[2] = yOptions[i];
              bestLoc[3] = xOptions[i];
            } //end if 
          } //end if
        } //end for

*/

      if (yLoc[(choice - 1)] < 6)
      {
        if ((xLoc[(choice - 1)] > 1) && ((board[yLoc[(choice - 1)] + 1][xLoc[(choice - 1)] - 1].getRank() == 1) || (board[yLoc[(choice - 1)] + 1][xLoc[(choice - 1)] - 1].getRank() == 2)))
        { 
          if (board[yLoc[(choice - 1)] + 2][xLoc[(choice - 1)] - 2].getRank() == 0)
          {
            move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
          } //end if
        } //end if

        if ((xLoc[(choice - 1)] < 6) && ((board[yLoc[(choice - 1)] + 1][xLoc[(choice - 1)] + 1].getRank() == 1) || (board[yLoc[(choice - 1)] + 1][xLoc[(choice - 1)] + 1].getRank() == 2)))
        { //check right side
          if (board[yLoc[(choice - 1)] + 2][xLoc[(choice - 1)] + 2].getRank() == 0)
          {
            move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
          } //end if
        } //end if
      } //end if
    } //end if

    else if ((board[yLoc[(choice - 1)]][xLoc[(choice - 1)]].getRank() == 1) || (board[yLoc[(choice - 1)]][xLoc[(choice - 1)]].getRank() == 2)) //if this piece is black
    {
      if (yLoc[(choice - 1)] < 6)
      {
        if (xLoc[(choice - 1)] > 1 && board[yLoc[(choice - 1)] + 1][xLoc[(choice - 1)] - 1].getRank() > 2)
        { //if the new location is past the second column and the tile to the upper left of this new location is an enemy piece, then if the tile behind that is empty, 
          if (board[yLoc[(choice - 1)] + 2][xLoc[(choice - 1)] - 2].getRank() == 0)
          {
            move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
          } //end if
        } //end if
  
        if ((xLoc[(choice - 1)] < 6) && board[yLoc[(choice - 1)] + 1][xLoc[(choice - 1)] + 1].getRank() > 2)
        { //if the new location is past the second column and the tile to the upper left of this new location is an enemy piece, then if the tile behind that is empty, 
          if (board[yLoc[(choice - 1)] + 2][xLoc[(choice - 1)] + 2].getRank() == 0)
          {
            move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
          } //end if
        } //end if
      } //end if
  
      if (board[yLoc[(choice - 1)]][xLoc[(choice - 1)]].getRank() == 2) //if this is a king it has its own special jump opportunities
      { //check left side
        if (yLoc[(choice - 1)] < 6)
        { 
          if (xLoc[(choice - 1)] > 1 && board[yLoc[(choice - 1)] - 1][xLoc[(choice - 1)] - 1].getRank() > 2)
          { 
            if (board[yLoc[(choice - 1)] - 2][xLoc[(choice - 1)] - 2].getRank() == 0)
            {
              move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
            } //end if
          } //end if
  
          if ((xLoc[(choice - 1)] < 6) && board[yLoc[(choice - 1)] - 1][xLoc[(choice - 1)] + 1].getRank() > 2)
          { //check right side
            if (board[yLoc[(choice - 1)] - 2][xLoc[(choice - 1)] + 2].getRank() == 0)
            {
              move(board, yLoc[(choice - 1)], xLoc[(choice - 1)], location, bestLoc);
            } //end if
          } //end if
        } //end if
      } //end if
    } //end else if
  } //end if
} //end move

void moveCheck(Checker board[][8], int player, int yOptions[12], int xOptions[12]) //check for moves for the player, takes the checkerboard as a parameter
{
  std :: stringstream ss; //stringstream for converting counter to printable number
  std :: stringstream ss2; //I'm creating a separate ss each time I need it so as not to run into unexpected errors

  int choiceNum = 0;
  int choiceX, choiceY; //passed into the move function, holds the location of the piece chosen
  int numCheck; //used with stringstream for checking user input
  int i = 0; //just a counter to fill the options arrays
  int location = 1;
  int bestLoc[5]; //totally unecessary part of this function: this is really just used by heuristic but because I'm passing it into move I've gotta make SOMETHING here (I know this is sloppy)

  std :: string stringChoice; //holds the initial input, just in case it's more than one character long (for digits > 9)

  char pieceChoice, pieceChoice2; //which piece the user chooses, second is for if the number is 2 digits long

  bool farLeft = false; //checks if we're in the far left column
  bool farRight = false; //these are used to avoid segfaults when checking tiles to the left and right of that selected
  bool farTop = false; //awkward but I'll stick with my naming convention
  bool farBottom = false;
  bool isOption = false; //checks if the piece is even a choice so that non-option pieces do not display a number
  bool red; //tracks the color of the piece
  bool redPlayer; //combo bool that is true if !black and player = 1
  bool blackPlayer;
  bool leftOption = false; //checks if the tile to the left is an option so that the choice counter doesn't iterate twice
  bool topOption = false;
  bool inputCheck = true; //checks that the user gives real input

  char num1, num2; //holds the choice # we're on; if it's a number of length 2, store second num in num2
  
  for (int y = 0; y < 8; y++) //y value
  {
    for (int x = 0; x < 8; x++) //x value
    {
      if (board[y][x].getRank() != 0) //if this is a piece
      {
        if (board[y][x].getRank() > 2)
        {
          red = true; //compare different tiles depending on what color piece is being moved (assuming it's not kinged)
        }

        else if ((board[y][x].getRank() == 1) || (board[y][x].getRank() == 2))
        {
          red = false;
        } //end else

        if (red && player == 1)
        {
          redPlayer = true;
          blackPlayer = false;
        } //end if

        else if (!red && (player == 0 || player == 2))
        {
          blackPlayer = true;
          redPlayer = false;
        } //end else if

        else
        {
          blackPlayer = false;
          redPlayer = false;
        } //end else

        if (x == 0) //are we in the far left column?
        {
          farLeft = true;
        } //end if

        else if (x == 7) //are we in the far right column?
        {
          farRight = true;
        } //end else if

        if (y == 0)
        {
          farTop = true;
        } //end if

        else if (y == 7)
        {
          farBottom = true;
        } //end if

    if (redPlayer || (player != 1 && board[y][x].getRank() == 2)) //if this is a red piece or a black king
     {
       if (!farTop && !farLeft)
       {
         if (((board[y][x].getRank() == 2 && blackPlayer) || (board[y][x].getRank() > 2 && redPlayer)) && board[y - 1][x - 1].getRank() == 0) //is the tile to the upper left empty?
         { //if this is a black king and the player controls black or if this is a red player and a red piece
           leftOption = true;
           topOption = true;
           choiceNum++;
           isOption = true;
         } //end if

         else if (redPlayer && x > 1 && y > 1 && ((board[y - 1][x - 1].getRank() == 1) || (board[y - 1][x - 1].getRank() == 2)) && board[y - 2][x - 2].getRank() == 0)
         {
           leftOption = true;
           topOption = true;
           choiceNum++;
           isOption = true;
         } //end else if

         else if (blackPlayer && x > 1 && y > 1 && (board[y - 1][x - 1].getRank() > 2) && board[y - 2][x - 2].getRank() == 0)
         { //checks for black king backwards jump
           leftOption = true;
           topOption = true;
           choiceNum++;
           isOption = true;
         } //end else if
       } //end if 

//NOTE changed if to else if
       if (!farRight && !farTop && !leftOption)
       {
         if (((redPlayer && board[y][x].getRank() > 2) || (blackPlayer && board[y][x].getRank() < 3)) && board[y - 1][x + 1].getRank() == 0) //is the tile to the upper right empty?
         { 
           topOption = true;
           choiceNum++; //num will go up whenever there is another piece with an opportunity to move
           isOption = true;          
         } //end if

         else if (x < 6 && y > 1 && redPlayer && ((board[y - 1][x + 1].getRank() == 1) || (board[y - 1][x + 1].getRank() == 2)) && board[y - 2][x + 2].getRank() == 0 )
         { //check for red jumps
           topOption = true;
           choiceNum++;
           isOption = true;
         } //end else if

         else if (x < 6 && y > 1 && blackPlayer && board[y - 1][x + 1].getRank() > 2 && board[y - 2][x + 2].getRank() == 0)
         { //check for black king jumps
           topOption = true;
           choiceNum++;
           isOption = true;
         } //end else if
       } //end if
     } //end if
     
//NOTE changed to else if   
     if (blackPlayer || (player == 1 && board[y][x].getRank() == 4)) //is this a black piece or a red king
     {
       if (!farBottom && !topOption)
       {
         if (!farLeft)
         {
           if (((blackPlayer && board[y][x].getRank() < 3) || (redPlayer && board[y][x].getRank() == 4)) && board[y + 1][x - 1].getRank() == 0)
           {
             leftOption = true;
             choiceNum++;
             isOption = true;
           } //end if

           else if (x > 1)
           {
             if (board[y][x].getRank() == 4) //for checking the king but not black pieces
             {
               if ((board[y + 1][x - 1].getRank() == 1 || board[y + 1][x - 1].getRank() == 2) && board[y + 2][x - 2].getRank() == 0)
               {
                 leftOption = true;
                 choiceNum++;
                 isOption = true;
               } //end if
             } //end if

             else if (blackPlayer && (board[y + 1][x - 1].getRank() > 2) && board[y + 2][x - 2].getRank() == 0)
             {
               leftOption = true;
               choiceNum++;
               isOption = true;
             } //end else if
           } //end else if
         } //end if
  //NOTE changed if to else if
         if (!farRight && !leftOption) //if this isn't on the right edge and it isn't an option for the left (so we don't iterate the choice twice)
         {
           if (((blackPlayer && board[y][x].getRank() < 3) || (redPlayer && board[y][x].getRank() == 4)) && board[y + 1][x + 1].getRank() == 0)
           { //check to bottom right of black pieces and red kings
             choiceNum++;
             isOption = true;
           } //end if

           else if (x < 6)
           {
             if ((board[y][x].getRank() == 4) && (board[y + 1][x + 1].getRank() == 1 || board[y + 1][x + 1].getRank() == 2))
             { //if this is a king and the tile to the bottom right is jumpable
               if (board[y + 2][x + 2].getRank() == 0)
               {
                 choiceNum++;
                 isOption = true;
               } //end if
             } //end if

             else if (blackPlayer && (board[y + 1][x + 1].getRank() == 3 || board[y + 1][x + 1].getRank() == 4))
             { //if this is a black piece and the tile to the bottom right is jumpable
               if (board[y + 2][x + 2].getRank() == 0)
               {
                 choiceNum++;
                 isOption = true;
               } //end if
             } //end else if
           } //end else if
         } //end if
       } //end if
       } //end else if
     } //end if

      if (isOption) //if this piece is eligible for a move
      {
        if (player > 0)
        {
          ss << choiceNum; //stringstream to convert iterated int to char
          if (choiceNum > 9) //if choiceNum is a 2 digit number
          {
            ss >> num1 >> num2;
            board[y][x].setIdentity(num1);
            board[y][x].setLineSep2(num2);
          } //end if

          else
          {
            ss >> num1;
            board[y][x].setIdentity(num1);
          } //end else
        } //end if

        else if (player < 1)
        {
          yOptions[i] = y;
          xOptions[i] = x;
          i++;
        } //end else if
      } //end if

      ss.clear(); //clear the stringstream
      
      //reset booleans
      farLeft = false; 
      farRight = false;
      farTop = false;
      farBottom = false;
      isOption = false;
      leftOption = false;
      topOption = false;
    } //end for
  } //end for

  //print player turn, print nothing if it is the bot's turn
  if (player == 1)
  {
    std :: cout << "\nIt is red player's turn.\n";
  } //end if 

  else if (player == 2)
  {
    std :: cout << "\nIt is black player's turn.\n";
  } //end else if

  //if this is not a bot, ask them which they'd like to move and call move
  //otherwise store the x and y values to be given to the heuristic algorithm
  if (player > 0)
  {
    while (inputCheck)
    {
      printBoard(board);
  
      std :: cout << "Which would you like to move?" << std :: endl;
      std :: cin >> stringChoice;
 
      // used for checking for correct input  
      std :: stringstream convert(stringChoice);
      convert >> numCheck;
      convert.clear(); //I don't trust stringstream not to mess up somehow
      convert.str("");
  
      if ((numCheck < 1) || (numCheck > choiceNum)) //check that the user is entering something real
      {
        std :: cout << "\nCut it out\n";
      } //end if
  
      else
      {
        inputCheck = false;
      } //end else
    } //end while
  
    if (stringChoice.length() == 1)
    {
      pieceChoice = stringChoice[0];
      } //end if
  
    else if (stringChoice.length() == 2)
    {
      pieceChoice = stringChoice[0];
      pieceChoice2 = stringChoice[1];
    } //end else
  
    for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
      {
        if (stringChoice.length() == 1)
        {
          if (pieceChoice != board[y][x].getIdentity()) //if this is an integer
          { 
            board[y][x].setIdentity(' ');
          } //end if
  
          else if (pieceChoice == board[y][x].getIdentity())
          {
            choiceY = y;
            choiceX = x;
          } //end else if
        } //end if
  
        else if (stringChoice.length() == 2)
        { //the number is 2 digits long
          if (pieceChoice != board[y][x].getIdentity())
          {
            board[y][x].setIdentity(' ');
          } //end if
  
          else if (pieceChoice == board[y][x].getIdentity())
          {
            if (pieceChoice2 == board[y][x].getLineSep2())
            {
              choiceY = y;
              choiceX = x;
            } //end if
  
            else
            {
              board[y][x].setIdentity(' ');
            } //end else
          } //end else if
        } //end else
        board[y][x].setLineSep2(' ');
      } //end for
    } //end for
    move(board, choiceY, choiceX, location, bestLoc);
  } //end if
} //end moveCheck

/*
//decision tree will be called for each move option presented in moveCheck
//it will return an integer value which will be the "value" of each choice (determined through minimax and heuristics)
//creates nodes up to a given depth
int decisionTree(Checker board[][8])
{
  bool player1 = false;
  moveCheck(board, player1);
  return 0;
} //end decisionTree
*/

//"If you're trying to solve chess and it's ugly, I won't care, I care about the attempt"
//this is what andy says so don't get caught up in dumb little details, focus on the meat first
int main()
{
  int player;

  //initialize each array to -1 so I can see which locations of the array haven't been filled 
  //as moveCheck will never fill the array with a negative value
  int xOptions[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  int yOptions[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  int bestLoc[5] = {0, 0, 0, 0, 0}; //holds the y and x location of the best possible moves. Holds the x and y location of the best tile to move to, in addition to the highest heuristic value
  int prevHeur = 0; //holds the previous best heuristic value for comparing values

  Checker board[8][8]; //8x8 checkerboard w/ pieces. At the moment, I'm going to use the checker object as a placeholder, may change this to an int later on

  //used for checking input and keeping loops rollin
  char choice;

  bool keepGoing = true;
  bool menuInput = true;
  
  //1 = black/not a king, 3 = red/not a king
  //2 = black/king, 4 = red/king

  board[0][1].setRank(1);
  board[0][3].setRank(1);
  board[0][5].setRank(1);
  board[0][7].setRank(1);

  board[1][0].setRank(1);
  board[1][2].setRank(1);
  board[1][4].setRank(1);
  board[1][6].setRank(1);
  
  board[2][1].setRank(1);
  board[2][3].setRank(1);
  board[2][5].setRank(1);
  board[2][7].setRank(1);
  
  board[5][0].setRank(3);
  board[5][2].setRank(3);
  board[5][4].setRank(3);
  board[5][6].setRank(3);
  
  board[6][1].setRank(3);
  board[6][3].setRank(3);
  board[6][5].setRank(3);
  board[6][7].setRank(3);
  
  board[7][0].setRank(3);
  board[7][2].setRank(3);
  board[7][4].setRank(3);
  board[7][6].setRank(3); 


  while (menuInput)
  {
    std :: cout << "Would you like to play against another human or a bot?\n1. human\t2. bot";
    std :: cin >> choice;
    
    if ((choice != '1') && (choice != '2'))
    {
      std :: cout << "\n8\t^\t)\n";
    } //end if

    else
    {
      menuInput = false;
    } //end else
  } //end while

  if (choice == '1')
  {
    player = 1;
    while (keepGoing)
    {
      if (redPieces == 0) //did anyone win?
      {
        std :: cout << "\n\nBlack wins!\n\n";
        keepGoing = false;
      } //end if
  
      else if (blackPieces == 0)
      {
        std :: cout << "\n\nRed wins!\n\n";
        keepGoing = false;
      }

      moveCheck(board, player, yOptions, xOptions);
      
      if (keepGoing) //if the previous bit of code didn't end the program (so "would you like to keep going" doesn't print twice)
      {
        std :: cout << "Would you like to keep going?\n1.  yes\n2.  no\n";
        std :: cin >> choice;
        if (choice == '2')
        {
          keepGoing = false;
        } //end if
  
        std :: cin.ignore();
        
        if (player == 1) //change the player
        {
          player = 2;
        } //end if

        else
        {
          player = 1;
        } //end if
      } //end if
    } //end while
  } //end if

  else if (choice == '2')
  {
/*
    Node headNode;
    headNode.setValue(6);
    //std :: cout << headNode.getValue() << " end startnode";
    //Node *nodePoint = &startNode;
    Node myNode(3, NULL, &headNode);
    int myVar = decisionTree(board);
*/
    player = 1;
    while (keepGoing)
    {
      if (redPieces == 0)
      {
        std :: cout << "\n\nBeep boop beep humans suk >8 ^ )\n\n";
        player = 9; //set player to a number which will never lead to anything being executed just so we exit the game
        keepGoing = false;
      } //end if

      else if (blackPieces == 0)
      {
        std :: cout << "\n\nYou have defeated our machine overlords.\n\n";
        player = 9;
        keepGoing = false;
      } //end else if

      if (player == 1)
      {
        moveCheck(board, 1, yOptions, xOptions);
        player = 2;
      } //end if

      else if (player == 2)
      {
        moveCheck(board, 0, yOptions, xOptions);
        
        for (int i = 0; i < 12; i++)
        {
          if(yOptions[i] != -1) //if this was altered (because yLoc will never be filled with a negative 
          {
            heuristic(board, yOptions[i], xOptions[i], bestLoc);
            if (bestLoc[4] > prevHeur) //store the best y and x piece location in addition to the best heuristic (for future comparison)
            {
              prevHeur = bestLoc[4];
              bestLoc[2] = yOptions[i];
              bestLoc[3] = xOptions[i];
            } //end if 
          } //end if
        } //end for

        move(board, bestLoc[2], bestLoc[3], 3, bestLoc); //it's pretty dumb to pass in items in an array and then also the array but move requires x and y values so whatever

        player = 1; //switch turns
      } //end if 
      for (int i = 0; i < 12; i++)
      { 
        if (i < 5)
        {
          bestLoc[i] = 0;
        } //end if
        yOptions[i] = -1;
        xOptions[i] = -1;
      } //end for
      prevHeur = 0;  
    } //end while
  } //end else if  
} //end main
