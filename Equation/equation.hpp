#ifndef EQUATION_H
#define EQUATION_H

#include <string>
#include <vector>

class Equation {
private:
  
  const int N_OPERATIONS = 5;
  const double EPS = 1e-6;

  enum class Operation {
    ADD,
    SUB,
    MUL,
    DIV,
    POW
  };

  enum class Type {
    VALUE,
    OPERATION,
    VARIABLE,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS
  };

  struct Element {
    Type type;
    double val;
    Operation op;
    std::string var;
    inline bool isValue () const { return type == Type::VALUE; }
    inline bool isOperation () const { return type == Type::OPERATION; }
    inline bool isVariable () const { return type == Type::VARIABLE; }
    inline bool isLeftParenthesis () const { return type == Type::LEFT_PARENTHESIS; }
    inline bool isRightParenthesis () const { return type == Type::RIGHT_PARENTHESIS; }
  };

  struct Node {
    Element* elem;
    Node* left = 0;
    Node* right = 0;
    Node () {}
    ~Node () {}
  };

  std::string equation;
  std::string equivalence;
  std::vector <Element> array;
  std::vector <int> level;
  std::vector <std::vector <std::vector <int>>> nearest; // [operation][position][level]
  Node* root = 0;

  void deleteSpaces ();
  void reduceSigns ();
  void createArray ();
  void transformUnaryOperations ();
  void computeLevel ();
  void computeRightNearestOperations ();
  void computeRightNearest (std::vector <std::vector <int>>& row, Operation op);
  void buildTree ();
  void build (int l, int r, Node*& cur);
  int findNearestOperation (int l, int r, Operation op);
  double eval (Node* cur);

public:
  Equation () {}
  void setEquation (std::string _equation) { equation = _equation; }
  inline std::string getEquation () const { return equation; }
  static Equation* buildFromEquation (std::string equation);
  double eval ();
};

#endif
