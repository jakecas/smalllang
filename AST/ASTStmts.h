//
// Created by Jake on 19/05/2020.
//

#ifndef SMALLLANG_ASTSTMTS_H
#define SMALLLANG_ASTSTMTS_H

#include <vector>

#include "ASTNode.h"
#include "ASTExprs.h"

class ASTStmt : public ASTNode {
public:
    ASTStmt(){};

    virtual void accept(Visitor* visitor) = 0;
};

class ASTBlock : public ASTStmt {
    vector<ASTStmt*> stmts;
public:
    ASTBlock(vector<ASTStmt*> stmts){
        this->stmts = stmts;
    }

    vector<ASTStmt*> getStmts(){
        return stmts;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTAssignment : public ASTStmt {
private:
    ASTId* id;
    ASTExpr* expr;
public:
    ASTAssignment(ASTId* id, ASTExpr* expr){
        this->id = id;
        this->expr = expr;
    }

    ASTId* getId(){
        return id;
    }
    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTVarDecl : public ASTStmt {
private:
    ASTId* id;
    ASTType* type;
    ASTExpr* expr;
public:
    ASTVarDecl(ASTId* id, ASTType* type, ASTExpr* expr){
        this->id = id;
        this->type = type;
        this->expr = expr;
    }

    ASTId* getId(){
        return id;
    }
    ASTType* getType(){
        return type;
    }
    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }

};

class ASTPrint : public ASTStmt {
private:
    ASTExpr* expr;
public:
    ASTPrint(ASTExpr* expr){
        this->expr = expr;
    }

    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTRtrn : public ASTStmt {
private:
    ASTExpr* expr;
public:
    ASTRtrn(ASTExpr* expr){
        this->expr = expr;
    }

    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTIfStmt : public ASTStmt {
private:
    ASTExpr* expr;
    ASTBlock* trueBlock;
    ASTBlock* falseBlock;
public:
    ASTIfStmt(ASTExpr* expr, ASTBlock* trueBlock){
        this->expr = expr;
        this->trueBlock = trueBlock;
        this->falseBlock = nullptr;
    }
    ASTIfStmt(ASTExpr* expr, ASTBlock* trueBlock, ASTBlock* falseBlock){
        this->expr = expr;
        this->trueBlock = trueBlock;
        this->falseBlock = falseBlock;
    }

    ASTExpr* getExpr(){
        return expr;
    }
    ASTBlock* getTrueBlock(){
        return trueBlock;
    }
    ASTBlock* getFalseBlock(){
        return falseBlock;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTForStmt : public ASTStmt {
private:
    ASTVarDecl* varDecl;
    ASTExpr* expr;
    ASTAssignment* assignment;
    ASTBlock* block;
public:
    ASTForStmt(ASTExpr* expr, ASTBlock* block){
        this->varDecl = nullptr;
        this->expr = expr;
        this->assignment = nullptr;
        this->block = block;
    }
    ASTForStmt(ASTVarDecl* varDecl, ASTExpr* expr, ASTBlock* block){
        this->varDecl = varDecl;
        this->expr = expr;
        this->assignment = nullptr;
        this->block = block;
    }
    ASTForStmt(ASTExpr* expr, ASTAssignment* assignment, ASTBlock* block){
        this->varDecl = nullptr;
        this->expr = expr;
        this->assignment = assignment;
        this->block = block;
    }
    ASTForStmt(ASTVarDecl* varDecl, ASTExpr* expr, ASTAssignment* assignment, ASTBlock* block){
        this->varDecl = varDecl;
        this->expr = expr;
        this->assignment = assignment;
        this->block = block;
    }

    ASTVarDecl* getVarDecl(){
        return varDecl;
    }
    ASTExpr* getExpr(){
        return expr;
    }
    ASTAssignment* getAssignment(){
        return assignment;
    }
    ASTBlock* getBlock(){
        return block;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTWhileStmt : public ASTStmt {
private:
    ASTExpr* expr;
    ASTBlock* block;
public:
    ASTWhileStmt(ASTExpr* expr, ASTBlock* block){
        this->expr = expr;
        this->block = block;
    }

    ASTExpr* getExpr(){
        return expr;
    }
    ASTBlock* getBlock(){
        return block;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTFormalParam : public ASTNode {
private:
    ASTId* id;
    ASTType* type;
public:
    ASTFormalParam(ASTId* id, ASTType* type){
        this->id = id;
        this->type = type;
    }

    ASTId* getId(){
        return id;
    }
    ASTType* getType(){
        return type;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTFuncDecl : public ASTStmt {
private:
    ASTId* id;
    vector<ASTFormalParam*> formalParams;
    ASTType* type;
    ASTBlock* block;
public:
    ASTFuncDecl(ASTId* id, ASTType* type, ASTBlock* block){
        this->id = id;
        this->type = type;
        this->block = block;
    }
    ASTFuncDecl(ASTId* id, vector<ASTFormalParam*> formalParams, ASTType* type, ASTBlock* block){
        this->id = id;
        this->formalParams = formalParams;
        this->type = type;
        this->formalParams = formalParams;
    }

    ASTId* getId(){
        return id;
    }
    vector<ASTFormalParam*> getFormalParams(){
        return formalParams;
    }
    ASTType* getType(){
        return type;
    }
    ASTBlock* getBlock(){
        return block;
    }
    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

#endif //SMALLLANG_ASTSTMTS_H
