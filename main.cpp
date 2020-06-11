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

    try {
        cout << "Parsing... ";
        parseTree = parser->parseProgram();
        cout << "Reached EOF." << endl;
    } catch(EOFException* e){
        cout << "ERROR: Reached EOF." << endl;
    } catch(InvalidStateException* e){
        cout << "ERROR" << endl;
    } catch(SyntaxErrorException* e){
        cout << e->message() << endl;
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
        cout << e->message() << endl;
    }
    return 0;
}

