//
// Created by Jake on 09/06/2020.
//

#ifndef SMALLLANG_SYMBOLTABLE_H
#define SMALLLANG_SYMBOLTABLE_H

#include <map>
#include "../AST/ASTPrimitives.h"
#include "../AST/ASTExprs.h"

struct VarNotInScope : public exception {
private:
    string msg;
public:
    VarNotInScope(string id){
        this->msg = "Variable \"" + id + "\" not found in scope."
    }
    const char *message () const throw () {
        return msg.c_str();
    }
};
struct FuncNotInScope : public exception {
private:
    string msg;
public:
    FuncNotInScope(string id){
        this->msg = "Function \"" + id + "\" not found in scope."
    }
    const char *message () const throw () {
        return msg.c_str();
    }
};
struct TooManyParams : public exception {
private:
    string msg;
public:
    FuncNotInScope(string id){
        this->msg = "Too many parameters in function call for function with id " + id;
    }
    const char *message () const throw () {
        return msg.c_str();
    }
};


class Var{
private:
    string id;
    Datatype datatype;
public:
    Var(string id, Datatype datatype){
        this->id = id;
        this->datatype = datatype;
    }

    string getId(){
        return id;
    }

    string getDatatype(){
        return datatype;
    }
};
class Func{
private:
    string id;
    Datatype returnType;
    vector<Var*> params;
public:
    Func(string id, Datatype datatype, vector<Var*> params){
        this->id = id;
        this->returnType = returnType;
        this->params = params;
    }

    string getId(){
        return id;
    }

    Datatype getReturnType(){
        return returnType;
    }

    Datatype getParamType(unsigned int i){
        if(i >= params.size()){
            throw new TooManyParams(this->id);
        }
        return params[i].getDatatype();
    }
};

class Scope{
private:
    map<id, Var*> varTable;
    map<id, Func*> funcTable;
public:
    void push(Var* var){
        varTable.insert(pair<id, Var*>(var->getId(), var));
    }
    void push(Func* func){
        funcTable.insert(pair<id, Func*>(func->getId(), func));
    }

    Var* findVar(string id){
        if(varTable.find(id) != varTable.end()) {
            return varTable.find(id)->second;
        }
        throw new VarNotInScope(id);
    }
    Func* findVar(string id){
        if(funcTable.find(id) != funcTable.end()) {
            return funcTable.find(id)->second;
        }
        throw new FuncNotInScope(id);
    }
};

class SymbolTable{
private:
    stack<Scope> scopes;

public:
    stack<Scope> getScopes(){
        return scopes;
    }

    void push(){
        scopes.push(new Scope());
    }

    void insert(Var* var){
        scopes.top().push(var);
    }
    void insert(Func* func){
        scopes.top().push(func);
    }

    bool isVarInCurrScope(string id){
        try{
            scopes.top().findVar(id);
            return true;
        } catch(VarNotInScope* e){
            return false;
        }
    }
    bool isFuncInCurrScope(string id){
        try{
            scopes.top().findFunc(id);
            return true;
        } catch(FuncNotInScope* e){
            return false;
        }
    }

    static Var* lookupVar(stack<Scope> scopes, string id){
        // Base case
        if(scopes.empty()){
            throw new VarNotInScope(id);
        }
        // Inductive case
        try{
            return scopes.top().findVar(id);
        } catch(VarNotInScope* e){
            Scope tmp = scopes.top();
            scopes.pop();
            Var* var = lookupVar(scopes, id);
            scopes.push(tmp);
            return var;
        }
    }
    static Func* lookupFunc(stack<Scope> scopes, string id){
        // Base case
        if(scopes.empty()){
            throw new FuncNotInScope(id);
        }
        // Inductive case
        try{
            return scopes.top().findFunc(id);
        } catch(FuncNotInScope* e){
            Scope tmp = scopes.top();
            scopes.pop();
            Func* func = lookupFunc(scopes, id);
            scopes.push(tmp);
            return func;
        }
    }

    void pop(){
        scopes.pop();
    }

};

Var* resolveVar(ASTId* id, ASTType* type){
    return new Var(id->getId(), type->getDatatype());
}

Func* resolveFunc(ASTId* id, ASTType* type, vector<ASTFormalParam*> params){
    vectors<Var*> vars;
    for(unsigned int i = 0; i < params.size(); i++){
        ASTFormalParam* param = params[i];
        vars.push_back(resolveVar(param->getId(), param->getType()))
    }

    return new Func(id->getId(), type->getDatatype(), vars);
}
#endif //SMALLLANG_SYMBOLTABLE_H
