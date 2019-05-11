#ifndef SCANNER_H
#define SCANNER_H
#endif

#include <vector>
#include <string>
#include <stdio.h>
#include <cctype>
#include <algorithm>
#include <iostream>
#include "Lex.h"

class Scanner {
public:
    Scanner ();
    Scanner(char * program);
	std::vector<Lex> Check();
    std::vector<Lex> lex;
    std::vector<std::string> TSW = {"int", "bool", "string", "if", "while", "true", "false", "print", "input"};
    std::vector<Lex> TID;
    std::vector<Lex> POLIZ;
    //in POLIZ[i] is the operator, POLIZ [i-1] is the first operand, etc, Но не всё так просто
    Lex findInTID (std::string toFind);
    Lex* lFindInTID (std::string toFind);

private:
    long int curr_line;

    FILE * fp;
    char c;

    void gc ();

    void skipSpaces ();
    bool inTSW (std::string toCheck);
    bool inTID (Lex toCheck);
    bool inTID (std::string toCheck);

    int readInt ();

	bool stateB();              //  B -> { '{' S '}' }
	bool stateS();              //  S -> I = E | I I = E | if (E) B {else B} | while (E) B | input (I) | print (F)
    bool stateE();              //  E -> E1 [==|<|>|!=] E1 | E1
    bool stateE1();             //  E1-> T { [ + | - | "||" ] T}
    void stateT();              //  T -> F {[* | / | "&&" ] F}
    void stateF();              //  F -> I | N | L | !F | (E)
                                //  L -> "true" | "false"
    std::string stateI();       //  I -> C | IC | IR
                                //  N -> R | NR
                                //  C -> "a" | "b" | ... | "z" | "A" | .. | "Z" |
                                //  R -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 };
