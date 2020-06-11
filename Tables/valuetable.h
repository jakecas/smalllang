//
// Created by Jake on 11/06/2020.
//

#ifndef SMALLLANG_VALUETABLE_H
#define SMALLLANG_VALUETABLE_H

#include "symboltable.h"

class VarVal : public Var {
private:
    ASTLiteral* val;
public:
    VarVal(string id, Datatype datatype, ASTLiteral* val): Var(id, datatype){
        this->val = val;
    }

    ASTLiteral* getVal(){
        return val;
    }
    void setVal(ASTLiteral* val){
        this->val = val;
    }
};

class FuncVal : public Func {
private:
    ASTBlock* block;
public:
    FuncVal(string id, Datatype datatype, vector<Var*> params, ASTBlock* block): Func(id, datatype, params){
        this->block = block;
    }

    string getParamId(unsigned int i){
        return params[i]->getId();
    }

    ASTBlock* getBlock(){
        return block;
    }
};

class ValueTable : public SymbolTable {
public:
    ValueTable(){}

    VarVal* lookupVarVal(string id){
        return dynamic_cast<VarVal*>(lookupVar(this->getScopes(), id));
    }

    FuncVal* lookupFuncVal(string id){
        return dynamic_cast<FuncVal*>(lookupFunc(this->getScopes(), id));
    }
};

VarVal* resolveVarVal(string id, Datatype datatype, ASTLiteral* val){
    return new VarVal(id, datatype, val);
}

FuncVal* resolveFuncVal(string id, Datatype returnType, vector<ASTFormalParam*> params, ASTBlock* block){
    vector<Var*> vars;
    for(unsigned int i = 0; i < params.size(); i++){
        ASTFormalParam* param = params[i];
        vars.push_back(resolveVar(param->getId(), param->getType()->getDatatype()));
    }
    return new FuncVal(id, returnType, vars, block);
}

#endif //SMALLLANG_VALUETABLE_H
