//
// Created by Jake on 09/06/2020.
//

#ifndef SMALLLANG_SEMANTICANALYZER_H
#define SMALLLANG_SEMANTICANALYZER_H

#include <string>

#include "Visitor.h"
#include "../AST/ASTProgram.h"
#include "../Tables/symboltable.h"


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
void ifIdExistsThrowExceptionThenReset(string type, string id){
    if(idExists){
        throw new SemanticErrorException(type + " with id " + id + " already exists in this scope.");
    }
    idExists = false;
}

class SemanticAnalyzer : public Visitor {
private:
    SymbolTable symbolTable;
    Datatype currType; // To be used when determining autos
    Datatype exprType; // Type of last expression evaluated
    Datatype simpleExprType; // Type of last simple expression evaluated
    Datatype termType; // Type of last term evaluated
    Datatype factorType; // Type of last factor evaluated
    bool idIsFunc; // To be used when checking if an id already exists as a function or a var
    bool idExists;

    void visit(ASTType* type);
    void visit(ASTBoolLit* boolLit);
    void visit(ASTIntLit* intLit);
    void visit(ASTFloatLit* floatLit);

    void visit(ASTId* id);
    void visit(ASTActualParam* actualParam);
    void visit(ASTFuncCall* funcCall);
    void visit(ASTSubExpr* subExpr);
    void visit(ASTUnaryOp* unaryOp);
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
    void visit(ASTBlock* block);

public:
    SemanticAnalyzer(){
        SymbolTable = new SymbolTable();
        currType = AUTOTYPE;
        exprType = AUTOTYPE;
        simpleExprType = AUTOTYPE;
        termType = AUTOTYPE;
        factorType = AUTOTYPE;
        idIsFunc = false;
    }

    void visit(ASTProgram* program);
};

void SemanticAnalyzer::visit(ASTType* type){
    // currently empty, but it is still called where it should be so it can be implemented in the future if needs be.
}
void SemanticAnalyzer::visit(ASTBoolLit* boolLit){
    factorType = BOOLTYPE;
}
void SemanticAnalyzer::visit(ASTIntLit* intLit){
    factorType = INTTYPE;
}
void SemanticAnalyzer::visit(ASTFloatLit* floatLit){

}


void SemanticAnalyzer::visit(ASTId* id){
    if(idIsFunc && symbolTable.isFuncInCurrScope(id)) {
        idExists = true;
    } else if(!idIsFunc && symbolTable.isVarInCurrScope(id)) {
        idExists = true;
    }
}
void SemanticAnalyzer::visit(ASTActualParam* actualParam){
    actualParam->getExpr()->accept(this);
}
void SemanticAnalyzer::visit(ASTFuncCall* funcCall){
    ASTId* id = assignment->getId();
    id->accept(this);
    idExists = false;
    Func* func = SymbolTable.lookupFunc(symbolTable.getScopes(), id->getId());

    vector<ASTActualParam*> params = funcCall->getActualParams();
    for(unsigned int i = 0; i < params.size(); i++){
        Datatype prev = exprType;
        params[i]->accept(this);
        if(func->getParamType(i) != exprType){
            throw new SemanticErrorException("Parameter " + (i+1) +" in function call to " + id->getId() + " is of type " + getDatatypeName(exprType) + ". Expected: " + getDatatypeName(func->getParamType(i)));
        }
        exprType = prev;
    }

}
void SemanticAnalyzer::visit(ASTSubExpr* subExpr){
    Datatype prev = exprType;
    subExpr->getExpr()->accept(this);
    factorType = exprType;
    exprType = prev;

}
void SemanticAnalyzer::visit(ASTUnaryOp* unaryOp){
    Datatype prev = exprType;
    unaryOp->getExpr()->accept(this);
    if(unaryOp->getUnaryOp() == NOT && exprType != BOOLTYPE){
        throw new SemanticErrorException("Expression of datatype " + getDatatypeName(exprType) + " cannot be used in NOT operation")
    } else if(unaryOp->getUnaryOp() == MINUS && exprType == BOOLTYPE){
        throw new SemanticErrorException("Expression of datatype " + getDatatypeName(exprType) + " cannot be used in NOT operation")
    }
    factorType = exprType;
    exprType = prev;
}
void SemanticAnalyzer::visit(ASTTerm* term){
    Datatype prev = factorType;
    term->getFactor()->accept(this);
    if(termType == AUTOTYPE){
        termType = factorType;
    } else if(termType != factorType){
        throw new SemanticErrorException("Type mismatch in term: " + getDatatypeName(exprType) + " cannot be used in multOps with " + getDatatypeName(termType));
    }

    if(term->getTerm()){
        if(term->getMultOp() == ANDOP && termType != BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(termType) + " cannot be used in AND operation")
        } else if(term->getMultOp() != ANDOP && termType == BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(termType) + " cannot be used in * or / operations")
        }
        if(term->getMultOp() == DIVOP && termType != FLOATTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(termType) + " cannot be used in / operations, only floats")
        }

        term->getTerm()->accept(this);
    }
    factorType = prev;
}
void SemanticAnalyzer::visit(ASTSimpleExpr* simpleExpr){
    Datatype prev = termType;
    simpleExpr->getTerm()->accept(this);
    if(simpleExprType == AUTOTYPE){
        simpleExprType = termType;
    } else if(simpleExprType != termType){
        throw new SemanticErrorException("Type mismatch in simple expression: " + getDatatypeName(simpleExprType) + " cannot be used in addOps with " + getDatatypeName(termType));
    }

    if(simpleExpr->getSimpleExpr()){
        if(simpleExpr->getAddOp() == OR && simpleExprType != BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(simpleExprType) + " cannot be used in OR operation")
        } else if(simpleExpr->getAddOp() != OR && simpleExprType == BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(simpleExprType) + " cannot be used in + or - operations")
        }

        simpleExpr->getSimpleExpr()->accept(this);
    }
    termType = prev;
}
void SemanticAnalyzer::visit(ASTExpr* expr){
    Datatype prev = simpleExprType;
    expr->getSimpleExpr()->accept(this);
    if(exprType == AUTOTYPE){
        exprType = simpleExprType;
    } else if(exprType != simpleExprType){
        throw new SemanticErrorException("Type mismatch in expression: " + getDatatypeName(exprType) + " cannot be related to " + getDatatypeName(simpleExprType));
    }

    if(expr->getExpr()){
        // Relop doesn't matter in semantic analysis, (though it could be added here that <, >, <=, >= cannot be used with bools)
        expr->getExpr()->accept(this);
    }
    simpleExprType = prev;
}


void SemanticAnalyzer::visit(ASTAssignment* assignment){
    ASTId* id = assignment->getId();
    id->accept(this);
    idExists = false;
    Var* var = SymbolTable.lookupVar(symbolTable.getScopes(), id->getId());
    assignment->getExpr()->accept(this);

    if(var->getDatatype() != exprType){
        throw new SemanticErrorException("Variable " + var->getId() + " of type " + getDatatypeName(var->getDatatype()) + " cannot be assigned a value of type " + getDatatypeName(exprType));
    }
}
void SemanticAnalyzer::visit(ASTVarDecl* varDecl){
    ASTId* id = varDecl->getId();
    id->accept(this);
    ifIdExistsThrowExceptionThenReset("Variable", id->getId());

    Datatype prev = currType;
    ASTType type = varDecl->getType();
    currType = type.getDatatype();
    type->accept(this);
    varDecl->getExpr()->accept(this);

    if(currType == AUTOTYPE){
        currType = exprType;
    } else if(currType != exprType){
        throw new SemanticErrorException("Function type " + getDatatypeName(currType) + " does not match return expression type " + getDatatypeName(exprType))
    }

    symbolTable.insert(resolveVar(id->getId(), currType))

    currType = prev;
}
void SemanticAnalyzer::visit(ASTPrint* print){
    print->getExpr()->accept(this);
}
void SemanticAnalyzer::visit(ASTRtrn* rtrn){
    rtrn->getExpr()->accept(this);
    if(currType == AUTOTYPE){
        currType = exprType;
    } else if(currType != exprType){
        throw new SemanticErrorException("Function type " + getDatatypeName(currType) + " does not match return expression type " + getDatatypeName(exprType))
    }
}
void SemanticAnalyzer::visit(ASTIfStmt* ifStmt){
    ifStmt->getExpr()->accept(this);
    if(exprType != BOOLTYPE){
        throw new SemanticErrorException("Expression in if statement must be of type bool, not " + getDatatypeName(exprType))
    }
    ifStmt->getTrueBlock()->accept(this);
    if(ifStmt->getFalseBlock()){
        ifStmt->getFalseBlock()->accept(this);
    }

}
void SemanticAnalyzer::visit(ASTForStmt* forStmt){
    symbolTable.push()
    if(forStmt->getVarDecl()) {
        forStmt->getVarDecl()->accept(this);
    }
    forStmt->getExpr()->accept(this);

    if(exprType != BOOLTYPE){
        throw new SemanticErrorException("Expression in for statement must be of type bool, not " + getDatatypeName(exprType))
    }

    if(forStmt->getAssignment()) {
        forStmt->getAssignment()->accept(this);
    }
    forStmt->getBlock()->accept(this);
    symbolTable.pop();
}
void SemanticAnalyzer::visit(ASTWhileStmt* whileStmt){
    whileStmt->getExpr()->accept(this);
    if(exprType != BOOLTYPE){
        throw new SemanticErrorException("Expression in if statement must be of type bool, not " + getDatatypeName(exprType))
    }
    whileStmt->getBlock()->accept(this);
}
void SemanticAnalyzer::visit(ASTFormalParam* formalParam){
    ASTId* id = formalParam->getId();
    id->accept(this);
    ifIdExistsThrowExceptionThenReset("Variable", id->getId());
    ASTType* type = formalParam->getType();
    type->accept(this);

    if(type->getDatatype() == AUTOTYPE){
        throw new SemanticErrorException("Formal parameters cannot have type: auto");
    }

    symbolTable.insert(id, type);
}
void SemanticAnalyzer::visit(ASTFuncDecl* funcDecl){
    ASTId* id = funcDecl->getId();
    idIsFunc = true;
    id->accept(this);
    ifIdExistsThrowExceptionThenReset("Function", id->getId());
    idIsFunc = false;

    Datatype prev = currType;
    ASTType* type = funcDecl->getType();
    currType = type->getDatatype();
    type->accept(this);

    vector<ASTFormalParam*> params = funcDecl->getFormalParams();
    symbolTable.push()
    for(unsigned int i = 0; i < params.size(); i++){
        params[i]->accept(this);
    }

    funcDecl->getBlock()->accept(this);

    symbolTable.pop();

    if(currType == AUTOTYPE){
        throw new SemanticErrorException("Function " + id->getId() + " has no return statement.");
    }
    symbolTable.insert(resolveFunc(id, currType, params));
    currType =  prev;
}

void SemanticAnalyzer::visit(ASTBlock* block){
    vector<ASTStmt*> stmts = block->getStmts();

    symbolTable.push();
    for(unsigned int i = 0; i < stmts.size(); i++){
        stmts[i]->accept(this);
    }
    symbolTable.pop();
}


void SemanticAnalyzer::visit(ASTProgram* program){
    vector<ASTStmt*> stmts = program->getStmts();

    symbolTable.push();
    for(unsigned int i = 0; i < stmts.size(); i++){
        stmts[i]->accept(this);
    }
    symbolTable.pop();
}

#endif //SMALLLANG_SEMANTICANALYZER_H
