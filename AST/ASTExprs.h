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

    virtual void accept(Visitor* visitor) = 0;
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

    ASTFactor* getFactor(){
        return factor;
    }
    MultOp getMultOp(){
        return multOp;
    }
    ASTTerm* getTerm(){
        return term;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
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

    ASTTerm* getTerm(){
        return term;
    }
    AddOp getAddOp(){
        return addOp;
    }
    ASTSimpleExpr* getSimpleExpr(){
        return simpleExpr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
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

    ASTSimpleExpr* getSimpleExpr(){
        return simpleExpr;
    }
    RelOp getRelOp(){
        return relOp;
    }
    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
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

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTIndexedId : public ASTId {
private:
    unsigned int index;
public:
    ASTIndexedId(string id, unsigned int index): ASTId(id){
        this->index = index;
    }
    unsigned int getIndex(){
        return index;
    }

    void accept(Visitor* visitor){
        //visitor->visit(this);
    }
};

class ASTActualParam : public ASTNode {
private:
    ASTExpr* expr;
public:
    ASTActualParam(ASTExpr* expr){
        this->expr = expr;
    }

    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
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

    ASTId* getId(){
        return id;
    }
    vector<ASTActualParam*> getActualParams(){
        return actualParams;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTSubExpr : public ASTFactor {
private:
    ASTExpr* expr;
public:
    ASTSubExpr(ASTExpr* expr){
        this->expr = expr;
    }

    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
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

    UnaryOp getUnaryOp(){
        return unaryOp;
    }
    ASTExpr* getExpr(){
        return expr;
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTLiteral : public ASTFactor {
public:
    ASTLiteral(){}

    virtual string toString() = 0;

    virtual void accept(Visitor* visitor) = 0;
};

class ASTBoolLit: public ASTLiteral {
private:
    bool val;
public:
    ASTBoolLit(bool val){
        this->val = val;
    }

    bool getVal(){
        return val;
    }

    string toString(){
        return (val ? "true" : "false");
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTIntLit: public ASTLiteral {
private:
    int val;
public:
    ASTIntLit(int val){
        this->val = val;
    }

    int getVal(){
        return val;
    }

    string toString(){
        return to_string(val);
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTFloatLit: public ASTLiteral {
private:
    float val;
public:
    ASTFloatLit(float val){
        this->val = val;
    }

    float getVal(){
        return val;
    }

    string toString(){
        return to_string(val);
    }

    void accept(Visitor* visitor){
        visitor->visit(this);
    }
};

class ASTCharLit: public ASTLiteral {
private:
    char val;
public:
    ASTCharLit(char val){
        this->val = val;
    }

    char getVal(){
        return val;
    }

    string toString(){
        return to_string(val);
    }

    void accept(Visitor* visitor){
        //visitor->visit(this);
    }
};

#endif //SMALLLANG_ASTEXPRS_H
