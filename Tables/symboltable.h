//
// Created by Jake on 09/06/2020.
//

#ifndef SMALLLANG_SYMBOLTABLE_H
#define SMALLLANG_SYMBOLTABLE_H

#include <map>
#include "../AST/ASTPrimitives.h"
#include "../AST/ASTExprs.h"

using namespace std;


struct SemanticErrorException : public exception {
private:
    string msg;
public:
    SemanticErrorException(string msg){
        this->msg = msg;
    }
    const char *message () const throw () {
        return msg.c_str();
    }
};

struct VarNotInScope : public SemanticErrorException {
public:
    VarNotInScope(string id):SemanticErrorException("Variable \"" + id + "\" not found in scope."){}
};
struct FuncNotInScope : public SemanticErrorException {
public:
    FuncNotInScope(string id):SemanticErrorException("Function \"" + id + "\" not found in scope."){}
};
struct TooManyParams : public SemanticErrorException {
public:
    TooManyParams(string id):SemanticErrorException("Too many parameters in function call for function with id " + id){}
};

string getDatatypeName(Datatype datatype){
    switch(datatype){
        case FLOATTYPE:
            return "float";
        case INTTYPE:
            return "int";
        case BOOLTYPE:
            return "bool";
        case AUTOTYPE:
            return "auto";
    }
}


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
    Datatype getDatatype(){
        return datatype;
    }

    string toString(){
        return id + ": " + getDatatypeName(datatype);
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
            throw new TooManyParams(id);
        }
        return params[i]->getDatatype();
    }

    string toString(){
        string out = id + ": " + getDatatypeName(returnType) + "\n\tParams:\t";

        vector<Var*>::iterator it = params.begin();
        while(it != params.end()){
            out += (*it)->toString() + ", ";
            it++;
        }
        return out;
    }
};

class Scope{
private:
    map<string, Var*> varTable;
    map<string, Func*> funcTable;
public:
    void push(Var* var){
        varTable.insert(pair<string, Var*>(var->getId(), var));
    }
    void push(Func* func){
        funcTable.insert(pair<string, Func*>(func->getId(), func));
    }

    Var* findVar(string id){
        if(varTable.find(id) != varTable.end()) {
            return varTable.find(id)->second;
        }
        throw new VarNotInScope(id);
    }
    Func* findFunc(string id){
        if(funcTable.find(id) != funcTable.end()) {
            return funcTable.find(id)->second;
        }
        throw new FuncNotInScope(id);
    }

    string toString(){
        string out = "Vars:\n";
        map<string, Var*>::iterator itv = varTable.begin();
        while(itv != varTable.end()){
            out += "\t" + itv->second->toString() + "\n";
            itv++;
        }

        out += "Funcs:\n";
        map<string, Func*>::iterator itf = funcTable.begin();
        while(itf != funcTable.end()){
            out += "\t" + itf->second->toString() + "\n";
            itf++;
        }
        return out;
    }
};

class SymbolTable{
private:
    stack<Scope*> scopes;

public:
    stack<Scope*> getScopes(){
        return scopes;
    }

    void push(){
        scopes.push(new Scope());
    }

    void insert(Var* var){
        scopes.top()->push(var);
    }
    void insert(Func* func){
        scopes.top()->push(func);
    }

    bool isVarInCurrScope(string id){
        try{
            scopes.top()->findVar(id);
            return true;
        } catch(VarNotInScope* e){
            return false;
        }
    }
    bool isFuncInCurrScope(string id){
        try{
            scopes.top()->findFunc(id);
            return true;
        } catch(FuncNotInScope* e){
            return false;
        }
    }

    static Var* lookupVar(stack<Scope*> scopes, string id){
        // Base case
        if(scopes.empty()){
            throw new VarNotInScope(id);
        }
        // Inductive case
        try{
            return scopes.top()->findVar(id);
        } catch(VarNotInScope* e){
            Scope* tmp = scopes.top();
            scopes.pop();
            Var* var = lookupVar(scopes, id);
            scopes.push(tmp);
            return var;
        }
    }

    static Func* lookupFunc(stack<Scope*> scopes, string id){
        // Base case
        if(scopes.empty()){
            throw new FuncNotInScope(id);
        }
        // Inductive case
        try{
            return scopes.top()->findFunc(id);
        } catch(FuncNotInScope* e){
            Scope* tmp = scopes.top();
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

Var* resolveVar(ASTId* id, Datatype datatype){
    return new Var(id->getId(), datatype);
}

Func* resolveFunc(ASTId* id, Datatype returnType, vector<ASTFormalParam*> params){
    vector<Var*> vars;
    for(unsigned int i = 0; i < params.size(); i++){
        ASTFormalParam* param = params[i];
        vars.push_back(resolveVar(param->getId(), param->getType()->getDatatype()));
    }

    return new Func(id->getId(), returnType, vars);
}
#endif //SMALLLANG_SYMBOLTABLE_H
