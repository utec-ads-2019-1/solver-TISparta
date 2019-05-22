#include <bits/stdc++.h>

#include "equation.hpp"

#define sz(A) int(A.size())

using namespace std;

Equation* Equation::buildFromEquation (string equation) {
  Equation* ret = new Equation();
  ret -> setEquation(equation);
  ret -> deleteSpaces();
  ret -> reduceSigns();
  ret -> createArray();
  ret -> transformUnaryOperations();
  ret -> computeLevel();
  ret -> computeRightNearestOperations();
  ret -> buildTree();
  return ret;
}

void Equation::deleteSpaces () {
  equivalence = "";
  for (char ch: equation) {
    if (ch == ' ') continue;
    equivalence += ch;
  }
}

void Equation::reduceSigns () {
  string newEquivalence = "";
  for (int i = 0; i < sz(equivalence); i++) {
    if (equivalence[i] != '+' and equivalence[i] != '-') {
      newEquivalence += equivalence[i];
      continue;
    }
    int cntNeg = (equivalence[i] == '-');
    int j = i;
    while (j + 1 < sz(equivalence) and
        (equivalence[j + 1] == '+' or equivalence[j + 1] == '-')) {
      j++;
      cntNeg += (equivalence[j] == '-');
    }
    newEquivalence += (cntNeg % 2 == 0) ? '+' : '-';
    i = j;
  }
  equivalence = newEquivalence;
}

pair <double, int> getNumber (const string& s, int pos) {
  string num = "";
  int j = pos;
  while (j < sz(s) and isdigit(s[j])) { // Integer Part
    num += s[j];
    j++;
  }
  num += '.';
  if (j < sz(s) and s[j] == '.') j++;
  while (j < sz(s) and isdigit(s[j])) { // Decimal Part
    num += s[j];
    j++;
  }
  if (num.back() == '.') num += '0';
  return {stod(num), j};
}

bool isValidForVariableName (const char ch) {
  return ch == '_' or
         ('a' <= ch and ch <= 'z') or
         ('A' <= ch and ch <= 'Z');
}

pair <string, int> getVariable (const string& s, int pos) {
  string var = "";
  int j = pos;
  while (j < sz(s) and isValidForVariableName(s[j])) {
    var += s[j];
    j++;
  }
  return {var, j};
}

void Equation::createArray () {
  for (int i = 0; i < sz(equivalence);) {
    Element elem;
    if (isdigit(equivalence[i])) { // A number
      pair <double, int> pp = getNumber(equivalence, i);
      elem.type = Type::VALUE;
      elem.val = pp.first;
      i = pp.second;
    } else if (equivalence[i] == '(') { // Left parenthesis
      elem.type = Type::LEFT_PARENTHESIS;
      i = i + 1;
    } else if (equivalence[i] == ')') { // Right parenthesis
      elem.type = Type::RIGHT_PARENTHESIS;
      i = i + 1;
    } else if (equivalence[i] == '+') { // Operation +
      elem.type = Type::OPERATION;
      elem.op = Operation::ADD;
      i = i + 1;
    } else if (equivalence[i] == '-') { // Operation -
      elem.type = Type::OPERATION;
      elem.op = Operation::SUB;
      i = i + 1;
    } else if (equivalence[i] == '*') { // Operation *
      elem.type = Type::OPERATION;
      elem.op = Operation::MUL;
      i = i + 1;
    } else if (equivalence[i] == '/') { // Operation /
      elem.type = Type::OPERATION;
      elem.op = Operation::DIV;
      i = i + 1;
    } else if (equivalence[i] == '^') { // Operation ^
      elem.type = Type::OPERATION;
      elem.op = Operation::POW;
      i = i + 1;
    } else if (isValidForVariableName(equivalence[i])) { // A variable
      pair <string, int> pp = getVariable(equivalence, i);
      elem.type = Type::VARIABLE;
      elem.var = pp.first;
      i = pp.second;
    } else throw "Invalid equation";
    array.push_back(elem);
  }
}

// -x = (-1) * x
// +x = x
void Equation::transformUnaryOperations () {
  vector <Element> newArray;
  Element prev;
  prev.type = Type::LEFT_PARENTHESIS;
  for (Element cur: array) {
    if (cur.isOperation()) {
      if (prev.isLeftParenthesis() or prev.isOperation()) { // Unary operation
        if (cur.op == Operation::ADD) {} // unary +
        else if (cur.op == Operation::SUB) { // unary -
          Element minus1;
          minus1.type = Type::VALUE;
          minus1.val = -1;
          newArray.push_back(minus1);
          Element mul;
          mul.type = Type::OPERATION;
          mul.op = Operation::MUL;
          newArray.push_back(mul);
        } else throw "Invalid use of unary operations";
      } else {
        newArray.push_back(cur);
      }
    } else {
      newArray.push_back(cur);
    }
    prev = cur;
  }
  prev.type = Type::LEFT_PARENTHESIS;
  for (Element cur: array) {
    if (cur.isOperation() and prev.isOperation()) {
      throw "Error with unary operations";
    }
    prev = cur;
  }
  array = newArray;
}

void Equation::computeLevel () {
  int open = 0;
  level.assign(sz(array), -1);
  for (int i = 0; i < sz(array); i++) {
    if (array[i].isLeftParenthesis()) open++;
    if (array[i].isRightParenthesis()) open--;
    if (open < 0) throw "Invalid use of parenthesis";
    level[i] = open;
  }
  if (open != 0) throw "Invalid use of parenthesis";
}

void Equation::computeRightNearest (vector <vector <int>>& row, Operation op) {
  if (row.empty()) return;
  vector <int> near(sz(row[0]), -1);
  for (int i = sz(array) - 1; i >= 0; i--) {
    if (array[i].isOperation() and array[i].op == op) {
      near[level[i]] = i;
    }
    row[i] = near;
  }
}

void Equation::computeRightNearestOperations () {
  int maxLevel = *max_element(begin(level), end(level));
  // [operation][position][level]
  nearest = vector <vector <vector <int>>> (N_OPERATIONS, vector <vector <int>> (sz(array), vector <int> (maxLevel, -1)));
  computeRightNearest(nearest[0], Operation::ADD);
  computeRightNearest(nearest[1], Operation::SUB);
  computeRightNearest(nearest[2], Operation::MUL);
  computeRightNearest(nearest[3], Operation::DIV);
  computeRightNearest(nearest[4], Operation::POW);
}

int Equation::findNearestOperation (int l, int r, Operation op) {
  int pos = nearest[int(op)][l][level[l]];
  return (l <= pos and pos <= r) ? pos : -1;
}

void Equation::build (int l, int r, Node*& cur) {
  cur = new Node();
  // (exp)
  if (array[l].isLeftParenthesis() and array[r].isRightParenthesis()) {
    build(l + 1, r - 1, cur);
    return;
  }
  if (l == r) {
    cur -> elem = &array[l];
    return;
  }
  // exp1 +- exp2
  int posAdd = findNearestOperation(l, r, Operation::ADD);
  int posSub = findNearestOperation(l, r, Operation::SUB);
  if (posAdd != -1 and posSub != -1) {
    if (posAdd < posSub) {
      cur -> elem = &array[posAdd];
      build(l, posAdd - 1, cur -> left);
      build(posAdd + 1, r, cur -> right);
    } else {
      cur -> elem = &array[posSub];
      build(l, posSub - 1, cur -> left);
      build(posSub + 1, r, cur -> right);
    }
    return;
  }
  if (posAdd != -1) {
    cur -> elem = &array[posAdd];
    build(l, posAdd - 1, cur -> left);
    build(posAdd + 1, r, cur -> right);
    return;
  }
  if (posSub != -1) {
    cur -> elem = &array[posSub];
    build(l, posSub - 1, cur -> left);
    build(posSub + 1, r, cur -> right);
    return;
  }
  // exp1 */ exp2
  int posMul = findNearestOperation(l, r, Operation::MUL);
  int posDiv = findNearestOperation(l, r, Operation::DIV);
  if (posMul != -1 and posDiv != -1) {
    if (posMul < posDiv) {
      cur -> elem = &array[posMul];
      build(l, posMul - 1, cur -> left);
      build(posMul + 1, r, cur -> right);
    } else {
      cur -> elem = &array[posDiv];
      build(l, posDiv - 1, cur -> left);
      build(posDiv + 1, r, cur -> right);
    }
    return;
  }
  if (posMul != -1) {
    cur -> elem = &array[posMul];
    build(l, posMul - 1, cur -> left);
    build(posMul + 1, r, cur -> right);
    return;
  }
  if (posDiv != -1) {
    cur -> elem = &array[posDiv];
    build(l, posDiv - 1, cur -> left);
    build(posDiv + 1, r, cur -> right);
    return;
  }
  // exp1 ^ exp2
  int posPow = findNearestOperation(l, r, Operation::POW);
  if (posPow != -1) {
    cur -> elem = &array[posPow];
    build(l, posPow - 1, cur -> left);
    build(posPow + 1, r, cur -> right);
    return;
  }
  cout << l << ' ' << r << ' ' << posAdd << ' ' << posSub << ' ' << posMul << ' ' << posDiv << ' ' << posPow << endl;
  throw "Something got wrong building the three";
}

void Equation::buildTree () {
  build(0, sz(array) - 1, root);
}

double Equation::eval () {
  return eval(root);
}

double Equation::eval (Node* cur) {
  if (not cur -> left and not cur -> right) {
    if (cur -> elem -> isValue()) return cur -> elem -> isValue();
    if (cur -> elem -> isVariable()) {} // TODO
    throw "Something got wrong while evaluating the equation";
  }
  if (not cur -> elem -> isOperation()) {
    throw "Something got wrong while evaluating the eqaution";
  }
  if (cur -> elem -> op == Operation::ADD) {
    return eval(cur -> left) + eval(cur -> right);
  }
  if (cur -> elem -> op == Operation::SUB) {
    return eval(cur -> left) - eval(cur -> right);
  }
  if (cur -> elem -> op == Operation::MUL) {
    return eval(cur -> left) * eval(cur -> right);
  }
  if (cur -> elem -> op == Operation::DIV) {
    double div = eval(cur -> right);
    if (fabs(div - 0) < EPS) throw "Error: Division by zero!";
    return eval(cur -> left) / div;
  }
  if (cur -> elem -> op == Operation::POW) {
    return pow(eval(cur -> left), eval(cur -> right));
  }
}
