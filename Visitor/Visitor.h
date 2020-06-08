//
// Created by Jake on 20/05/2020.
//

#ifndef SMALLLANG_VISITOR_H
#define SMALLLANG_VISITOR_H

class ASTType;
class ASTBoolLit;
class ASTIntLit;
class ASTFloatLit;

class ASTId;
class ASTActualParam;
class ASTFuncCall;
class ASTSubExpr;
class ASTUnaryOp;
class ASTTerm;
class ASTSimpleExpr;
class ASTExpr;

class ASTAssignment;
class ASTVarDecl;
class ASTPrint;
class ASTRtrn;
class ASTIfStmt;
class ASTForStmt;
class ASTWhileStmt;
class ASTFormalParam;
class ASTFuncDecl;
class ASTBlock;

class ASTProgram;

class Visitor {
public:
    virtual void visit(ASTType* type){};
    virtual void visit(ASTBoolLit* boolLit){};
    virtual void visit(ASTIntLit* intLit){};
    virtual void visit(ASTFloatLit* floatLit){};

    virtual void visit(ASTId* id){};
    virtual void visit(ASTActualParam* actualParam){};
    virtual void visit(ASTFuncCall* funcCall){};
    virtual void visit(ASTSubExpr* subExpr){};
    virtual void visit(ASTUnaryOp* unaryOp){};
    virtual void visit(ASTTerm* term){};
    virtual void visit(ASTSimpleExpr* simpleExpr){};
    virtual void visit(ASTExpr* expr){};

    virtual void visit(ASTAssignment* assignment){};
    virtual void visit(ASTVarDecl* varDecl){};
    virtual void visit(ASTPrint* print){};
    virtual void visit(ASTRtrn* rtrn){};
    virtual void visit(ASTIfStmt* ifStmt){};
    virtual void visit(ASTForStmt* forStmt){};
    virtual void visit(ASTWhileStmt* whileStmt){};
    virtual void visit(ASTFormalParam* formalParam){};
    virtual void visit(ASTFuncDecl* funcDecl){};
    virtual void visit(ASTBlock* block){};

    virtual void visit(ASTProgram* program){};
};

class Connector {
public:
    virtual void accept(Visitor* visitor) = 0;
};

#endif //SMALLLANG_VISITOR_H
