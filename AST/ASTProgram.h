//
// Created by Jake on 13/05/2020.
//

#ifndef SMALLLANG_AST_H
#define SMALLLANG_AST_H

#include <vector>

#include "ASTNode.h"
#include "ASTStmts.h"

class ASTProgram : public ASTNode {
private:
    vector<ASTStmt*> stmts;
public:
    ASTProgram(){}

    void addStmt(ASTStmt* stmt){
        stmts.push_back(stmt);
    }
};

#endif //SMALLLANG_AST_H
