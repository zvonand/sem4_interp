#ifndef LEX_TYPE_H
#define LEX_TYPE_H
#endif

enum lex_type {
    L_IF,               // if
    L_WGT,              //goto while cycle begin
    L_IFBGN,
    L_ENDIF,
    L_INPUT,            //input () function
    L_PRINT,            //print () function
    L_INT_VAR,          //int variable
    L_BOOL_VAR,         //bool variable
    L_STR_VAR,          //string variable
    L_TRUE_VAL,         //boolean true value
    L_FALSE_VAL,        //boolean false value
    L_CMP_EQ,           // ==
    L_EQ,               // =
    L_LESS,             // <
    L_GRTR,             // >
    L_NOT_EQ,           // !=
    L_ADD,              // +
    L_SUBTR,            // -
    L_OR,               // ||
    L_MULT,             // *
    L_DIV,              // /
    L_AND,              // &&
    L_NOT               // !
};
