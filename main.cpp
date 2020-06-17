#include <iostream>
#include <fstream>

#include "lexer.h"
#include "parser.h"
#include "Visitor/XMLVisitor.h"
#include "Visitor/SemanticAnalyzer.h"
#include "Visitor/Executor.h"

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

    XMLVisitor* xmlVisitor = new XMLVisitor(filename + ".xml");
    xmlVisitor->visit(parseTree);

    SemanticAnalyzer* semanticAnalyzer = new SemanticAnalyzer();
    try {
        semanticAnalyzer->visit(parseTree);
    } catch(SemanticErrorException* e){
        cout << "Error: Semantic error found:\n" << e->message() << endl;
        return -1;
    }

    Executor* executor = new Executor();
    executor->visit(parseTree);

    return 0;
}

