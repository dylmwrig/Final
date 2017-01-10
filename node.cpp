#include "node.h"

#include <cstddef>

Node :: Node()
{
  value = 0;
  next = NULL;
  prev = NULL;
} //end constructor

Node :: Node(int value, Node *next, Node *prev)
{
  Node :: value = value;
  Node :: next = next;
  Node :: prev = prev;
} //end overloaded constructor

//getters
int Node :: getValue()
{
  return value;
} //end getter

Node* Node :: getNext()
{
  return next;
} //end getter

Node* Node :: getPrev()
{
  return prev;
} //end getter

//setters
void Node :: setValue(int value)
{
  Node :: value = value;
} //end setter

void Node :: setNext(Node *next)
{
  Node :: next = next;
} //end setter

void Node :: setPrev(Node *prev)
{
  Node :: prev = prev;
} //end setter

