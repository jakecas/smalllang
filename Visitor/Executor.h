//
// Created by Jake on 09/06/2020.
//

#ifndef SMALLLANG_EXECUTOR_H
#define SMALLLANG_EXECUTOR_H

#include <stack>
#include <string>
#include <functional>

#include "Visitor.h"
#include "../AST/ASTProgram.h"
#include "../Tables/valuetable.h"

class Executor : public Visitor {
private:
    ValueTable* valueTable;
    stack<ASTLiteral*> execStack;
    bool isReturn;

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

    // T is specific ASTLiteral, P is primitive of specific ASTLiteral, and Op is the function to be applied
    template<class T, class Op> void applyOp(Op op){
        T* rhs = dynamic_cast<T*>(stackPop());
        T* lhs = dynamic_cast<T*>(stackPop());
        stackPush(new T(op(lhs->getVal(), rhs->getVal())));
    }
    // This needs to be separate otherwise we could have the result of a comparison between floats stored as ASTFloat
    template<class T, class Op> void applyRelOp(Op op){
        T* rhs = dynamic_cast<T*>(stackPop());
        T* lhs = dynamic_cast<T*>(stackPop());
        stackPush(new ASTBoolLit(op(lhs->getVal(), rhs->getVal())));
    }

public:
    Executor(){
        valueTable = new ValueTable();
        isReturn = false;
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


void Executor::visit(ASTId* id){
    // This is only visited if the id represents a variable.
    stackPush(valueTable->lookupVarVal(id->getId())->getVal());
}
void Executor::visit(ASTActualParam* actualParam){
    actualParam->getExpr()->accept(this);
}
void Executor::visit(ASTFuncCall* funcCall){
    ASTId* id = funcCall->getId();
    FuncVal* func = valueTable->lookupFuncVal(id->getId());

    // Scope for function parameters.
    valueTable->push();

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

    // If it is just a single term, it's already on top of the stack.
    if(term->getTerm()){
        term->getTerm()->accept(this);

        ASTFloatLit* floatLit = dynamic_cast<ASTFloatLit*>(execStack.top());

        // val1 multOp val2
        switch (term->getMultOp()){
            case MULTOP:
                if(floatLit){
                    applyOp<ASTFloatLit>(multiplies<float>());
                } else{
                    applyOp<ASTIntLit>(multiplies<int>());
                }
                break;
            case DIVOP:
                applyOp<ASTFloatLit>(divides<float>());
                break;
            case ANDOP:
                applyOp<ASTBoolLit>(logical_and<bool>());
                break;
        }
    }
}
void Executor::visit(ASTSimpleExpr* simpleExpr){
    simpleExpr->getTerm()->accept(this);

    if(simpleExpr->getSimpleExpr()){
        simpleExpr->getSimpleExpr()->accept(this);

        ASTFloatLit* floatLit = dynamic_cast<ASTFloatLit*>(execStack.top());

        // val1 addOp val2
        switch (simpleExpr->getAddOp()){
            case ADD:
                if(floatLit){
                    applyOp<ASTFloatLit>(plus<float>());
                } else{
                    applyOp<ASTIntLit>(plus<int>());
                }
                break;
            case SUB:
                if(floatLit){
                    applyOp<ASTFloatLit>(minus<float>());
                } else{
                    applyOp<ASTIntLit>(minus<int>());
                }
                break;
            case OR:
                applyOp<ASTBoolLit>(logical_or<bool>());
                break;
        }
    }
}
void Executor::visit(ASTExpr* expr){
    expr->getSimpleExpr()->accept(this);

    if(expr->getExpr()){
        expr->getExpr()->accept(this);

        // Checking types of top of stack without popping
        ASTFloatLit* floatLit = dynamic_cast<ASTFloatLit*>(execStack.top());
        ASTIntLit* intLit = dynamic_cast<ASTIntLit*>(execStack.top());

        // val1 relOp val2
        switch (expr->getRelOp()){
            case LTOP:
                if(floatLit){
                    applyRelOp<ASTFloatLit>(less_equal<float>());
                } else if(intLit){
                    applyRelOp<ASTIntLit>(less_equal<int>());
                } else{
                    applyRelOp<ASTBoolLit>(less_equal<bool>());
                }
                break;
            case LEOP:
                if(floatLit){
                    applyRelOp<ASTFloatLit>(less<float>());
                } else if(intLit){
                    applyRelOp<ASTIntLit>(less<int>());
                } else{
                    applyRelOp<ASTBoolLit>(less<bool>());
                }
                break;
            case NEOP:
                if(floatLit){
                    applyRelOp<ASTFloatLit>(not_equal_to<float>());
                } else if(intLit){
                    applyRelOp<ASTIntLit>(not_equal_to<int>());
                } else{
                    applyRelOp<ASTBoolLit>(not_equal_to<bool>());
                }
                break;
            case GTOP:
                cout << "Is a gtop" << endl;
                if(floatLit){
                    cout << "Is a float" << endl;
                    applyRelOp<ASTFloatLit>(greater<float>());
                    cout << "Result: " << execStack.top()->toString() << endl;
                } else if(intLit){
                    applyRelOp<ASTIntLit>(greater<int>());
                } else{
                    applyRelOp<ASTBoolLit>(greater<bool>());
                }
                break;
            case GEOP:
                if(floatLit){
                    applyRelOp<ASTFloatLit>(greater_equal<float>());
                } else if(intLit){
                    applyRelOp<ASTIntLit>(greater_equal<int>());
                } else{
                    applyRelOp<ASTBoolLit>(greater_equal<bool>());
                }
                break;
            case EQOP:
                if(floatLit){
                    applyRelOp<ASTFloatLit>(equal_to<float>());
                } else if(intLit){
                    applyRelOp<ASTIntLit>(equal_to<int>());
                } else{
                    applyRelOp<ASTBoolLit>(equal_to<bool>());
                }
                break;
        }
    }
}


void Executor::visit(ASTAssignment* assignment){
    ASTId* id = assignment->getId();
    VarVal* var = valueTable->lookupVarVal(id->getId());

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
    rtrn->getExpr()->accept(this);
    isReturn = true;
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
        cout << "Block stmt:" << i << endl;

        stmts[i]->accept(this);
        if(isReturn){
            break;
        }
    }
    valueTable->pop();
}


void Executor::visit(ASTProgram* program){
    vector<ASTStmt*> stmts = program->getStmts();

    valueTable->push();
    for(unsigned int i = 0; i < stmts.size(); i++){
        try {
            cout << "Stmt:" << i << endl;
            stmts[i]->accept(this);
        } catch(exception* e){
            cout << "Runtime error found in program statement: " << i << endl;
            cout << "Top-most stack vars and funcs:\n"<< endl;
            cout << valueTable->getScopes().top()->toString() << endl;
            throw e;
        }
    }
    valueTable->pop();
}

#endif //SMALLLANG_EXECUTOR_H
