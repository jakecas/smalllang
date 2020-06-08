//
// Created by Jake on 20/05/2020.
//

#ifndef SMALLLANG_XMLVISITOR_H
#define SMALLLANG_XMLVISITOR_H

#include "Visitor.h"
#include "../AST/ASTStmts.h"

class XMLVisitor : public Visitor {
private:
public:
    void visit(ASTType* type);
    void visit(ASTBoolLit* boolLit);
    void visit(ASTIntLit* intLit);
    void visit(ASTFloatLit* floatLit);
    void visit(ASTId* id);
    void visit(ASTActualParam* actualParam);
    void visit(ASTFuncCall* funcCall);
    void visit(ASTSubExpr* subExpr);
    void visit(ASTUnaryOp* unaryOp);
    void visit(ASTFactor* factor);
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
    void visit(ASTStmt* stmt);
    void visit(ASTBlock* block);
};

#endif //SMALLLANG_XMLVISITOR_H
