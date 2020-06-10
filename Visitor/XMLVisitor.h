//
// Created by Jake on 20/05/2020.
//

#ifndef SMALLLANG_XMLVISITOR_H
#define SMALLLANG_XMLVISITOR_H

#include <string>
#include <fstream>

#include "Visitor.h"
#include "../AST/ASTProgram.h"

using namespace std;

class XMLVisitor : public Visitor {
private:
    string filename;
    ofstream file;
    int indent;

    void incIndent(){
        indent++;
    }
    void decIndent(){
        indent--;
    }
    string getIndent(){
        string t = "";
        for(int i = 0; i < indent; i++){
            t += "\t";
        }
        return t;
    }

    void visit(ASTType* type);
    void visit(ASTBoolLit* boolLit);
    void visit(ASTIntLit* intLit);
    void visit(ASTFloatLit* floatLit);

    void visit(ASTId* id);
    void visit(ASTActualParam* actualParam);
    void visit(ASTFuncCall* funcCall);
    void visit(ASTSubExpr* subExpr);
    void visit(ASTUnaryOp* unaryOp);
    void visit(ASTTerm* term);
    void visit(ASTSimpleExpr* simpleExpr);
    void visit(ASTExpr* expr);

    void visit(ASTAssignment* assignment);
    void visit(ASTVarDecl* varDecl);
    void visit(ASTPrint* print);
    void visit(ASTRtrn* rtrn);
    void visit(ASTIfStmt* ifStmt);
    void visit(ASTForStmt* forStmt);
    void visit(ASTWhileStmt* whileStmt);
    void visit(ASTFormalParam* formalParam);
    void visit(ASTFuncDecl* funcDecl);
    void visit(ASTBlock* block);

public:
    XMLVisitor(string filename){
        this->filename = filename;
        file.open(filename);
        indent = 0;
    }
    ~XMLVisitor(){
        file.close();
    }

    void visit(ASTProgram* program);
};

void XMLVisitor::visit(ASTType* type){
    file << getIndent() << "<Type>";
    switch(type->getDatatype()){
        case FLOATTYPE:
            file << "float";
            break;
        case INTTYPE:
            file << "int";
            break;
        case BOOLTYPE:
            file << "bool";
            break;
        case AUTOTYPE:
            file << "auto";
            break;
    }
    file << "</Type>" << endl;
}
void XMLVisitor::visit(ASTBoolLit* boolLit){
    file << getIndent() << "<Bool>";
    if(boolLit->getVal()){
        file << "true";
    } else {
        file << "false";
    }
    file <<"</Bool>" <<  endl;
}
void XMLVisitor::visit(ASTIntLit* intLit){
    file << getIndent() << "<Int>" << intLit->getVal() <<"</Int>" <<  endl;
}
void XMLVisitor::visit(ASTFloatLit* floatLit){
    file << getIndent() << "<Float>" << floatLit->getVal() <<"</Float>" <<  endl;
}

void XMLVisitor::visit(ASTId* id){
    file << getIndent() << "<Id>" << id->getId() <<"</Id>" <<  endl;
}
void XMLVisitor::visit(ASTActualParam* actualParam){
    file << getIndent()  << "<ActualParam>" << endl;
    incIndent();
    actualParam->getExpr()->accept(this);
    decIndent();
    file << getIndent()  << "</ActualParam>" << endl;
}
void XMLVisitor::visit(ASTFuncCall* funcCall){
    file << getIndent() << "<FuncCall>" << endl;
    incIndent();
    funcCall->getId()->accept(this);

    vector<ASTActualParam*> params = funcCall->getActualParams();
    file << getIndent()  << "<ActualParams>" << endl;
    incIndent();
    for(unsigned int i = 0; i < params.size(); i++){
        params[i]->accept(this);
    }
    decIndent();
    file << getIndent()  << "</ActualParams>" << endl;

    decIndent();
    file << getIndent() <<"</FuncCall>" << endl;
}
void XMLVisitor::visit(ASTSubExpr* subExpr){
    file << getIndent() << "<SubExpr>" << endl;
    incIndent();
    subExpr->getExpr()->accept(this);
    decIndent();
    file << getIndent() << "</SubExpr>" << endl;
}
void XMLVisitor::visit(ASTUnaryOp* unaryOp){
    file << getIndent() << "<UnaryOp>" << endl;
    incIndent();
    file << getIndent() << "<Type>";
    if(unaryOp->getUnaryOp() == MINUS){
        file << "\"-\"";
    } else {
        file << "\"not\"";
    }
    file << "</Type>" << endl;
    unaryOp->getExpr()->accept(this);
    decIndent();
    file << getIndent() << "</UnaryOp>" << endl;
}
void XMLVisitor::visit(ASTTerm* term){
    if(!term->getTerm()){
        term->getFactor()->accept(this);
        return;
    }
    file << getIndent() << "<Term>" << endl;
    incIndent();
    term->getFactor()->accept(this);
    file << getIndent() << "<MultOp>";
    switch(term->getMultOp()){
        case MULTOP:
            file << "\"*\"";
            break;
        case DIVOP:
            file << "\"/\"";
            break;
        case ANDOP:
            file << "\"and\"";
            break;
    }
    file << "</MultOp>" << endl;
    term->getTerm()->accept(this);

    decIndent();
    file << getIndent() << "</Term>" << endl;}
void XMLVisitor::visit(ASTSimpleExpr* simpleExpr){
    if(!simpleExpr->getSimpleExpr()){
        simpleExpr->getTerm()->accept(this);
        return;
    }
    file << getIndent() << "<SimpleExpr>" << endl;
    incIndent();
    simpleExpr->getTerm()->accept(this);
    file << getIndent() << "<AddOp>";
    switch(simpleExpr->getAddOp()){
        case ADD:
            file << "\"+\"";
            break;
        case SUB:
            file << "\"-\"";
            break;
        case OR:
            file << "\"or\"";
            break;
    }
    file << "</AddOp>" << endl;
    simpleExpr->getSimpleExpr()->accept(this);

    decIndent();
    file << getIndent() << "</SimpleExpr>" << endl;
}
void XMLVisitor::visit(ASTExpr* expr){
    if(!expr->getExpr()){
        expr->getSimpleExpr()->accept(this);
        return;
    }
    file << getIndent() << "<Expr>" << endl;
    incIndent();
    expr->getSimpleExpr()->accept(this);
    file << getIndent() << "<RelOp>";
    switch(expr->getRelOp()){
        case LTOP:
            file << "\"<\"";
            break;
        case LEOP:
            file << "\"<=\"";
            break;
        case NEOP:
            file << "\"<>\"";
            break;
        case GTOP:
            file << "\">\"";
            break;
        case GEOP:
            file << "\">=\"";
            break;
        case EQOP:
            file << "\"==\"";
            break;
    }
    file << "</RelOp>" << endl;
    expr->getExpr()->accept(this);

    decIndent();
    file << getIndent() << "</Expr>" << endl;
}

void XMLVisitor::visit(ASTAssignment* assignment){
    file << getIndent()  << "<VarAssign>" << endl;
    incIndent();
    assignment->getId()->accept(this);
    assignment->getExpr()->accept(this);
    decIndent();
    file << getIndent()  << "</VarAssign>" << endl;
}
void XMLVisitor::visit(ASTVarDecl* varDecl){
    file << getIndent()  << "<VarDecl>" << endl;
    incIndent();
    varDecl->getId()->accept(this);
    varDecl->getType()->accept(this);
    varDecl->getExpr()->accept(this);
    decIndent();
    file << getIndent()  << "</VarDecl>" << endl;
}
void XMLVisitor::visit(ASTPrint* print){
    file << getIndent()  << "<Print>" << endl;
    incIndent();
    print->getExpr()->accept(this);
    decIndent();
    file << getIndent()  << "</Print>" << endl;}
void XMLVisitor::visit(ASTRtrn* rtrn){
    file << getIndent()  << "<Return>" << endl;
    incIndent();
    rtrn->getExpr()->accept(this);
    decIndent();
    file << getIndent()  << "</Return>" << endl;
}
void XMLVisitor::visit(ASTIfStmt* ifStmt){
    file << getIndent()  << "<If>" << endl;
    incIndent();
    ifStmt->getExpr()->accept(this);
    file << getIndent()  << "<TrueBlock>" << endl;
    incIndent();
    ifStmt->getTrueBlock()->accept(this);
    decIndent();
    file << getIndent()  << "</TrueBlock>" << endl;
    if(ifStmt->getFalseBlock()){
        file << getIndent()  << "<FalseBlock>" << endl;
        incIndent();
        ifStmt->getFalseBlock()->accept(this);
        decIndent();
        file << getIndent()  << "</FalseBlock>" << endl;
    }
    decIndent();
    file << getIndent()  << "</If>" << endl;
}
void XMLVisitor::visit(ASTForStmt* forStmt){
    file << getIndent()  << "<For>" << endl;
    incIndent();
    if(forStmt->getVarDecl()) {
        forStmt->getVarDecl()->accept(this);
    }
    forStmt->getExpr()->accept(this);
    if(forStmt->getAssignment()) {
        forStmt->getAssignment()->accept(this);
    }
    forStmt->getBlock()->accept(this);
    decIndent();
    file << getIndent()  << "</For>" << endl;
}
void XMLVisitor::visit(ASTWhileStmt* whileStmt){
    file << getIndent()  << "<While>" << endl;
    incIndent();
    whileStmt->getExpr()->accept(this);
    whileStmt->getBlock()->accept(this);
    decIndent();
    file << getIndent()  << "</While>" << endl;
}
void XMLVisitor::visit(ASTFormalParam* formalParam){
    file << getIndent()  << "<FormalParam>" << endl;
    incIndent();
    formalParam->getId()->accept(this);
    formalParam->getType()->accept(this);
    decIndent();
    file << getIndent()  << "</FormalParam>" << endl;
}
void XMLVisitor::visit(ASTFuncDecl* funcDecl){
    file << getIndent() << "<FuncDecl>" << endl;
    incIndent();
    funcDecl->getId()->accept(this);
    funcDecl->getType()->accept(this);

    vector<ASTFormalParam*> params = funcDecl->getFormalParams();
    file << getIndent()  << "<FormalParams>" << endl;
    incIndent();
    for(unsigned int i = 0; i < params.size(); i++){
        params[i]->accept(this);
    }
    decIndent();
    file << getIndent()  << "</FormalParams>" << endl;

    funcDecl->getBlock()->accept(this);

    decIndent();
    file << getIndent() <<"</FuncDecl>" << endl;
}
void XMLVisitor::visit(ASTBlock* block){
    vector<ASTStmt*> stmts = block->getStmts();

    file << getIndent() << "<Block>" << endl;
    incIndent();
    for(unsigned int i = 0; i < stmts.size(); i++){
        stmts[i]->accept(this);
    }
    decIndent();
    file << getIndent() << "</Block>" << endl;
}

void XMLVisitor::visit(ASTProgram* program){
    vector<ASTStmt*> stmts = program->getStmts();

    file << "<Program>" << endl;
    incIndent();
    for(unsigned int i = 0; i < stmts.size(); i++){
        stmts[i]->accept(this);
    }
    decIndent();
    file << "</Program>" << endl;
}

#endif //SMALLLANG_XMLVISITOR_H
