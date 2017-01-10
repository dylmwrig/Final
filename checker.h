//checker header
//checker requires 
//  -rank
//  -location
//  -potentially value (tied to the minimax algorithm)

class Checker
{
  private:
  int rank;
  int printNum; //used for allowing user to pick piece
  int heuristic; //holds the heuristic value of each tile
  char identity; //the identifying character which will be printed on the checkerboard
  char lineSep2; //is "|" for most pieces but will become a number if numChoice is 2 digits long 
                 //when the user is picking the piece they want to manipulate
  public:
  //getters
  int getRank(); 
  int getPrintNum();
  int getHeuristic();
  char getIdentity();
  char getLineSep2();

  //setters
  void setRank(int rank);
  void setHeuristic(int heuristic);
  void setPrintNum(int printNum);
  void setIdentity(char identity);
  void setLineSep2(char lineSep2);

  //needs to have location value passed, once I understand how to pass a multidimensional array location
  //starting this as a void: This may return a value though, only do this temporarily
  void move();

  Checker();
  ~Checker(); //deconstructor, for if I decide to put this on the heap
};//end class def
