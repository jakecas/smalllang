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

bool eofflag = false;

// Thrown if the file could not be opened.
struct InvalidStateException : public exception {
    const char *message () const throw () {
        return "Invalid state reached.";
    }
};
// Thrown when file is already read.
struct EOFException : public exception {
    const char *message () const throw () {
        return "EOF reached.";
    }
};

// generic utility functions
template <class T>
void clear(stack<T> st){
    while(!st.empty()){
        st.pop();
    }
}

// lexer specific functions
void reset(){
    currState = START;
    lexeme = "";
    clear(states);
    states.push(BAD);
}

void rollback(){
    while (!isFinal(currState) && currState != BAD){
        currState = states.top();
        states.pop();
        lexeme.pop_back();
    }
}

string truncate(ifstream &file){
    if (isFinal(currState)) {
        file.seekg(p); // Moving pointer back to where it was good.
        while(!eofflag && isspace(file.peek())){
            file.get();
        }
        return lexeme;
    } else {
        throw new InvalidStateException();
    }
}

string getNextToken(ifstream &file){
    if(eofflag){
        throw new EOFException();
    }

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
            rollback();
            return truncate(file);
        }
    }

    // Final Rollback loop.
    eofflag = true;
    rollback();
    return truncate(file);
}
#endif //SMALLLANG_LEXER_H
