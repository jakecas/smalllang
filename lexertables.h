//
// Created by Jake on 19/03/2020.
//

#ifndef SMALLLANG_LEXERTABLE_H
#define SMALLLANG_LEXERTABLE_H

enum State {
    START,
    DELIM, // Whitespace
    UCMT, // Comments
    CMT, // Comments
    UMCMT, // Open ended multi-line comments
    AMCMT, // Almost finished multi-line comment (meaning found a *)
    MCMT, // Multi-line Comment
    INTEG,
    UFLOAT, // Unfinished Float
    FLOAT,
    WORD,
    ADDS, // Additive operators state: + -
    DIVS, // Division operator state: /
    MULTS, // Multiplicative operators state: *
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
    if(s == CMT || s == MCMT){
        return true;
    }
    if (s == DELIM){
        return true;
    }
    if (s == INTEG || s == FLOAT || s == WORD){
        return true;
    }
    if (s == ADDS || s == DIVS || s == MULTS || s == LTS || s == LES || s == NES || s == GTS || s == GES || s == EQUALS || s == EQS){
        return true;
    }
    return false;
}

enum Cat {
    NEWLINE,
    WS, // Any other whitespace
    DIGIT,
    PERIOD,
    CHAR,
    ADDOP, // Additive operators: + -
    DIV,
    MULT,
    LT, // Greater than relational operator: <
    GT, // Less than relational operator: <
    EQ, // Equals sign: <
    OTHER // This must always be the last element of the enum.
};

Cat getCat(char c){
    if(c == '\n'){
        return NEWLINE;
    }
    if(isspace(c)){
        return WS;
    }
    if(isdigit(c)){
        return DIGIT;
    }
    if(c == '.'){
        return PERIOD;
    }
    if(isalpha(c) || c == '_'){
        return CHAR;
    }
    if(c == '+' || c == '-'){
        return ADDOP;
    }
    if(c == '/'){
        return DIV;
    }
    if(c == '*'){
        return MULT;
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
        //START,DELIM, UCMT,  CMT,   UMCMT, AMCMT, MCMT,  INTEG, UFLOAT,FLOAT, WORD,  ADDS,  DIVS,  MULTS, LTS,   LES,   NES,   GTS,   GES,   EQUALS,EQS,   ERROR
        {DELIM, DELIM, CMT,   ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // NEWLINE
        {DELIM, DELIM, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // WS
        {INTEG, ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, INTEG, FLOAT, FLOAT, WORD,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // DIGIT
        {ERROR, ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, UFLOAT,ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // PERIOD
        {WORD,  ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, WORD,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // CHAR
        {ADDS,  ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // ADDOP
        {DIVS,  ERROR, UCMT,  ERROR, UMCMT, MCMT,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCMT,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // DIV
        {MULTS, ERROR, UCMT,  ERROR, AMCMT, AMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // MULT
        {LTS,   ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // LT
        {GTS,   ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, NES,   ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}, // GT
        {EQUALS,ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, LES,   ERROR, ERROR, GES,   ERROR, EQS,   ERROR, ERROR}, // EQ
        {ERROR, ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR} // OTHER
};

#endif //SMALLLANG_LEXERTABLE_H
