#include "Lex.h"

Lex::Lex() {
    identifier = "";
    exec = false;
}

Lex::Lex(int val) {
    identifier = "";
    type = L_INT_VAR;
    int_val = val;
}

Lex::Lex(std::string val) {
    identifier = "";
    type = L_STR_VAR;
    str_val = val;
}

Lex::Lex(bool val) {
    identifier = "";
    type = L_BOOL_VAR;
    bool_val = val;
}
