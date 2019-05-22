#include "tester.hpp"
#include "../../Equation/equation.hpp"

void Tester::execute() {
  string equations[] = {
    "7/4*((3+1)*3)+3",
    "7/4*((2+-+--8)*2)+3",
    "2+(3)",
    "7/4*((1+2)^4*2)+3",
    "7*-1",
    "1-2*5/2+1-2^4+1",
    "19+-+++-++++++9",
    "((7*3)/4+6*(3^2)/2)*(-1)",
    "(5+-3)+(21/3*5)-(5^3-2)",
    "((19--45/16*100-(181^2-15*10)))", // Faltaba poner un paréntesis al final
    // Extras
    "(0)",
    "+10",
    "10/0",
    "3 + 5 * 2",
    "20-16/4",
    "60-30/3*5+7",
    "900/(6+3*8)-10",
    "4-3 * (20 - 3 * 4 - (2 + 4))/2",
    "6 / (3) * (2)",
    "14+28/2^2",
    "3^2*2^3",
    "3*5^-3/7",
    "(3 + 4) ^ 2 + (8) * (4)"
  };

  double results[] = {
    24,
    -18,
    5,
    286.5,
    -7,
    -18,
    28,
    -32.25,
    -86,
    -32310.75,
    // Extras
    0,
    10,
    0, // Error: Division by zero,
    13,
    16,
    17,
    20,
    1,
    4,
    21,
    72,
    0.0034,
    81
  };

  const unsigned int size = sizeof(equations) / sizeof(string);
  for (int i = 0; i < int(size); ++i) {
    cout << "Equation -> " << equations[i] << endl;
    try {
      Equation* root = Equation::buildFromEquation(equations[i]);
      cout << "Equation -> " << root -> getEquation() << endl;
      cout << "Equivalence -> " << root -> getEquivalence() << endl;
      double respuesta = root -> eval();
      if (not (fabs(respuesta - results[i]) < 1e-3)) cout <<  "The solver is not working" << endl;
      else cout << "Equation(" << i + 1 << ") solved" << endl;
      delete root;
    } catch (char const* msg) {
      cerr << msg << endl;
    }
  }
  cout << "############################" << endl;
  vector <string> more_equations = {
    "7/4*((a+b)*a)+3",
    "7/4*((a+-+--b)*a)+3",
    "2+(3)",
    "7/4*((a+b)^4*a)+3",
    "a-2*5/2+1-2^4+a",
    "a+-+++-++++++9",
    "((7*3)/4+6*(a^2)/b)*(1)"
  };
  for (string equation: more_equations) {
    Equation* root = Equation::buildFromEquation(equation);
    cout << "Equation -> " << root -> getEquation() << endl;
    cout << "Equivalence -> " << root -> getEquivalence() << endl;
    root -> askValues();
    double respuesta = root -> eval();
    cout << "Respuesta: " << respuesta << endl;
    delete root;
  }
}
