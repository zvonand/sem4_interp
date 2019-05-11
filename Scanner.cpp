#include "Scanner.h"
//WARNING: Lies niemals diese File, Gefahr von Geisteskrankheit

Scanner::Scanner(char * program) {
    fp = fopen ( program, "r" );
    curr_line = 1;
    gc();
}

Scanner::Scanner() {
}


void Scanner::gc () {
    if (c == '\n'){
        curr_line++;
    }
    c = fgetc (fp);
}

void Scanner::skipSpaces () {
    while (c == ' ' || c == '\n' || c == '\t') {
        gc();
    }
    return;
}

bool Scanner::inTSW (std::string toCheck) {
    for (int i = 0; i < TSW.size(); i++) {
        if (TSW[i] == toCheck) {
            return true;
        }
    }
    return false;
}

bool Scanner::inTID (Lex toCheck) {
    for (auto ir = TID.crbegin(); ir != TID.crend(); ++ir) {
        if (toCheck.identifier == ir->identifier) {
            return true;
        }
    }
    return false;
}

bool Scanner::inTID (std::string toCheck) {
    for (auto ir = TID.crbegin(); ir != TID.crend(); ++ir) {
        if (toCheck == ir->identifier) {
            return true;
        }
    }
    return false;
}

int Scanner::readInt () {
    int koeff = 1;
    if (c == '-') {
        koeff = -1;
        gc ();
        skipSpaces ();
    }
    int res = 0;
    while (std::isdigit (c)) {
        res *= 10;
        res += (c - '0');
        gc ();
    }
    res *= koeff;
    return res;
}


Lex Scanner::findInTID (std::string toFind) {
    auto ir = TID.crbegin();
    for (ir = TID.crbegin(); ir != TID.crend(); ++ir) {
        if (toFind == ir->identifier) {
            break;
        }
    }
    return *ir;
}

Lex* Scanner::lFindInTID (std::string toFind) {
    int i;
    for (i = 0; i < TID.size (); i++) {
        if (toFind == TID[i].identifier) {
            break;
        }
    }
    return &(TID[i]);
}

std::vector<Lex> Scanner::Check()
{
    stateB ();
    return POLIZ;

}

bool Scanner::stateB() {
    skipSpaces ();
	if( c == '{' ) {
        gc ();
        skipSpaces ();
        while (c != '}'){
            stateS();
            if (c != ';' && c != '}') {
                printf ("Error: ';' expected, %c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            skipSpaces ();
        }
	} else {
        printf ("Error: '{' expected, %c found at line %li\n", c, curr_line);
        exit (EXIT_FAILURE);
    }
	return false;
}

//IDEA: by default, all newly declared variables withoud default value should be 0, false or ""
bool Scanner::stateS () {
    skipSpaces ();
    if (c == '}') {
        return true;
    }
	if( std::isalpha (c) ) {
        Lex curr_lex;
        std::string token = stateI ();

        skipSpaces ();
        if (token == "if") {
            curr_lex.type = L_IF;
            skipSpaces ();
            if (c != '(') {
                printf ("Error: ( expected at line %li\n", curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            skipSpaces ();
            stateE ();
            skipSpaces ();
            if (c != ')') {
                printf ("Error: ) expected at line %li\n", curr_line);
                exit (EXIT_FAILURE);
            }
            curr_lex.exec = true;
            POLIZ.push_back (curr_lex);
            gc();
            stateB();
            curr_lex.type = L_ENDIF;
            POLIZ.push_back (curr_lex);
            skipSpaces ();
        } else if (token == "while") {
            curr_lex.type = L_IFBGN;
            POLIZ.push_back (curr_lex);
            skipSpaces ();
            if (c != '(') {
                printf ("Error: ( expected at line %li\n", curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            skipSpaces ();
            int cond = stateE ();
            skipSpaces ();
            if (c != ')') {
                printf ("Error: ) expected at line %li\n", curr_line);
                exit (EXIT_FAILURE);
            }
            curr_lex.type = L_IF;
            curr_lex.exec = true;
            POLIZ.push_back (curr_lex);
            gc();
            stateB();
            curr_lex.type = L_WGT;
            POLIZ.push_back (curr_lex);
            curr_lex.type = L_ENDIF;
            POLIZ.push_back (curr_lex);
            skipSpaces ();
        } else if (token == "print") {
            skipSpaces ();
            if (c != '(') {
                printf ("Error: ( expected in function call, %c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            stateF ();
            skipSpaces ();
            if (c != ')') {
                printf ("Error: ) expected in function call, %c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            skipSpaces ();
            Lex op;
            op.type = L_PRINT;
            op.exec = true;
            POLIZ.push_back (op);

        } else if (token == "input") {
            skipSpaces ();
            if (c != '(') {
                printf ("Error: ( expected in function call, %c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            if (!std::isalpha (c) && !std::isdigit (c)) {
                printf ("Error: unexpected %c at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            stateI ();
            skipSpaces ();
            if (c != ')') {
                printf ("Error: ) expected in function call, %c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            skipSpaces ();
            Lex op;
            op.type = L_INPUT;
            op.exec = true;
            POLIZ.push_back (op);
        } else if (c != ';') {
            Lex topush = POLIZ.back();
            POLIZ.pop_back ();
            //already defined, now using it
            if (!inTID (token)) {
                printf ("Error: undefined name %s at line %li\n", token.c_str(), curr_line);
                exit (EXIT_FAILURE);
            }
            skipSpaces ();
            if (c != '=') {
                printf ("Error: unexpected %s at line %li\n", token.c_str(), curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            skipSpaces ();
            stateE ();
            skipSpaces ();
            POLIZ.push_back (topush);
            Lex eq;
            eq.type = L_EQ;
            eq.exec = true;
            POLIZ.push_back (eq);
            if (c != ';') {
                printf ("Error: ; expected, %c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
        }

	} else {
        printf ("Error: unexpected %c found at line %li\n", c, curr_line);
        exit (EXIT_FAILURE);
    }
}

std::string Scanner::stateI () {
    //if " met, then it is a value, if not - an identifier, if service word met - we dont push it, it is processed earlier
    skipSpaces ();
    Lex curr_lex;
    std::string curr_lex_str = "";
    if (c == '"') {
        gc ();
        while (c != '"') {
            if (c == '\n') {
                printf ("Error: \" expected, newline found at line %li\n", curr_line);
                exit (EXIT_FAILURE);
            }
            curr_lex_str += c;
            gc ();
        }
        curr_lex.type = L_STR_VAR;
        curr_lex.str_val = curr_lex_str;
        POLIZ.push_back (curr_lex);
        gc ();
    } else if( std::isalpha (c) ) {
        while (std::isalpha (c) || std::isdigit (c)) {
            curr_lex_str += c;
            gc ();
        }
        skipSpaces ();
        if (!inTSW (curr_lex_str) && !inTID (curr_lex_str)) {
            printf ("Error: undefined name %s at line %li\n", curr_lex_str.c_str(), curr_line);
            exit (EXIT_FAILURE);
        } else if (inTSW (curr_lex_str)) {
            if (curr_lex_str == "int" || curr_lex_str == "bool" || curr_lex_str == "string") {
                if (curr_lex_str == "int") {
                    curr_lex.type = L_INT_VAR;
                } else if (curr_lex_str == "bool") {
                    curr_lex.type = L_BOOL_VAR;
                } else if (curr_lex_str == "string") {
                    curr_lex.type = L_STR_VAR;
                }
                curr_lex_str = "";
                if (std::isalpha (c)) {
                    while (std::isalpha (c) || std::isdigit (c)) {
                        curr_lex_str += c;
                        gc ();
                    }
                    skipSpaces ();
                } else {
                    printf ("Error: unexpected %s at line %li\n", curr_lex_str.c_str(), curr_line);
                    exit (EXIT_FAILURE);
                }
                if (c == ';') {
                    if (curr_lex.type == L_INT_VAR) {
                        curr_lex.int_val = 0;;
                    } else if (curr_lex.type == L_BOOL_VAR) {
                        curr_lex.bool_val = false;
                    } else if (curr_lex.type == L_STR_VAR) {
                        curr_lex.str_val = "";
                    }
                }
                if (inTID (curr_lex_str)) {
                    printf ("Error: name %s already defined, line %li\n", curr_lex_str.c_str(), curr_line);
                    exit (EXIT_FAILURE);
                }
                curr_lex.identifier = curr_lex_str;
                TID.push_back (curr_lex);
                if (c != ';') {
                    POLIZ.push_back (curr_lex);
                }
            } else if (curr_lex_str == "false") {
                POLIZ.push_back (Lex(false));
            } else if (curr_lex_str == "true") {
                POLIZ.push_back (Lex(true));
            } else {
                return curr_lex_str;
            }

        } else if (inTID (curr_lex_str)) {
            curr_lex = findInTID (curr_lex_str);
            POLIZ.push_back (curr_lex);
            return curr_lex_str;
        }
	} else {
        printf ("Error: unexpected %c found at line %li\n", c, curr_line);
        exit (EXIT_FAILURE);
    }
	return curr_lex_str;
}

bool Scanner::stateE () {
    skipSpaces ();
    stateE1 ();
    skipSpaces ();
    Lex op;
    op.exec = true;
    switch (c) {
        case '=':
            gc ();
            if (c != '=') {
                printf ("Error: unexpected =%c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            stateE ();
            op.type = L_CMP_EQ;
            POLIZ.push_back (op);
            break;
        case '<':
            gc ();
            stateE ();
            op.type = L_LESS;
            POLIZ.push_back (op);
            break;
        case '>':
            gc ();
            stateE ();
            op.type = L_GRTR;
            POLIZ.push_back (op);
            break;
        case '!':
            gc ();
            if (c != '=') {
                printf ("Error: unexpected !%c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            stateE ();
            op.type = L_NOT_EQ;
            POLIZ.push_back (op);
            break;
    }
}

bool Scanner::stateE1 () {
    skipSpaces ();
    stateT ();
    skipSpaces ();
    Lex op;
    op.exec = true;
    switch (c) {
        case '+':
            gc ();
            stateE1 ();
            op.type = L_ADD;
            POLIZ.push_back (op);
            break;
        case '-':
            gc ();
            stateE1 ();
            op.type = L_SUBTR;
            POLIZ.push_back (op);
            break;
        case '|':
            gc ();
            if (c != '|') {
                printf ("Error: unexpected |%c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            stateE1 ();
            op.type = L_OR;
            POLIZ.push_back (op);
            break;
    }
}

void Scanner::stateT () {
    skipSpaces ();
    stateF ();
    skipSpaces ();
    Lex op;
    op.exec = true;
    switch (c) {
        case '*':
            gc ();
            stateT ();
            op.type = L_MULT;
            POLIZ.push_back (op);
            break;
        case '/':
            gc ();
            stateT ();
            op.type = L_DIV;
            POLIZ.push_back (op);
            break;
        case '&':
            gc ();
            if (c != '&') {
                printf ("Error: unexpected &%c found at line %li\n", c, curr_line);
                exit (EXIT_FAILURE);
            }
            gc ();
            stateT ();
            op.type = L_AND;
            POLIZ.push_back (op);
            break;
    }
}

void Scanner::stateF () {
    skipSpaces ();
    Lex op;
    if (c == '!') {
        gc ();
        stateF ();
        op.type = L_NOT;
        op.exec = true;
        POLIZ.push_back (op);
    } else if (c == '(') {
        gc ();
        stateE ();
        skipSpaces ();
        if (c != ')') {
            printf ("Error: ) expected, %c found at line %li\n", c, curr_line);
            exit (EXIT_FAILURE);
        }
        gc ();
    } else if (std::isdigit (c) || c == '-') {
        int koeff = 1;
        if (c == '-') {
            koeff = -1;
            gc ();
            skipSpaces ();
        }
        int res = 0;
        while (std::isdigit (c)) {
            res = (res * 10) + (c - '0');
            gc ();
        }
        res = res * koeff;
        op = Lex(res);
        POLIZ.push_back (op);
    } else if (std::isalpha (c) || c == '"') {
        bool constval = false;
        if (c == '"') {
            constval = true;
        }
        stateI ();
    } else {
        printf ("Error: unexpected %c at line %li\n", c, curr_line);
        exit (EXIT_FAILURE);
    }
}
