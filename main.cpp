#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2){
        cout << "No source file specified." << endl;
        return -1;
    }

    Lexer* lexer = new Lexer(argv[1]);
    Parser* parser = new Parser(lexer);

//    while(!lexer->isEof()){
    try {
        parser->parseProgram();
    } catch(EOFException* e){
        cout << "ERROR: Reached EOF." << endl;
//        break;
    } catch(InvalidStateException* e){
        cout << "ERROR" << endl;
//        break;
    } catch(SyntaxErrorException* e){
        cout << e->message() << endl;
    }
//    }

    cout << "Reached EOF." << endl;
    return 0;
}

