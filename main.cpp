#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cstdlib>
#include "Scanner.h"
#include <stdio.h>


using namespace std;
Scanner sc;


void err (std::string type) {
    printf ("Operand type mismatch for %s operator\n", type.c_str());
    exit (EXIT_FAILURE);
}


void execute (vector<Lex> POLIZ) {
    stack<Lex> stck;
    for (int i = 0; i < POLIZ.size(); i++) {
        if (POLIZ[i].type == L_INT_VAR || POLIZ[i].type == L_BOOL_VAR || POLIZ[i].type == L_STR_VAR) {
            stck.push (POLIZ[i]);
        } else {
            Lex op1, op2, res;
            Lex * op;
            int count;

            switch (POLIZ[i].type){
                case L_EQ:

                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.type != op2.type) {
                        err ("=");
                    }
                    if (op1.identifier != "") {
                        op = (sc.lFindInTID (op1.identifier));
                    } else {
                        err ("=");
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op->type == L_INT_VAR) {
                        op->int_val = op2.int_val;
                    } else if (op->type == L_BOOL_VAR) {
                        op->bool_val = op2.bool_val;
                    } else if (op->type == L_STR_VAR) {
                        op->str_val = op2.str_val;
                    } else {
                        err ("=");
                    }
                    break;

                case L_IF:
                    op1 = stck.top();
                    stck.pop();

                    if ((op1.type == L_INT_VAR && op1.int_val == 0) || (op1.type == L_STR_VAR && op1.str_val == "") || (op1.type == L_BOOL_VAR && !op1.bool_val)) {
                        count = 0;
                        i++;
                        while (count != 0 || POLIZ[i].type != L_ENDIF) {
                            if (POLIZ[i].type == L_IF) {
                                count++;
                            }
                            if (POLIZ[i].type == L_ENDIF) {
                                count--;
                            }
                            i++;
                        }
                    }
                    break;

                case L_WGT:
                    count = 1;
                    while (count != 0 || POLIZ[i].type != L_IFBGN) {
                        if (POLIZ[i].type == L_ENDIF) {
                            count++;
                        }
                        if (POLIZ[i].type == L_IF) {
                            count--;
                        }
                        i--;
                    }
                    break;

                case L_INPUT:
                    op1 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op = sc.lFindInTID (op1.identifier);
                    }
                    if (op->type == L_INT_VAR) {
                        cin >> op->int_val;
                    } else if (op->type == L_STR_VAR) {
                        cin >> op->str_val;
                    } else {
                        err ("input");
                    }
                    break;

                case L_PRINT:
                    op1 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op1.type == L_INT_VAR) {
                        printf ("%i\n", op1.int_val);
                    } else if (op1.type == L_STR_VAR) {
                        printf ("%s\n", op1.str_val.c_str());
                    } else {
                        err ("print");
                    }
                    break;

                case L_CMP_EQ:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type == L_INT_VAR && op2.type == L_INT_VAR) {
                        res.bool_val = (op1.int_val == op2.int_val);
                    } else if (op1.type == L_STR_VAR && op2.type == L_STR_VAR) {
                        res.bool_val = (op1.str_val == op2.str_val);
                    } else if (op1.type == L_BOOL_VAR && op2.type == L_BOOL_VAR) {
                        res.bool_val = (op1.bool_val == op2.bool_val);
                    } else {
                        err ("==");
                    }
                    res.type = L_BOOL_VAR;
                    stck.push (res);
                    break;

                case L_LESS:
                    op2 = stck.top();
                    stck.pop();
                    op1 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type == L_INT_VAR && op2.type == L_INT_VAR) {
                        res.bool_val = (op1.int_val < op2.int_val);
                    } else {
                        err ("<");
                    }
                    res.type = L_BOOL_VAR;
                    stck.push (res);
                    break;

                case L_GRTR:
                    op2 = stck.top();
                    stck.pop();
                    op1 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type == L_INT_VAR && op2.type == L_INT_VAR) {
                        res.bool_val = (op1.int_val > op2.int_val);
                    } else {
                        err (">");
                    }
                    res.type = L_BOOL_VAR;
                    stck.push (res);
                    break;

                case L_NOT_EQ:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type == L_INT_VAR && op2.type == L_INT_VAR) {
                        res.bool_val = !(op1.int_val == op2.int_val);
                    } else if (op1.type == L_STR_VAR && op2.type == L_STR_VAR) {
                        res.bool_val = !(op1.str_val == op2.str_val);
                    } else if (op1.type == L_BOOL_VAR && op2.type == L_BOOL_VAR) {
                        res.bool_val = !(op1.bool_val == op2.bool_val);
                    } else {
                        err ("!=");
                    }
                    res.type = L_BOOL_VAR;
                    stck.push (res);
                    break;

                case L_ADD:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type == L_INT_VAR && op2.type == L_INT_VAR) {
                        res.int_val = op1.int_val + op2.int_val;
                        res.type = L_INT_VAR;
                    } else if (op1.type == L_STR_VAR && op2.type == L_STR_VAR) {
                        res.str_val = op2.str_val + op1.str_val;
                        res.type = L_STR_VAR;
                    } else {
                        err ("+");
                    }
                    stck.push (res);
                    break;

                case L_SUBTR:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type == L_INT_VAR && op2.type == L_INT_VAR) {
                        res.int_val = op2.int_val - op1.int_val;
                    } else {
                        err ("-");
                    }
                    res.type = L_INT_VAR;
                    stck.push (res);
                    break;

                case L_OR:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type != op2.type) {
                        err("||");
                    }
                    if (op1.type == L_INT_VAR) {
                        res.bool_val = op1.int_val || op2.int_val;
                    } else if (op1.type == L_BOOL_VAR) {
                        res.bool_val = op1.bool_val || op2.bool_val;
                    } else {
                        err("||");
                    }
                    res.type = L_BOOL_VAR;
                    stck.push (res);
                    break;


                case L_MULT:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type == L_INT_VAR && op2.type == L_INT_VAR) {
                        res.int_val = op1.int_val * op2.int_val;
                    } else {
                        err ("*");
                    }
                    res.type = L_INT_VAR;
                    stck.push (res);
                    break;

                case L_DIV:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type != op2.type || op1.type != L_INT_VAR) {
                        err("/");
                    }
                    res.int_val = op1.int_val / op2.int_val;
                    res.type = L_INT_VAR;
                    stck.push (res);
                    break;

                case L_AND:
                    op1 = stck.top();
                    stck.pop();
                    op2 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op2.identifier != "") {
                        op2 = sc.findInTID (op2.identifier);
                    }
                    if (op1.type != op2.type) {
                        err("&&");
                    }
                    if (op1.type == L_INT_VAR) {
                        res.int_val = op1.int_val && op2.int_val;
                    } else if (op1.type == L_BOOL_VAR) {
                        res.bool_val = op1.bool_val && op2.bool_val;
                    } else {
                        err("&&");
                    }
                    res.type = L_BOOL_VAR;
                    stck.push (res);
                    break;

                case L_NOT:
                    op1 = stck.top();
                    stck.pop();
                    if (op1.identifier != "") {
                        op1 = sc.findInTID (op1.identifier);
                    }
                    if (op1.type = L_INT_VAR) {
                        res.int_val = !op1.int_val;
                        res.type = L_INT_VAR;
                    } else if (op1.type = L_BOOL_VAR) {
                        res.bool_val = !op1.bool_val;
                        res.type = L_BOOL_VAR;
                    } else {
                        printf ("Operand type mismatch for ! operator\n");
                        exit (EXIT_FAILURE);
                    }
                    stck.push (res);
                    break;
            }


        }
    }
}


int main (int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Wrong args number\n";
        return -1;
    }
    sc = Scanner (argv[1]);

    execute (sc.Check ());


    return 0;
}
