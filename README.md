# Final
Final for CSCI-240, Computing 2. Basic checkers AI using heuristics.

##General info
Command line checkers game with the following basic format:

![checkersreadme](https://cloud.githubusercontent.com/assets/22689666/21791777/4a83b992-d6b3-11e6-858b-4cbd7fa64449.png)

The user can choose to play against another human, in which case they will just take turns. They can also play against a bot.

###Game Design
The checkerboard is a 2 dimensional array filled with checker objects. All of the functions in this program do not return anything, as most of the operations just involve manipulating and examining the game board. There is a "printBoard" function which is called each time something happens in the game, which displays a "b" for every black piece at a certain location, and an "r" for every red piece. If the piece is a king, it is displayed as uppercase. Otherwise, if there is no piece here, either a plus or a minus is displayed to represent black or white tiles.

This program basically involves a lot of scanning the board. A moveCheck function is responsible for checking for possible moves. It takes in a given player's color, then scans each checker on the board. If it is the same color and it has a possible move, it is displayed with a number. The user then inputs a number which is passed into a "move" function. This function then checks for each specific move that piece can make, displaying possible moves with number values. The user finally inputs where they want to move and the board is displayed again. If a piece was jumped, its color value is reset to 0 so it will not be displayed, and the score counter is decremented. If the move was a jump, the move function is recursively called to check for other possible moves to allow for double/triple/general multijumping.

###Checker Object
The checker object simply holds some basic checker properties, no methods beyond getters and setters. The properties include a heuristic value, a "rank" value (the identifying number: 0 means dead piece, 1 means black piece, 2 means black King, etc), an "identity" value which is what's displayed for each piece, and some other visual formatting characteristics.

###Heuristic
The heuristic is used by the AI and is used to determine a number value for each possible move, categorized in order of importance as follows: 
1. Can this piece multijump?
2. Can this piece jump?
3. Will this piece be blocking a jump? 
4. Will this piece be moving away from being jumped?
5. Will this piece be a king?
6. Will this piece be threatening another (presumably while staying safe from being jumped itself)
7. Will this piece be on the edge of the board? Center tiles are worse than edge tiles add 6
8. Is this piece a king and can it move towards enemy pieces? Add 5
9. Is it possible for this piece to be jumped on one side?
10. Is it possible for this piece to be jumped on both sides?
11. Will this piece be jumpable? Make the value negative if so, absolutely avoid that move

Each rank of move has its own number value modifier to the heuristic value of a given piece, ie if the piece is moving into a jump the heuristic is set to a hugely negative value (so the computer never chooses this), if it can jump but lands on a dangerous space a certain amount is added, but if it can jump and land on a safe space even more is added, etc. The computer chooses the best move given this value, then the x and y values of the chosen piece are passed into the move function. Instead of printing the options to the user, however, the possible moves are then passed back into heuristic to choose an appropriate move. 

##Issues
I originally wanted to use a minimax algorithm with my heuristic but didn't have enough time. As a result, my bot only looks one move ahead. Additionally, it still presents some strange behavior. I'm going to implement the minimax algorithm in the future once I perfect the heuristic functions. 
