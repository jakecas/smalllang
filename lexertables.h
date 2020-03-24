//
// Created by Jake on 19/03/2020.
//

#ifndef SMALLLANG_LEXERTABLE_H
#define SMALLLANG_LEXERTABLE_H

enum State {
    START,
    INTEG,
    UFLOAT, // Unfinished Float
    FLOAT,
    WORD,
    OPER, // Additive and multiplicative operators: + - * /
    LTS, // Less than State
    LES, // Less than or equal to  State
    NES, // Not equal to State
    GTS, // Greater than State
    GES, // Greater than or equal to State
    EQUALS, // Equals Assignment Operator
    EQS, // Equal To Operator
    ERROR,
    BAD// This must always be the last element of the enum.
};

bool isFinal(State s){
    if (s == INTEG || s == FLOAT || s == WORD){
        return true;
    }
    if (s == OPER || s == LTS || s == LES || s == NES || s == GTS || s == GES || s == EQUALS || s == EQS){
        return true;
    }
    return false;
}

enum Cat {
    DIGIT,
    PERIOD,
    CHAR,
    OP, // Additive and multiplicative operators: + - * /
    LT, // Greater than relational operator: <
    GT, // Less than relational operator: <
    EQ, // Equals sign: <
    OTHER // This must always be the last element of the enum.
};

bool isoperator(char c){
    return c == '/' || c == '*' || c == '+' || c == '-';
}

Cat getCat(char c){
    if(isdigit(c)){
        return DIGIT;
    }
    if(c == '.'){
        return PERIOD;
    }
    if(isalpha(c) || c == '_'){
        return CHAR;
    }
    if(isoperator(c)){
        return OP;
    }
    if(c == '<'){
        return LT;
    }
    if(c == '>'){
        return GT;
    }
    if(c == '='){
        return EQ;
    }
    return OTHER;
}

// Using OTHER+1 and BAD for the size since they are the last element of the enum (and BAD has no transitions).
// i.e. To facilitate adding states/classifiers.
State TX[OTHER+1][BAD] = {
        //START,INTEG, UFLOAT,FLOAT, WORD,  OPER,  LTS,   LES,   NES,   GTS,   GES,   EQUALS,EQS,   ERROR
        {INTEG, INTEG, FLOAT, FLOAT, WORD,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // DIGIT
        {ERROR, UFLOAT,ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // PERIOD
        {WORD,  ERROR, ERROR, ERROR, WORD,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // CHAR
        {OPER,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // OP
        {LTS,   ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // LT
        {GTS,   ERROR, ERROR, ERROR, ERROR, ERROR, NES,   ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // GT
        {EQUALS,ERROR, ERROR, ERROR, ERROR, ERROR, LES,   ERROR, ERROR, GES,   ERROR, EQS,   ERROR, ERROR}, // EQ
        {ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR} // OTHER
};

#endif //SMALLLANG_LEXERTABLE_H
