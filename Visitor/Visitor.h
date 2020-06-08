//
// Created by Jake on 20/05/2020.
//

#ifndef SMALLLANG_VISITOR_H
#define SMALLLANG_VISITOR_H

class Visitor {
public:
    virtual void visit(ASTType* type) = 0;
    virtual void visit(ASTBoolLit* boolLit) = 0;
    virtual void visit(ASTIntLit* intLit) = 0;
    virtual void visit(ASTFloatLit* floatLit) = 0;
    virtual void visit(ASTId* id) = 0;
    virtual void visit(ASTActualParam* actualParam) = 0;
    virtual void visit(ASTFuncCall* funcCall) = 0;
    virtual void visit(ASTSubExpr* subExpr) = 0;
    virtual void visit(ASTUnaryOp* unaryOp) = 0;
    virtual void visit(ASTFactor* factor) = 0;
    virtual void visit(ASTTerm* term) = 0;
    virtual void visit(ASTSimpleExpr* simpleExpr) = 0;
    virtual void visit(ASTExpr* expr) = 0;
    virtual void visit(ASTAssignment* assignment) = 0;
    virtual void visit(ASTVarDecl* varDecl) = 0;
    virtual void visit(ASTPrint* print) = 0;
    virtual void visit(ASTRtrn* rtrn) = 0;
    virtual void visit(ASTIfStmt* ifStmt) = 0;
    virtual void visit(ASTForStmt* forStmt) = 0;
    virtual void visit(ASTWhileStmt* whileStmt) = 0;
    virtual void visit(ASTFormalParam* formalParam) = 0;
    virtual void visit(ASTFuncDecl* funcDecl) = 0;
    virtual void visit(ASTStmt* stmt) = 0;
    virtual void visit(ASTBlock* block) = 0;
};

#endif //SMALLLANG_VISITOR_H
