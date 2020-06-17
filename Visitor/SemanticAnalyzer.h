//
// Created by Jake on 09/06/2020.
//

#ifndef SMALLLANG_SEMANTICANALYZER_H
#define SMALLLANG_SEMANTICANALYZER_H

#include <string>

#include "Visitor.h"
#include "../AST/ASTProgram.h"
#include "../Tables/symboltable.h"

class SemanticAnalyzer : public Visitor {
private:
    SymbolTable* symbolTable;
    Datatype currType; // To be used when determining autos
    Datatype exprType; // Type of last expression evaluated
    Datatype simpleExprType; // Type of last simple expression evaluated
    Datatype termType; // Type of last term evaluated
    Datatype factorType; // Type of last factor evaluated
    bool idIsFunc; // To be used when checking if an id already exists as a function or a var
    bool idExistsInCurrScope; // Used by visit(ASTId) to alert that the id exists in current scope.
    bool findFactorType;
    bool hasReturn;
    bool isInIfStmt;
    bool isInFunc;

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

    void ifIdExistsThrowException(string type, string id);

public:
    SemanticAnalyzer(){
        symbolTable = new SymbolTable();
        currType = AUTOTYPE;
        exprType = AUTOTYPE;
        simpleExprType = AUTOTYPE;
        termType = AUTOTYPE;
        factorType = AUTOTYPE;
        idIsFunc = false;
        findFactorType = false;
        hasReturn = false;
        isInIfStmt = false;
        isInFunc = false;
    }

    void visit(ASTProgram* program);
};
void SemanticAnalyzer::ifIdExistsThrowException(string type, string id){
    if(idExistsInCurrScope){
        string typeExists;
        if(idIsFunc) {
            typeExists = getDatatypeName(SymbolTable::lookupFunc(symbolTable->getScopes(), id)->getReturnType());
        } else {
            typeExists = getDatatypeName(SymbolTable::lookupVar(symbolTable->getScopes(), id)->getDatatype());
        }
        throw new SemanticErrorException(type + " with id " + id + " already exists in this scope with type: " + typeExists);
    }
}

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
    factorType = FLOATTYPE;
}


void SemanticAnalyzer::visit(ASTId* id){
    if(idIsFunc && symbolTable->isFuncInCurrScope(id->getId())) {
        idExistsInCurrScope = true;
    } else if(!idIsFunc && symbolTable->isVarInCurrScope(id->getId())) {
        idExistsInCurrScope = true;
    }
    if(!idIsFunc && findFactorType){
        factorType = SymbolTable::lookupVar(symbolTable->getScopes(), id->getId())->getDatatype();
    }
}
void SemanticAnalyzer::visit(ASTActualParam* actualParam){
    // we do not want to save the previous exprType here, that is done in the function call visitor
    actualParam->getExpr()->accept(this);
}
void SemanticAnalyzer::visit(ASTFuncCall* funcCall){
    ASTId* id = funcCall->getId();
    idIsFunc = true;
    id->accept(this);
    Func* func = SymbolTable::lookupFunc(symbolTable->getScopes(), id->getId());
    idIsFunc = false;

    vector<ASTActualParam*> params = funcCall->getActualParams();
    for(unsigned int i = 0; i < params.size(); i++){
        Datatype prev = exprType;
        params[i]->accept(this);
        if(func->getParamType(i) != exprType){
            throw new SemanticErrorException("Parameter " + to_string(i+1) +" in function call to " + id->getId() + " is of type " + getDatatypeName(exprType) + ". Expected: " + getDatatypeName(func->getParamType(i)));
        }
        exprType = prev;
    }
    factorType = func->getReturnType();
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
        throw new SemanticErrorException("Expression of datatype " + getDatatypeName(exprType) + " cannot be used in NOT operation");
    } else if(unaryOp->getUnaryOp() == MINUS && exprType == BOOLTYPE){
        throw new SemanticErrorException("Expression of datatype " + getDatatypeName(exprType) + " cannot be used in NOT operation");
    }
    factorType = exprType;
    exprType = prev;
}
void SemanticAnalyzer::visit(ASTTerm* term){
    Datatype prev = factorType;
    findFactorType = true;
    term->getFactor()->accept(this);
    findFactorType = false;

    if(termType == AUTOTYPE){
        termType = factorType;
    } else if(termType != factorType){
        throw new SemanticErrorException("Type mismatch in term: " + getDatatypeName(termType) + " cannot be used in multOps with " + getDatatypeName(factorType));
    }
    factorType = prev;

    if(term->getTerm()){
        if(term->getMultOp() == ANDOP && termType != BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(termType) + " cannot be used in AND operation");
        } else if(term->getMultOp() != ANDOP && termType == BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(termType) + " cannot be used in * or / operations");
        }
        if(term->getMultOp() == DIVOP && termType != FLOATTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(termType) + " cannot be used in / operations, only floats");
        }

        term->getTerm()->accept(this);
    }
}
void SemanticAnalyzer::visit(ASTSimpleExpr* simpleExpr){
    Datatype prev = termType;
    simpleExpr->getTerm()->accept(this);
    if(simpleExprType == AUTOTYPE){
        simpleExprType = termType;
    } else if(simpleExprType != termType){
        throw new SemanticErrorException("Type mismatch in simple expression: " + getDatatypeName(simpleExprType) + " cannot be used in addOps with " + getDatatypeName(termType));
    }
    termType = prev;

    if(simpleExpr->getSimpleExpr()){
        if(simpleExpr->getAddOp() == OR && simpleExprType != BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(simpleExprType) + " cannot be used in OR operation");
        } else if(simpleExpr->getAddOp() != OR && simpleExprType == BOOLTYPE){
            throw new SemanticErrorException("Datatype " + getDatatypeName(simpleExprType) + " cannot be used in + or - operations");
        }

        simpleExpr->getSimpleExpr()->accept(this);
    }
}
void SemanticAnalyzer::visit(ASTExpr* expr){
    Datatype prev = simpleExprType;
    expr->getSimpleExpr()->accept(this);
    if(exprType == AUTOTYPE){
        exprType = simpleExprType;
    } else if(exprType != simpleExprType){
        throw new SemanticErrorException("Type mismatch in expression: " + getDatatypeName(exprType) + " cannot be related to " + getDatatypeName(simpleExprType));
    }
    simpleExprType = prev;

    if(expr->getExpr()){
        // Relop doesn't matter in semantic analysis, (though it could be added here that <, >, <=, >= cannot be used with bools)
        // What matters is that if all operands are of the same type, then the expression results in bool
        expr->getExpr()->accept(this);
        exprType = BOOLTYPE;
    }
}


void SemanticAnalyzer::visit(ASTAssignment* assignment){
    ASTId* id = assignment->getId();
    id->accept(this);
    idExistsInCurrScope = false;
    Var* var = SymbolTable::lookupVar(symbolTable->getScopes(), id->getId());

    Datatype prev = exprType;
    assignment->getExpr()->accept(this);

    if(var->getDatatype() != exprType){
        throw new SemanticErrorException("Variable " + var->getId() + " of type " + getDatatypeName(var->getDatatype()) + " cannot be assigned a value of type " + getDatatypeName(exprType));
    }
    exprType = prev;
}
void SemanticAnalyzer::visit(ASTVarDecl* varDecl){
    ASTId* id = varDecl->getId();
    idExistsInCurrScope = false;
    id->accept(this);
    ifIdExistsThrowException("Variable", id->getId());

    Datatype prevType = currType;
    ASTType* type = varDecl->getType();
    currType = type->getDatatype();
    type->accept(this);

    Datatype prevExpr = exprType;
    varDecl->getExpr()->accept(this);

    if(currType == AUTOTYPE){
        currType = exprType;
    } else if(currType != exprType){
        throw new SemanticErrorException("Variable type " + getDatatypeName(currType) + " does not match assigned expression type " + getDatatypeName(exprType));
    }
    symbolTable->insert(resolveVar(id, currType));

    exprType = prevExpr;
    currType = prevType;
}
void SemanticAnalyzer::visit(ASTPrint* print){
    Datatype prev = exprType;
    print->getExpr()->accept(this);
    exprType = prev;
}
void SemanticAnalyzer::visit(ASTRtrn* rtrn){
    if(!isInFunc){
        throw new SemanticErrorException("Cannot have a return statement outside of a function block.");
    }

    Datatype prev = exprType;
    rtrn->getExpr()->accept(this);
    if(currType == AUTOTYPE){
        currType = exprType;
    } else if(currType != exprType){
        throw new SemanticErrorException("Function type " + getDatatypeName(currType) + " does not match return expression type " + getDatatypeName(exprType));
    }
    exprType = prev;
    // The following sets the hasReturn flag to true if this return statement is not in an if statement.
    hasReturn = hasReturn || !isInIfStmt;
}
void SemanticAnalyzer::visit(ASTIfStmt* ifStmt){
    isInIfStmt = true;
    Datatype prev = exprType;
    ifStmt->getExpr()->accept(this);
    if(exprType != BOOLTYPE){
        throw new SemanticErrorException("Expression in if statement must be of type bool, not " + getDatatypeName(exprType));
    }
    exprType = prev;
    ifStmt->getTrueBlock()->accept(this);
    if(ifStmt->getFalseBlock()){
        ifStmt->getFalseBlock()->accept(this);
    }
    isInIfStmt = false;
}
void SemanticAnalyzer::visit(ASTForStmt* forStmt){
    symbolTable->push();
    if(forStmt->getVarDecl()) {
        forStmt->getVarDecl()->accept(this);
    }
    Datatype prev = exprType;
    forStmt->getExpr()->accept(this);

    if(exprType != BOOLTYPE){
        throw new SemanticErrorException("Expression in for statement must be of type bool, not " + getDatatypeName(exprType));
    }
    exprType = prev;

    if(forStmt->getAssignment()) {
        forStmt->getAssignment()->accept(this);
    }
    forStmt->getBlock()->accept(this);
    symbolTable->pop();
}
void SemanticAnalyzer::visit(ASTWhileStmt* whileStmt){
    Datatype prev = exprType;
    whileStmt->getExpr()->accept(this);
    if(exprType != BOOLTYPE){
        throw new SemanticErrorException("Expression in if statement must be of type bool, not " + getDatatypeName(exprType));
    }
    exprType = prev;
    whileStmt->getBlock()->accept(this);
}
void SemanticAnalyzer::visit(ASTFormalParam* formalParam){
    ASTId* id = formalParam->getId();
    idExistsInCurrScope = false;
    id->accept(this);
    ifIdExistsThrowException("Variable", id->getId());
    ASTType* type = formalParam->getType();
    type->accept(this);

    if(type->getDatatype() == AUTOTYPE){
        throw new SemanticErrorException("Formal parameters cannot have type: " + getDatatypeName(type->getDatatype()));
    }

    symbolTable->insert(resolveVar(id, type->getDatatype()));
}
void SemanticAnalyzer::visit(ASTFuncDecl* funcDecl){
    isInFunc = true;
    hasReturn = false;

    ASTId* id = funcDecl->getId();
    idIsFunc = true;
    idExistsInCurrScope = false;
    id->accept(this);
    ifIdExistsThrowException("Function", id->getId());
    idIsFunc = false;

    Datatype prev = currType;
    ASTType* type = funcDecl->getType();
    // set it to auto, then check against function type so as to ensure a return statement exists.
    currType = AUTOTYPE;
    type->accept(this);

    vector<ASTFormalParam*> params = funcDecl->getFormalParams();
    symbolTable->push();
    for(unsigned int i = 0; i < params.size(); i++){
        params[i]->accept(this);
    }

    funcDecl->getBlock()->accept(this);

    symbolTable->pop();

    if(currType == AUTOTYPE){
        throw new SemanticErrorException("Function " + id->getId() + " has no return statement.");
    } else if(type->getDatatype() != AUTOTYPE && currType != type->getDatatype()){
        // return expression doesn't match the type of the function (which is not auto)
        throw new SemanticErrorException("Function " + id->getId() + " of type " + getDatatypeName(type->getDatatype()) + " has return statement of type " + getDatatypeName(currType));
    } else if(!hasReturn){
        throw new SemanticErrorException("Function " + id->getId() + " has no return statement outside of an if statement.");
    }
    symbolTable->insert(resolveFunc(id, currType, params));
    currType =  prev;
    isInFunc = false;
}

void SemanticAnalyzer::visit(ASTBlock* block){
    vector<ASTStmt*> stmts = block->getStmts();

    symbolTable->push();
    for(unsigned int i = 0; i < stmts.size(); i++){
        stmts[i]->accept(this);
    }
    symbolTable->pop();
}


void SemanticAnalyzer::visit(ASTProgram* program){
    vector<ASTStmt*> stmts = program->getStmts();

    symbolTable->push();
    for(unsigned int i = 0; i < stmts.size(); i++){
        try {
            stmts[i]->accept(this);
        } catch(SemanticErrorException* e){
            cout << "Semantic error found in program statement: " << i+1 << endl;
            //cout << "Top-most stack vars and funcs:\n"<< endl;
            //cout << symbolTable->getScopes().top()->toString() << endl;
            throw e;
        }
    }
    symbolTable->pop();
}

#endif //SMALLLANG_SEMANTICANALYZER_H
