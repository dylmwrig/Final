//implement the checker header
//

#include "checker.h"

#include <iostream>

//getters
int Checker :: getRank()
{
  return rank;
}

int Checker :: getPrintNum()
{
  return printNum;
} //end getter

int Checker :: getHeuristic()
{
  return heuristic;
} //end getter

char Checker :: getIdentity()
{
  return identity;
} //end getter

char Checker :: getLineSep2()
{
  return lineSep2;
} //end getter

//setters
void Checker :: setRank(int rank)
{
  Checker :: rank = rank;
} //end setter

void Checker :: setPrintNum(int printNum)
{
  Checker :: printNum = printNum;
} //end setter

void Checker :: setHeuristic(int heuristic)
{
  Checker :: heuristic = heuristic;
} //end setter

void Checker :: setIdentity(char identity)
{
  Checker :: identity = identity;
} //end setter

void Checker :: setLineSep2(char lineSep2)
{
  Checker :: lineSep2 = lineSep2;
} //end setter

Checker :: Checker()
{
  rank = 0; //initialize the rank to zero so each space w/o a piece is registered as empty
  identity = ' '; //leaving this uninitialized was giving me such strange errors before...
  lineSep2 = ' ';
  heuristic = 0;
} //end constructor 

Checker :: ~Checker() //may or may not need a deconstructor
{

} //end destructor
