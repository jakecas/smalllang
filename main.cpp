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

    string filename = argv[1];

    Lexer* lexer = new Lexer(filename);
    Parser* parser = new Parser(lexer);
    ASTProgram* parseTree;

    try {
        parseTree = parser->parseProgram();
    } catch(EOFException* e){
        cout << "Error: Reached EOF while parsing." << endl;
        return -1;
    } catch(InvalidStateException* e){
        cout << "Error: Invalid state reached while parsing:\n" << e->message() << endl;
        return -1;
    } catch(SyntaxErrorException* e){
        cout <<  "Error: Syntax error found:\n" << e->message() << endl;
        return -1;
    }

    return 0;
}

