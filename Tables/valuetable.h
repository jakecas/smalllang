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
    VarVal(Var* var, ASTLiteral* val): Var(var->getId(), var->getDatatype()){
        this->val = val;
    }

    ASTLiteral* getVal(){
        return val;
    }
};

class FuncVal : public Func {
private:
    ASTBlock* block;
public:
    FuncVal(string id, Datatype datatype, vector<Var*> params, ASTBlock* block): Func(id, datatype, params){
        this->block = block;
    }
    FuncVal(Func* func, ASTBlock* block): Func(func->getId(), func->getReturnType(), func->getParams()){
        this->block = block;
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

VarVal* resolveVarVal(ASTId* id, Datatype datatype, ASTLiteral* val){
    return new VarVal(resolveVar(id, datatype), val);
}

FuncVal* resolveFuncVal(ASTId* id, Datatype returnType, vector<ASTFormalParam*> params, ASTBlock* block){
    return new FuncVal(resolveFunc(id, returnType, params), block);
}

#endif //SMALLLANG_VALUETABLE_H
