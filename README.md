# Final
Final for CSCI-240, Computing 2. Basic checkers AI using heuristics.

##General info
Command line checkers game with the following basic format:

![checkersreadme](https://cloud.githubusercontent.com/assets/22689666/21791777/4a83b992-d6b3-11e6-858b-4cbd7fa64449.png)

The user can choose to play against another human, in which case they will just take turns. They can also play against a bot.

###Game Design
The checkerboard is a 2 dimensional array filled with checker objects. There is a "printBoard" function which is called each time something happens in the game, which displays a "b" for every black piece at a certain location, and an "r" for every red piece. If the piece is a king, it is displayed as uppercase. Otherwise, if there is no piece here, either a plus or a minus is displayed to represent black or white tiles.



###Checker Object
