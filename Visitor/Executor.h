//
// Created by Jake on 09/06/2020.
//

#ifndef SMALLLANG_SEMANTICANALYZER_H
#define SMALLLANG_SEMANTICANALYZER_H

#include <stack>
#include <string>

#include "Visitor.h"
#include "../AST/ASTProgram.h"
#include "../Tables/valuetable.h"

class Executor : public Visitor {
private:
    ValueTable* valueTable;
    stack<ASTLiteral*> execStack;

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

    ASTLiteral* stackPop(){
        ASTLiteral* val = execStack.top();
        execStack.pop();
        return val;
    }

    void stackPush(ASTLiteral* val){
        execStack.push(val);
    }

public:
    Executor(){
        valueTable = new ValueTable();
    }

    void visit(ASTProgram* program);
};

void Executor::visit(ASTType* type){}
void Executor::visit(ASTBoolLit* boolLit){
    stackPush(boolLit);
}
void Executor::visit(ASTIntLit* intLit){
    stackPush(intLit);
}
void Executor::visit(ASTFloatLit* floatLit){
    stackPush(floatLit);
}


void Executor::visit(ASTId* id){}
void Executor::visit(ASTActualParam* actualParam){
    actualParam->getExpr()->accept(this);
}
void Executor::visit(ASTFuncCall* funcCall){
    ASTId* id = funcCall->getId();
    FuncVal* func = ValueTable::lookupFuncVal(id->getId());

    // Scope for function parameters.
    valueTable->push()

    vector<ASTActualParam*> params = funcCall->getActualParams();
    for(unsigned int i = 0; i < params.size(); i++){
        params[i]->accept(this);
        valueTable->insert(resolveVarVal(func->getParamId(i), func->getParamType(i), stackPop()));
    }

    func->getBlock()->accept(this);

    valueTable->pop();
}
void Executor::visit(ASTSubExpr* subExpr){
    subExpr->getExpr()->accept(this);
}
void Executor::visit(ASTUnaryOp* unaryOp){
    unaryOp->getExpr()->accept(this);
    ASTLiteral* val = stackPop();

    ASTBoolLit* boolLit = dynamic_cast<ASTBoolLit*>(val);
    if(boolLit){
        bool lit = !boolLit->getVal();
        stackPush(new ASTBoolLit(lit));
        return;
    }

    ASTIntLit* intLit = dynamic_cast<ASTIntLit*>(val);
    if(intLit){
        int lit = -intLit->getVal();
        stackPush(new ASTIntLit(lit));
        return;
    }

    ASTFloatLit* floatLit = dynamic_cast<ASTFloatLit*>(val);
    if(floatLit){
        float lit = -floatLit->getVal();
        stackPush(new ASTFloatLit(lit));
        return;
    }
}
void Executor::visit(ASTTerm* term){
    term->getFactor()->accept(this);

    if(term->getTerm()){
        ASTLiteral* val1 = stackPop();

        term->getTerm()->accept(this);
        ASTLiteral* val2 = stackPop();

        // val1 multOp val2
        switch (term->getMultOp()){
            case MULTOP:
                ASTFloatLit* floatLit1 = dynamic_cast<ASTFloatLit*>(val1);
                if(floatLit){
                    float lit1 = floatLit1->getVal();
                    ASTFloatLit* floatLit2 = dynamic_cast<ASTFloatLit*>(val2);
                    float lit2 = floatLit2->getVal();
                    float ans = lit1 * lit2;
                    stackPush(new ASTFloatLit(ans));
                } else{
                    ASTIntLit* intLit1 = dynamic_cast<ASTIntLit*>(val1);
                    int lit1 = intLit1->getVal();
                    ASTIntLit* intLit2 = dynamic_cast<ASTIntLit*>(val2);
                    int lit2 = intLit2->getVal();
                    int ans = lit1 * lit2;
                    stackPush(new ASTIntLit(ans));
                }
                break;
            case DIVOP:
                ASTFloatLit* floatLit1 = dynamic_cast<ASTFloatLit*>(val1);
                float lit1 = floatLit1->getVal();
                ASTFloatLit* floatLit2 = dynamic_cast<ASTFloatLit*>(val2);
                float lit2 = floatLit2->getVal();
                float ans = lit1 / lit2;
                stackPush(new ASTFloatLit(ans));
                break;
            case AND:
                ASTBoolLit* boolLit1 = dynamic_cast<ASTBoolLit*>(val1);
                bool lit1 = boolLit1->getVal();
                ASTBoolLit* boolLit2 = dynamic_cast<ASTBoolLit*>(val2);
                bool lit2 = boolLit2->getVal();
                bool ans = lit1 && lit2;
                stackPush(new ASTBoolLit(ans));
                break;
        }
    }
}
void Executor::visit(ASTSimpleExpr* simpleExpr){
    simpleExpr->getTerm()->accept(this);

    if(simpleExpr->getSimpleExpr()){
        ASTLiteral* val1 = stackPop();

        simpleExpr->getSimpleExpr()->accept(this);
        ASTLiteral* val2 = stackPop();

        // val1 multOp val2
        switch (simpleExpr->getAddOp()){
            case ADD:
                ASTFloatLit* floatLit1 = dynamic_cast<ASTFloatLit*>(val1);
                if(floatLit){
                    float lit1 = floatLit1->getVal();
                    ASTFloatLit* floatLit2 = dynamic_cast<ASTFloatLit*>(val2);
                    float lit2 = floatLit2->getVal();
                    float ans = lit1 + lit2;
                    stackPush(new ASTFloatLit(ans));
                } else{
                    ASTIntLit* intLit1 = dynamic_cast<ASTIntLit*>(val1);
                    int lit1 = intLit1->getVal();
                    ASTIntLit* intLit2 = dynamic_cast<ASTIntLit*>(val2);
                    int lit2 = intLit2->getVal();
                    int ans = lit1 + lit2;
                    stackPush(new ASTIntLit(ans));
                }
                break;
            case SUB:
                ASTFloatLit* floatLit1 = dynamic_cast<ASTFloatLit*>(val1);
                if(floatLit){
                    float lit1 = floatLit1->getVal();
                    ASTFloatLit* floatLit2 = dynamic_cast<ASTFloatLit*>(val2);
                    float lit2 = floatLit2->getVal();
                    float ans = lit1 - lit2;
                    stackPush(new ASTFloatLit(ans));
                } else{
                    ASTIntLit* intLit1 = dynamic_cast<ASTIntLit*>(val1);
                    int lit1 = intLit1->getVal();
                    ASTIntLit* intLit2 = dynamic_cast<ASTIntLit*>(val2);
                    int lit2 = intLit2->getVal();
                    int ans = lit1 - lit2;
                    stackPush(new ASTIntLit(ans));
                }
                break;
            case OR:
                ASTBoolLit* boolLit1 = dynamic_cast<ASTBoolLit*>(val1);
                bool lit1 = boolLit1->getVal();
                ASTBoolLit* boolLit2 = dynamic_cast<ASTBoolLit*>(val2);
                bool lit2 = boolLit2->getVal();
                bool ans = lit1 || lit2;
                stackPush(new ASTBoolLit(ans));
                break;
        }
    }
}
void Executor::visit(ASTExpr* expr){
    expr->getSimpleExpr()->accept(this);

    if(expr->getExpr()){
        ASTLiteral* val1 = stackPop();

        expr->getExpr()->accept(this);
        ASTLiteral* val2 = stackPop();

        // val1 multOp val2
        switch (expr->getRelOp()){
//            case ADD:
//                ASTFloatLit* floatLit1 = dynamic_cast<ASTFloatLit*>(val1);
//                if(floatLit){
//                    float lit1 = floatLit1->getVal();
//                    ASTFloatLit* floatLit2 = dynamic_cast<ASTFloatLit*>(val2);
//                    float lit2 = floatLit2->getVal();
//                    float ans = lit1 + lit2;
//                    stackPush(new ASTFloatLit(ans));
//                } else{
//                    ASTIntLit* intLit1 = dynamic_cast<ASTIntLit*>(val1);
//                    int lit1 = intLit1->getVal();
//                    ASTIntLit* intLit2 = dynamic_cast<ASTIntLit*>(val2);
//                    int lit2 = intLit2->getVal();
//                    int ans = lit1 + lit2;
//                    stackPush(new ASTIntLit(ans));
//                }
//                break;
//            case SUB:
//                ASTFloatLit* floatLit1 = dynamic_cast<ASTFloatLit*>(val1);
//                if(floatLit){
//                    float lit1 = floatLit1->getVal();
//                    ASTFloatLit* floatLit2 = dynamic_cast<ASTFloatLit*>(val2);
//                    float lit2 = floatLit2->getVal();
//                    float ans = lit1 - lit2;
//                    stackPush(new ASTFloatLit(ans));
//                } else{
//                    ASTIntLit* intLit1 = dynamic_cast<ASTIntLit*>(val1);
//                    int lit1 = intLit1->getVal();
//                    ASTIntLit* intLit2 = dynamic_cast<ASTIntLit*>(val2);
//                    int lit2 = intLit2->getVal();
//                    int ans = lit1 - lit2;
//                    stackPush(new ASTIntLit(ans));
//                }
//                break;
//            case OR:
//                ASTBoolLit* boolLit1 = dynamic_cast<ASTBoolLit*>(val1);
//                bool lit1 = boolLit1->getVal();
//                ASTBoolLit* boolLit2 = dynamic_cast<ASTBoolLit*>(val2);
//                bool lit2 = boolLit2->getVal();
//                bool ans = lit1 || lit2;
//                stackPush(new ASTBoolLit(ans));
//                break;
        }
    }
}


void Executor::visit(ASTAssignment* assignment){
    ASTId* id = assignment->getId();
    VarVal* var = ValueTable::lookupVarVal(id->getId());

    assignment->getExpr()->accept(this);

    var->setVal(stackPop());
}
void Executor::visit(ASTVarDecl* varDecl){
    ASTId* id = varDecl->getId();

    Datatype type = varDecl->getType()->getDatatype();

    varDecl->getExpr()->accept(this);

    valueTable->insert(resolveVarVal(id->getId(), type, stackPop()));
}
void Executor::visit(ASTPrint* print){
    print->getExpr()->accept(this);
    cout << stackPop()->toString() << endl;
}
void Executor::visit(ASTRtrn* rtrn){
    EOirgfaoraiaesrignurezruek
}
void Executor::visit(ASTIfStmt* ifStmt){
    ifStmt->getExpr()->accept(this);

    if(dynamic_cast<ASTBoolLit*>(stackPop())->getVal()){
        ifStmt->getTrueBlock()->accept(this);
    } else if(ifStmt->getFalseBlock()){
        ifStmt->getFalseBlock()->accept(this);
    }
}
void Executor::visit(ASTForStmt* forStmt){
    valueTable->push();
    if(forStmt->getVarDecl()) {
        forStmt->getVarDecl()->accept(this);
    }

    while(dynamic_cast<ASTBoolLit*>(stackPop())->getVal()){
        forStmt->getExpr()->accept(this);

        if (forStmt->getAssignment()) {
            forStmt->getAssignment()->accept(this);
        }
        forStmt->getBlock()->accept(this);
    }

    valueTable->pop();
}
void Executor::visit(ASTWhileStmt* whileStmt){
    whileStmt->getExpr()->accept(this);
    while(dynamic_cast<ASTBoolLit*>(stackPop())->getVal()){
        whileStmt->getBlock()->accept(this);
        whileStmt->getExpr()->accept(this);
    }
}
void Executor::visit(ASTFormalParam* formalParam){}
void Executor::visit(ASTFuncDecl* funcDecl){
    // Getting function id, we don't need to check for uniqueness.
    ASTId* id = funcDecl->getId();

    // Getting function datatype, we do not need to handle autos.
    Datatype type = funcDecl->getType()->getDatatype();

    // Getting formal params, no need to handle anything here, that will be done when the function is called.
    vector<ASTFormalParam*> params = funcDecl->getFormalParams();

    // Getting the function block, again simply storing it to be used when the function is called.
    ASTBlock* block = funcDecl->getBlock();

    // Inserting the function into the valueTable.
    valueTable->insert(resolveFuncVal(id->getId(), type, params, block));
}

void Executor::visit(ASTBlock* block){
    vector<ASTStmt*> stmts = block->getStmts();

    valueTable->push();
    for(unsigned int i = 0; i < stmts.size(); i++){
        stmts[i]->accept(this);
    }
    valueTable->pop();
}


void Executor::visit(ASTProgram* program){
    vector<ASTStmt*> stmts = program->getStmts();

    valueTable->push();
    for(unsigned int i = 0; i < stmts.size(); i++){
        try {
            stmts[i]->accept(this);
        } catch(* e){
            cout << "Runtime error found in program statement: " << i << endl;
            cout << "Top-most stack vars and funcs:\n"<< endl;
            cout << valueTable->getScopes().top()->toString() << endl;
            throw e;
        }
    }
    valueTable->pop();
}

#endif //SMALLLANG_SEMANTICANALYZER_H
