//
// Created by Jake on 19/03/2020.
//

#ifndef SMALLLANG_LEXERTABLE_H
#define SMALLLANG_LEXERTABLE_H

enum State {
    START,
    INTEGER,
    UFLOAT, // Unfinished Float
    FLOAT,
    ERROR,
    BAD// This must always be the last element of the enum.
};

bool isFinal(State s){
    if (s == INTEGER || s == FLOAT){
        return true;
    }
    return false;
}

enum Cat {
    DIGIT,
    PERIOD,
    OTHER // This must always be the last element of the enum.
};

Cat getCat(char c){
    if(isdigit(c)){
        return DIGIT;
    }
    if(c == '.'){
        return PERIOD;
    }
    return OTHER;
}

// Using OTHER+1 and BAD for the size since they are the last element of the enum (and BAD has no transitions).
// i.e. To facilitate adding states/classifiers.
State TX[OTHER+1][BAD] = {
        // START, INTEGER, UFLOAT, FLOAT, ERROR
        {INTEGER, INTEGER, FLOAT, FLOAT, ERROR}, // DIGIT
        {ERROR, UFLOAT, ERROR, ERROR, ERROR}, // PERIOD
        {ERROR, ERROR, ERROR, ERROR, ERROR} // OTHER
};

#endif //SMALLLANG_LEXERTABLE_H
