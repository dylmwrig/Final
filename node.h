/*node used by the AI in building a decision tree
 *holds the location of the next node and the value of the given move decisions
 */

#ifndef NODE_H
#define NODE_H

class Node
{
  private:
  int value;
  Node *prev;
  Node *next;

  public: 
  Node();
  Node(int value, Node *next, Node *prev);

  //getters
  int getValue();
  Node* getNext();
  Node* getPrev();

  //setters
  void setValue(int value);
  void setNext(Node* next);
  void setPrev(Node* prev);
}; //end Node

#endif
