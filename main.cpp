#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "Visitor/XMLVisitor.h"
#include "Visitor/SemanticAnalyzer.h"

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

//    while(!lexer->isEof()){
    try {
        parseTree = parser->parseProgram();
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
    cout << "Creating XML." << endl;
    XMLVisitor* xmlVisitor = new XMLVisitor(filename + ".xml");
    xmlVisitor->visit(parseTree);

    SemanticAnalyzer* semanticAnalyzer = new SemanticAnalyzer();
    try {
        semanticAnalyzer->visit(parseTree);
    } catch(SemanticErrorException* e){
        cout << e->message() << endl;
    }
    return 0;
}

