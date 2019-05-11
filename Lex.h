#ifndef LEX_H
#define LEX_H
#endif

#include "lex_type.h"
#include <string>

class Lex {
    public:
        bool exec;
        Lex ();
        Lex (int val);
        Lex (std::string val);
        Lex (bool val);
        lex_type type;
        std::string identifier;
        int int_val;
        bool bool_val;
        std::string str_val;
};
