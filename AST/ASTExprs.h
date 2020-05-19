//
// Created by Jake on 19/05/2020.
//

#ifndef SMALLLANG_ASTEXPRS_H
#define SMALLLANG_ASTEXPRS_H

#include <vector>

#include "ASTNode.h"
#include "ASTPrimitives.h"

class ASTFactor : public ASTNode {
public:
    ASTFactor(){};
};

class ASTTerm : public ASTNode  {
    ASTFactor* factor;
    MultOp multOp;
    ASTTerm* term;
public:
    ASTTerm(ASTFactor* factor){
        this->factor = factor;
        this->term = nullptr;
    }
    ASTTerm(ASTFactor* factor, MultOp multOp, ASTTerm* term){
        this->factor = factor;
        this->multOp = multOp;
        this->term = term;
    }
};

class ASTSimpleExpr : public ASTNode {
    ASTTerm* term;
    AddOp addOp;
    ASTSimpleExpr* simpleExpr;
public:
    ASTSimpleExpr(ASTTerm* term){
        this->term = term;
        this->simpleExpr = nullptr;
    }
    ASTSimpleExpr(ASTTerm* term, AddOp addOp, ASTSimpleExpr* simpleExpr){
        this->term = term;
        this->addOp = addOp;
        this->simpleExpr = simpleExpr;
    }
};

class ASTExpr : public ASTNode {
    ASTSimpleExpr* simpleExpr;
    RelOp relOp;
    ASTExpr* expr;
public:
    ASTExpr(ASTSimpleExpr* simpleExpr){
        this->simpleExpr = simpleExpr;
        this->expr = nullptr;
    }
    ASTExpr(ASTSimpleExpr* simpleExpr, RelOp relOp, ASTExpr* expr){
        this->simpleExpr = simpleExpr;
        this->relOp = relOp;
        this->expr = expr;
    }
};

class ASTId : public ASTFactor {
private:
    string id;
public:
    ASTId(string id){
        this->id = id;
    }
    string getId(){
        return id;
    }
};

class ASTActualParam : public ASTNode {
private:
    ASTExpr* expr;
public:
    ASTActualParam(ASTExpr* expr){
        this->expr = expr;
    }
};

class ASTFuncCall : public ASTFactor {
private:
    ASTId* id;
    vector<ASTActualParam*> actualParams;
public:
    ASTFuncCall(ASTId* id){
        this->id = id;
    }
    ASTFuncCall(ASTId* id, vector<ASTActualParam*> actualParams){
        this->id = id;
        this->actualParams = actualParams;
    }
};

class ASTSubExpr : public ASTFactor {
private:
    ASTExpr* expr;
public:
    ASTSubExpr(ASTExpr* expr){
        this->expr = expr;
    }
};

class ASTUnaryOp : public ASTFactor{
private:
    UnaryOp unaryOp;
    ASTExpr* expr;
public:
    ASTUnaryOp(UnaryOp unaryOp, ASTExpr* expr){
        this->unaryOp = unaryOp;
        this->expr = expr;
    }
};

class ASTLiteral : public ASTFactor {
public:
    ASTLiteral(){}
};

class ASTBoolLit: public ASTLiteral {
private:
    bool val;
public:
    ASTBoolLit(bool val){
        this->val = val;
    }
};

class ASTIntLit: public ASTLiteral {
private:
    int val;
public:
    ASTIntLit(int val){
        this->val = val;
    }
};

class ASTFloatLit: public ASTLiteral {
private:
    float val;
public:
    ASTFloatLit(float val){
        this->val = val;
    }
};

#endif //SMALLLANG_ASTEXPRS_H
