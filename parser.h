//
// Created by Jake on 13/05/2020.
//

#ifndef SMALLLANG_PARSER_H
#define SMALLLANG_PARSER_H


#include <queue>
#include <deque>
#include <vector>
#include "lexer.h"
#include "AST/ASTProgram.h"

using namespace std;

// Thrown if the file could not be opened.
struct SyntaxErrorException : public exception {
private:
    string msg;
public:
    SyntaxErrorException(int linec, string msg){
        this->msg = "line:" + to_string(linec) + "; " + msg;
    }
    const char *message () const throw () {
        return msg.c_str();
    }
};

class Parser {
private:
    Lexer* lexer;
    ASTProgram* astTree;
    deque<Token*> lookaheadBuffer;

    // Internal Functions
    Token* consumeNullPtrsAndGetToken();
    vector<Token*> lookahead(int c);
    void checkAndConsumeNextToken(Type type, string msg);
    Token* peekNextToken();
    bool checkEof();
    bool isNextToken(Type type);
    Token* nextToken();

    // Parsing functions
    ASTType* parseType();
    ASTBoolLit* parseBoolLit();
    ASTIntLit* parseIntLit();
    ASTFloatLit* parseFloatLit();
    ASTId* parseId();
    MultOp parseMultOp();
    AddOp parseAddOp();
    RelOp parseRelOp();

    //-- Expressions
    ASTActualParam* parseActualParam();
    ASTFuncCall* parseFuncCall();
    ASTSubExpr* parseSubExpr();
    ASTUnaryOp* parseUnaryOp();
    ASTFactor* parseFactor();
    ASTTerm* parseTerm();
    ASTSimpleExpr* parseSimpleExpr();
    ASTExpr* parseExpr();

    //--Statements
    ASTAssignment* parseVarAssign();
    ASTVarDecl* parseVarDecl();
    ASTPrint* parsePrintStmt();
    ASTRtrn* parseRtrnStmt();
    ASTIfStmt* parseIfStmt();
    ASTForStmt* parseForStmt();
    ASTWhileStmt* parseWhileStmt();
    ASTFormalParam* parseFormalParam();
    ASTFuncDecl* parseFuncDecl();
    ASTStmt* parseStmt();
    ASTBlock* parseBlock();

public:
    Parser(Lexer* lexer){
        this->lexer = lexer;
        this->astTree = new ASTProgram;
    }

    ASTProgram* parseProgram();
};

Token* Parser::consumeNullPtrsAndGetToken() {
    Token* tok;
    do{
        tok = lexer->getNextToken();
    } while(!tok);
    return tok;
}

vector<Token*> Parser::lookahead(int c){
    vector<Token*> lookaheadResult;
    for (unsigned int i = 0; i < c; i++) {
        Token* tmp;
        if(i >= lookaheadBuffer.size()){
            lookaheadBuffer.push_back(consumeNullPtrsAndGetToken());
        }
        lookaheadResult.push_back(lookaheadBuffer[i]);
    }
    return lookaheadResult;
}

void Parser::checkAndConsumeNextToken(Type type, string msg){
    if(nextToken()->getType() != type){
        throw new SyntaxErrorException(lexer->getLineNum(), msg);
    }
}
Token* Parser::peekNextToken(){
    return lookahead(1)[0];
}
bool Parser::checkEof(){
    // This part is a workaround, it is not clean and could definitely be improved.
    try{
        peekNextToken();
    } catch (EOFException* e){
        return true;
    }
    return false;
}
bool Parser::isNextToken(Type type){
    return peekNextToken()->getType() == type;
}

Token* Parser::nextToken(){
    if(!lookaheadBuffer.empty()){
        Token* tkn = lookaheadBuffer.front();
        lookaheadBuffer.pop_front();
        return tkn;
    }

    return consumeNullPtrsAndGetToken();
}

ASTType* Parser::parseType(){
    Token* tkn = nextToken();
    if(tkn->getLexeme().compare("auto") == 0){
        return new ASTType(AUTOTYPE);
    } else if(tkn->getLexeme().compare("bool") == 0){
        return new ASTType(BOOLTYPE);
    } else if(tkn->getLexeme().compare("int") == 0){
        return new ASTType(INTTYPE);
    } else if(tkn->getLexeme().compare("float") == 0){
        return new ASTType(FLOATTYPE);
    }
}

ASTVarDecl* Parser::parseVarDecl(){
    // consume let token as it's served its purpose
    nextToken();

    ASTId* id = parseId();
    checkAndConsumeNextToken(CLNL, "Missing ':' character in variable declaration.");

    ASTType* type = parseType();
    checkAndConsumeNextToken(EQUALSL, "Missing '=' character in variable declaration.");

    ASTExpr* expr = parseExpr();
    checkAndConsumeNextToken(SEMICLNL, "Missing ';' character in variable declaration.");

    return new ASTVarDecl(id, type, expr);
}

ASTAssignment* Parser::parseVarAssign(){
    ASTId* id = parseId();
    checkAndConsumeNextToken(EQUALSL, "Missing '=' character in variable assignment.");

    ASTExpr* expr = parseExpr();
    checkAndConsumeNextToken(SEMICLNL, "Missing ';' character in variable assignment.");

    return new ASTAssignment(id, expr);
}

ASTPrint* Parser::parsePrintStmt(){
    // consume print token as it's served its purpose
    nextToken();

    ASTExpr* expr = parseExpr();
    checkAndConsumeNextToken(SEMICLNL, "Missing ';' character in print statement.");

    return new ASTPrint(expr);
}

ASTRtrn* Parser::parseRtrnStmt(){
    // consume return token as it's served its purpose
    nextToken();

    ASTExpr* expr = parseExpr();
    checkAndConsumeNextToken(SEMICLNL, "Missing ';' character in return statement.");

    return new ASTRtrn(expr);
}

ASTIfStmt* Parser::parseIfStmt(){
    // consume if token as it's served its purpose
    nextToken();

    checkAndConsumeNextToken(OPENROUND, "Missing '(' character in if statement.");
    ASTExpr* expr = parseExpr();

    checkAndConsumeNextToken(CLOSEROUND, "Missing ')' character in if statement.");
    ASTBlock* trueBlock = parseBlock();

    if(!isNextToken(ELSEK)) {
        return new ASTIfStmt(expr, trueBlock);
    }
    // Throwaway the else token, it's served its purpose
    nextToken();
    ASTBlock* falseBlock = parseBlock();

    return new ASTIfStmt(expr, trueBlock, falseBlock);
}

ASTForStmt* Parser::parseForStmt(){
    // consume for token as it's served its purpose
    nextToken();

    checkAndConsumeNextToken(OPENROUND, "Missing '(' character in for statement.");

    ASTVarDecl* varDecl = nullptr;
    if(isNextToken(LETK)){
        varDecl = parseVarDecl();
    }

    checkAndConsumeNextToken(SEMICLNL, "Missing ';' character in for statement after optional variable declaration.");

    ASTExpr* expr = parseExpr();

    checkAndConsumeNextToken(SEMICLNL, "Missing ';' character in for statement after expression.");

    ASTAssignment* varAssign = nullptr;
    if(isNextToken(IDT)){
        varAssign = parseVarAssign();
    }

    checkAndConsumeNextToken(CLOSEROUND, "Missing ')' character in for statement.");

    ASTBlock* block = parseBlock();

    return new ASTForStmt(varDecl, expr, varAssign, block);
}

ASTWhileStmt* Parser::parseWhileStmt(){
    // consume while token as it's served its purpose
    nextToken();

    checkAndConsumeNextToken(OPENROUND, "Missing '(' character in while statement.");
    ASTExpr* expr = parseExpr();
    checkAndConsumeNextToken(CLOSEROUND, "Missing ')' character in for statement.");

    ASTBlock* block = parseBlock();

    return new ASTWhileStmt(expr, block);
}

ASTFormalParam* Parser::parseFormalParam() {
    ASTId* id = parseId();
    nextToken(); // consume ':' token as it's served its purpose
    ASTType* type = parseType();
    return new ASTFormalParam(id, type);
}

ASTFuncDecl* Parser::parseFuncDecl(){
    // consume ff token as it's served its purpose
    nextToken();

    ASTId* id = parseId();
    checkAndConsumeNextToken(OPENROUND, "Missing '(' character in function declaration.");
    vector<ASTFormalParam*> params;
    while(!isNextToken(CLOSEROUND)){
        params.push_back(parseFormalParam());
        if(isNextToken(COMMAL)){
            nextToken();
        }
    }
    checkAndConsumeNextToken(CLOSEROUND, "Missing ')' character in function declaration.");

    checkAndConsumeNextToken(CLNL, "Missing ':' character in function declaration.");

    ASTType* type = parseType();
    ASTBlock* block = parseBlock();

    return new ASTFuncDecl(id, params, type, block);
}

// Perhaps put a try catch here in case there is no closing curly brace?
ASTBlock* Parser::parseBlock(){
    // consume '{' token as it's served its purpose
    nextToken();
    vector<ASTStmt*> stmts;

    while(!isNextToken(CLOSECURLY)){
        stmts.push_back(parseStmt());
    }
    // consume '}' token as it's served its purpose
    nextToken();

    return new ASTBlock(stmts);
}

ASTStmt* Parser::parseStmt(){
    Token* tkn = peekNextToken();
    switch(tkn->getType()){
        case LETK:
            // variable declaration
            return parseVarDecl();
        case IDT:
            // variable assignment
            return parseVarAssign();
        case PRINTK:
            // print statement
            return parsePrintStmt();
        case IFK:
            // if statement
            return parseIfStmt();
        case FORK:
            // for statement
            return parseForStmt();
        case WHILEK:
            // while statement
            return parseWhileStmt();
        case RETURNK:
            // return statement
            return parseRtrnStmt();
        case FUNCK:
            // function declaration
            return parseFuncDecl();
        case OPENCURLY:
            // block
            return parseBlock();
        default:
            throw new SyntaxErrorException(lexer->getLineNum(), "Invalid token \""+tkn->getLexeme()+"\" found while parsing statement.");
            // Error?
    }
}

ASTBoolLit* Parser::parseBoolLit(){
    Token* tkn = nextToken();
    if(tkn->getLexeme().compare("true") == 0){
        return new ASTBoolLit(true);
    }
    return new ASTBoolLit(false);
}
ASTIntLit* Parser::parseIntLit(){
    return new ASTIntLit(stoi(nextToken()->getLexeme()));
}
ASTFloatLit* Parser::parseFloatLit(){
    return new ASTFloatLit(stof(nextToken()->getLexeme()));
}

ASTActualParam* Parser::parseActualParam(){
    return new ASTActualParam(parseExpr());
}

ASTFuncCall* Parser::parseFuncCall(){
    ASTId* id = parseId();

    checkAndConsumeNextToken(OPENROUND, "Missing '(' character in function declaration.");
    vector<ASTActualParam*> params;
    while(!isNextToken(CLOSEROUND)){
        params.push_back(parseActualParam());
        if(isNextToken(COMMAL)){
            nextToken();
        }
    }
    checkAndConsumeNextToken(CLOSEROUND, "Missing ')' character in function declaration.");

    return new ASTFuncCall(id, params);
}

ASTId* Parser::parseId(){
    return new ASTId(nextToken()->getLexeme());
}

ASTSubExpr* Parser::parseSubExpr(){
    checkAndConsumeNextToken(OPENROUND, "Missing '(' character in function declaration.");
    ASTExpr* expr = parseExpr();
    checkAndConsumeNextToken(CLOSEROUND, "Missing ')' character in function declaration.");
    return new ASTSubExpr(expr);
}

ASTUnaryOp* Parser::parseUnaryOp(){
    UnaryOp unaryOp;
    if(isNextToken(NOTK)){
        nextToken();
        return new ASTUnaryOp(NOT, parseExpr());
    }
    nextToken();
    return new ASTUnaryOp(MINUS, parseExpr());
}

MultOp Parser::parseMultOp(){
    Token* tkn = nextToken();
    if(tkn->getLexeme().compare("*") == 0){
        return MULTOP;
    } else if(tkn->getLexeme().compare("/") == 0){
        return DIVOP;
    }
    return ANDOP;
}

AddOp Parser::parseAddOp(){
    Token* tkn = nextToken();
    if(tkn->getLexeme().compare("+") == 0){
        return ADD;
    } else if(tkn->getLexeme().compare("-") == 0){
        return SUB;
    }
    return OR;
}

RelOp Parser::parseRelOp(){
    Token* tkn = nextToken();
    if(tkn->getLexeme().compare("<") == 0){
        return LTOP;
    } else if(tkn->getLexeme().compare("<=") == 0){
        return LEOP;
    } else if(tkn->getLexeme().compare("<>") == 0){
        return NEOP;
    } else if(tkn->getLexeme().compare(">") == 0){
        return GTOP;
    } else if(tkn->getLexeme().compare(">=") == 0){
        return GEOP;
    } else if(tkn->getLexeme().compare("==") == 0){
        return EQOP;
    }
}

ASTFactor* Parser::parseFactor(){
    Token* tkn = peekNextToken();
    switch(tkn->getType()){
        case BOOLL:
            return parseBoolLit();
        case INTL:
            return parseIntLit();
        case FLOATL:
            return parseFloatLit();
        case IDT:
            // isNextToken(OPENROUND) cannot be used, as the next token is the identifier.
            // We have to look two tokens ahead, and this is the only place this is done.
            // So an extra (isNextNextToken) function was not made.
            if(lookahead(2)[1]->getType() == OPENROUND){
                return parseFuncCall();
            }
            return parseId();
        case OPENROUND:
            return parseSubExpr();
        case ADDOPT:
            if(tkn->getLexeme().compare("-") != 0){
                throw new SyntaxErrorException(lexer->getLineNum(), tkn->getLexeme() + " is not a valid unary operator.");
            }
        case NOTK:
            return parseUnaryOp();
        default:
            throw new SyntaxErrorException(lexer->getLineNum(), "Invalid token \""+tkn->getLexeme()+"\" found while parsing factor.");
            // Error?
    }
}

ASTTerm* Parser::parseTerm(){
    ASTFactor* factor = parseFactor();
    if(!isNextToken(MULTOPT)){
        return new ASTTerm(factor);
    }

    MultOp multOp = parseMultOp();
    ASTTerm* term = parseTerm();

    return new ASTTerm(factor, multOp, term);
}

ASTSimpleExpr* Parser::parseSimpleExpr(){
    ASTTerm* term = parseTerm();
    if(!isNextToken(ADDOPT)){
        return new ASTSimpleExpr(term);
    }

    AddOp addOp = parseAddOp();
    ASTSimpleExpr* simpleExpr = parseSimpleExpr();

    return new ASTSimpleExpr(term, addOp, simpleExpr);
}

ASTExpr* Parser::parseExpr(){
    ASTSimpleExpr* simpleExpr = parseSimpleExpr();
    if(!isNextToken(RELOPT)){
        return new ASTExpr(simpleExpr);
    }

    RelOp relOp = parseRelOp();
    ASTExpr* expr = parseExpr();

    return new ASTExpr(simpleExpr, relOp, expr);
}

ASTProgram* Parser::parseProgram(){
    while(!checkEof()){
        astTree->addStmt(parseStmt());
    }
    return astTree;
}

#endif //SMALLLANG_PARSER_H
