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
};

class ASTBlock : public ASTStmt {
    vector<ASTStmt*> stmts;
public:
    ASTBlock(vector<ASTStmt*> stmts){
        this->stmts = stmts;
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
};

class ASTVarDecl : public ASTStmt {
private:
    ASTId* idnode;
    ASTType* type;
    ASTExpr* expr;
public:
    ASTVarDecl(ASTId* idnode, ASTType* type, ASTExpr* expr){
        this->idnode = idnode;
        this->type = type;
        this->expr = expr;
    }

};

class ASTPrint : public ASTStmt {
private:
    ASTExpr* expr;
public:
    ASTPrint(ASTExpr* expr){
        this->expr = expr;
    }
};

class ASTRtrn : public ASTStmt {
private:
    ASTExpr* expr;
public:
    ASTRtrn(ASTExpr* expr){
        this->expr = expr;
    }
};

class ASTIfStmt : public ASTStmt {
private:
    // in "if X then Y", X is the antecedent
    ASTExpr* antecedent;
    ASTBlock* trueBlock;
    ASTBlock* falseBlock;
public:
    ASTIfStmt(ASTExpr* antecedent, ASTBlock* trueBlock){
        this->antecedent = antecedent;
        this->trueBlock = trueBlock;
        this->falseBlock = nullptr;
    }
    ASTIfStmt(ASTExpr* antecedent, ASTBlock* trueBlock, ASTBlock* falseBlock){
        this->antecedent = antecedent;
        this->trueBlock = trueBlock;
        this->falseBlock = falseBlock;
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
};

#endif //SMALLLANG_ASTSTMTS_H
