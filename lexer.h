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

class Lexer {
private:
    State currState;
    string lexeme;
    stack<State> states;

    ifstream file;
    istream::streampos p; // Position of last final state, to facilitate rollback step.

    bool eofflag = false;

    // functions
    void reset();
    void rollback();
    string truncate();

public:
    Lexer(string filename){
        file.open(filename);

        if (!file.is_open()){
            cout << "Error opening source file: " << filename << endl;
            throw new InvalidStateException();
        }
    }

    Token* getNextToken();
    bool isEof(){
        return eofflag;
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
void Lexer::reset(){
    currState = START;
    lexeme = "";
    clear(states);
    states.push(BAD);
}

void Lexer::rollback(){
    while (!isFinal(currState) && currState != BAD){
        currState = states.top();
        states.pop();
        lexeme.pop_back();
    }
}

string Lexer::truncate(){
    if (isFinal(currState)) {
        file.seekg(p); // Moving pointer back to where it was good.
        return lexeme;
    } else {
        throw new InvalidStateException();
    }
}

Token* Lexer::getNextToken(){
    if(eofflag){
        throw new EOFException();
    }
    State finalState;

    reset();
    while (!file.eof()){
        if(currState != ERROR) {
            char c = file.get();
            lexeme += c;
            if (isFinal(currState)){
                finalState = currState;
                clear(states);
                p = file.tellg() - (streamoff)1;
            }
            states.push(currState);
            Cat cat = getCat(c);
            currState = TX[cat][currState];
        } else {
            // Rollback loop.
            rollback();
            string lexeme = truncate();

            Type type = findType(lexeme, finalState);
            if(type == SKIP){
                return nullptr;
            }
            return new Token(type, lexeme);
        }
    }

    // Final Rollback loop.
    eofflag = true;
    rollback();
    string lexeme = truncate();

    Type type = findType(lexeme, finalState);
    if(type == SKIP){
        return nullptr;
    }
    return new Token(type, lexeme);
}
#endif //SMALLLANG_LEXER_H
