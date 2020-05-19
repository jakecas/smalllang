//
// Created by Jake on 19/05/2020.
//

#ifndef SMALLLANG_ASTPRIMITIVES_H
#define SMALLLANG_ASTPRIMITIVES_H

#include "ASTNode.h"

enum RelOp {
    LTOP,
    LEOP,
    NEOP,
    GTOP,
    GEOP,
    EQOP
};

enum AddOp {
    ADD,
    SUB,
    OR
};

enum MultOp {
    MULTOP,
    DIVOP,
    ANDOP
};

enum Datatype {
    FLOATTYPE,
    INTTYPE,
    BOOLTYPE,
    AUTOTYPE
};

enum UnaryOp{
    MINUS,
    NOT
};

class ASTType : public ASTNode {
private:
    Datatype datatype;
public:
    ASTType(Datatype datatype){
        this->datatype = datatype;
    }
};

#endif //SMALLLANG_ASTPRIMITIVES_H
