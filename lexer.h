//
// Created by Jake on 19/03/2020.
//

#ifndef SMALLLANG_LEXER_H
#define SMALLLANG_LEXER_H

#include <stack>
#include <string>
#include <fstream>
#include "lexertables.h"

using namespace std;

State currState;
string lexeme;
stack<State> states;
istream::streampos p; // Position of last final state, to facilitate rollback step.

// Thrown if the file could not be opened.
struct InvalidStateException : public std::exception {
    const char *message () const throw () {
        return "Invalid state reached.";
    }
};

template <class T>
void clear(stack<T> st){
    while(!st.empty()){
        st.pop();
    }
}

void reset(){
    currState = START;
    lexeme = "";
    clear(states);
    states.push(ERROR);
}

string getNextToken(ifstream &file){
    reset();
    while (!file.eof()){
        if(currState != ERROR) {
            char c = file.get();
            lexeme += c;
            if (isFinal(currState)){
                clear(states);
                p = file.tellg();
            }
            states.push(currState);
            Cat cat = getCat(c);
            currState = TX[cat][currState];
        } else {
            // Rollback loop.
            while (!isFinal(currState) && currState != BAD){
                currState = states.top();
                states.pop();
                lexeme.pop_back();
            }
            if (isFinal(currState)) {
                file.seekg(p); // Moving pointer back to where it was good.
                return lexeme;
            } else {
                throw new InvalidStateException();
            }
        }
    }
    throw new InvalidStateException();
}
#endif //SMALLLANG_LEXER_H
