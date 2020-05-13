#include <iostream>
#include <fstream>
#include "lexer.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2){
        cout << "No source file specified." << endl;
        return -1;
    }

    Lexer* lexer = new Lexer(argv[1]);

    while(!lexer->isEof()){
        try {
            Token* token = lexer->getNextToken();
            if(token != nullptr) {
                cout << "\"" << token->getLexeme() << " : " << token->getType() << "\"" << endl;
            }
        } catch(EOFException* e){
            cout << "ERROR: Reached EOF." << endl;
            break;
        } catch(InvalidStateException* e){
            cout << "ERROR" << endl;
            break;
        }
    }

    cout << "Reached EOF." << endl;
    return 0;
}

