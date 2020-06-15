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
        cout << "Parsing... ";
        parseTree = parser->parseProgram();
        cout << "Reached EOF." << endl;
    } catch(EOFException* e){
        cout << "\nError: Reached EOF while parsing." << endl;
        return -1;
    } catch(InvalidStateException* e){
        cout << "\nError: Invalid state reached while parsing:\n" << e->message() << endl;
        return -1;
    } catch(SyntaxErrorException* e){
        cout <<  "\nError: Syntax error found:\n" << e->message() << endl;
        return -1;
    }

    cout << "Creating XML... ";
    XMLVisitor* xmlVisitor = new XMLVisitor(filename + ".xml");
    xmlVisitor->visit(parseTree);
    cout << "Complete." << endl;

    cout << "Performing semantic analysis... ";
    SemanticAnalyzer* semanticAnalyzer = new SemanticAnalyzer();
    try {
        semanticAnalyzer->visit(parseTree);
        cout << "Complete." << endl;
    } catch(SemanticErrorException* e){
        cout << "\nError: Semantic error found:\n" << e->message() << endl;
        return -1;
    }

    Executor* executor = new Executor();
    cout << "Running the program..." << endl;
    executor->visit(parseTree);
    cout << "\nDone." << endl;

    return 0;
}

