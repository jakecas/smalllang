//
// Created by Jake on 19/03/2020.
//

#ifndef SMALLLANG_LEXERTABLE_H
#define SMALLLANG_LEXERTABLE_H

#include <string>
#include <map>

using namespace std;

enum State {
    START,
    DELIM, // Whitespace
    UCMT, // Comments
    CMT, // Comments
    UMCMT, // Open ended multi-line comments
    AMCMT, // Almost finished multi-line comment (meaning found a *)
    MCMT, // Multi-line Comment
    INTEG,
    UFLOAT, // Unfinished Float
    FLOAT,
    WORD,
    ADDS, // Additive operators state: + -
    DIVS, // Division operator state: /
    MULTS, // Multiplicative operators state: *
    LTS, // Less than State
    LES, // Less than or equal to  State
    NES, // Not equal to State
    GTS, // Greater than State
    GES, // Greater than or equal to State
    EQUALS, // Equals Assignment Operator
    EQS, // Equal To Operator
    BRACES,
    SEPS, // Separator state ,:;
    APOS, // Apostrophe character
    UCHAR, // Unfinished character e.g.: 'a
    FCHAR, // Finished character (in apostrophes) e.g.: 'a'
    ERROR,
    BAD// This must always be the last element of the enum.
};

bool isFinal(State s){
    if(s == CMT || s == MCMT){
        return true;
    }
    if (s == DELIM){
        return true;
    }
    if (s == INTEG || s == FLOAT || s == WORD ||  s == FCHAR){
        return true;
    }
    if (s == ADDS || s == DIVS || s == MULTS || s == LTS || s == LES || s == NES || s == GTS || s == GES || s == EQUALS || s == EQS){
        return true;
    }
    if (s == BRACES || s == SEPS){
        return true;
    }
    return false;
}

enum Cat {
    NEWLINE,
    WS, // Any other whitespace
    DIGIT,
    PERIOD,
    CHAR,
    ADDOP, // Additive operators: + -
    DIV,
    MULT,
    LT, // Greater than relational operator: <
    GT, // Less than relational operator: <
    EQ, // Equals sign: <
    BRACE, // (){}
    SEPARATOR, //,:;
    APOSTROPHE, // '
    OTHER // This must always be the last element of the enum.
};

Cat getCat(char c){
    if(c == '\n'){
        return NEWLINE;
    }
    if(isspace(c)){
        return WS;
    }
    if(isdigit(c)){
        return DIGIT;
    }
    if(c == '.'){
        return PERIOD;
    }
    if(isalpha(c) || c == '_'){
        return CHAR;
    }
    if(c == '+' || c == '-'){
        return ADDOP;
    }
    if(c == '/'){
        return DIV;
    }
    if(c == '*'){
        return MULT;
    }
    if(c == '<'){
        return LT;
    }
    if(c == '>'){
        return GT;
    }
    if(c == '='){
        return EQ;
    }
    if(c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']'){
        return BRACE;
    }
    if(c == ',' || c == ':' || c == ';'){
        return SEPARATOR;
    }
    if(c == '\''){
        return APOSTROPHE;
    }
    return OTHER;
}

// Using OTHER+1 and BAD for the size since they are the last element of the enum (and BAD has no transitions).
// i.e. To facilitate adding states/classifiers.
State TX[OTHER+1][BAD] = {
        //START,DELIM, UCMT,  CMT,   UMCMT, AMCMT, MCMT,  INTEG, UFLOAT,FLOAT, WORD,  ADDS,  DIVS,  MULTS, LTS,   LES,   NES,   GTS,   GES,   EQUALS,EQS,   BRACES, SEPS, APOS,  UCHAR, FCHAR  ERROR
        {DELIM, DELIM, CMT,   ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // NEWLINE
        {DELIM, DELIM, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // WS
        {INTEG, ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, INTEG, FLOAT, FLOAT, WORD,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // DIGIT
        {ERROR, ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, UFLOAT,ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // PERIOD
        {WORD,  ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, WORD,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // CHAR
        {ADDS,  ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // ADDOP
        {DIVS,  ERROR, UCMT,  ERROR, UMCMT, MCMT,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCMT,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // DIV
        {MULTS, ERROR, UCMT,  ERROR, AMCMT, AMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // MULT
        {LTS,   ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // LT
        {GTS,   ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, NES,   ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // GT
        {EQUALS,ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, LES,   ERROR, ERROR, GES,   ERROR, EQS,   ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // EQ
        {BRACES,ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // BRACE
        {SEPS,  ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR}, // SEPARATOR
        {APOS,  ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, FCHAR, ERROR, ERROR}, // APOSTROPHE
        {ERROR, ERROR, UCMT,  ERROR, UMCMT, UMCMT, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, UCHAR, ERROR, ERROR, ERROR} // OTHER
};

enum Type {
    TYPET, // Datatype token keywords float, int, bool
    AUTOK, // Auto type keyword
    BOOLL, // Boolean literal, true/false
    CHARL, // Character literal, a character enclosed in apostrophes
    INTL, // Integer literal
    FLOATL, // Float literal
    IDT, // Identifier, i.e. unreserved word
    MULTOPT, // Multiplicative operators * /  and
    ADDOPT, // Additive operators token + - or
    RELOPT, // Relational operators token < > == <> <= >,=
    NOTK, // "not" unary operator eyword
    LETK, // "let" keyword
    PRINTK, // "print" keyword
    RETURNK, // "return" keyword
    IFK, // "if" keyword
    ELSEK, // "else" keyword
    FORK, // "for" keyword
    WHILEK, // "while" keyword
    FUNCK, // "ff" keyword
    EQUALSL, // '=' literal
    OPENROUND, // '(' literal
    CLOSEROUND, // ')' literal
    OPENCURLY, // '{' literal
    CLOSECURLY, // '}' literal
    OPENSQUARE, // '[' literal
    CLOSESQUARE, // ']' literal
    COMMAL, // ',' literal
    CLNL, // ':' literal
    SEMICLNL, // ';' literal
    SKIP // Token to skip, namely whitespace and comments.
};

class Token {
private:
    Type type;
    string lexeme;
public:
    Token(Type type, string lexeme){
        this->type = type;
        this->lexeme = lexeme;
    }
    Token(Type type){
        this->type = type;
    }
    Type getType(){
        return type;
    }
    string getLexeme(){
        return lexeme;
    }
};

map<string, Type> keywords = {{"float", TYPET}, {"int", TYPET}, {"bool", TYPET},
                                  {"char", TYPET},
                                  {"auto", AUTOK}, {"true", BOOLL}, {"false", BOOLL},
                                  {"and", MULTOPT}, {"or", ADDOPT}, {"not", NOTK},
                                  {"let", LETK}, {"print", PRINTK}, {"return", RETURNK},
                                  {"if", IFK}, {"else", ELSEK},
                                  {"for", FORK}, {"while", WHILEK},
                                  {"ff", FUNCK}};

bool isKeyword(string s){
    return keywords.count(s) != 0;
}

Type findType(string lexeme, State state){
    switch(state){
        case FLOAT:
            return FLOATL;
        case INTEG:
            return INTL;
        case WORD:
            if(isKeyword(lexeme)){
                return keywords.find(lexeme)->second;
            }
            return IDT;
        case MULTS:
        case DIVS:
            return MULTOPT;
        case ADDS:
            return ADDOPT;
        case LTS:
        case LES:
        case NES:
        case GTS:
        case GES:
        case EQS:
            return RELOPT;
        case EQUALS:
            return EQUALSL;
        case BRACES:
            if(lexeme.compare("(") == 0){
                return OPENROUND;
            } else if(lexeme.compare(")") == 0){
                return CLOSEROUND;
            } else if(lexeme.compare("{") == 0){
                return OPENCURLY;
            } else if(lexeme.compare("}") == 0){
                return CLOSECURLY;
            } else if(lexeme.compare("[") == 0){
                return OPENSQUARE;
            } else {
                return CLOSESQUARE;
            }
        case SEPS:
            if(lexeme.compare(",") == 0){
                return COMMAL;
            } else if(lexeme.compare(":") == 0) {
                return CLNL;
            }else {
                return SEMICLNL;
            }
        case FCHAR:
            return CHARL;
        default:
            return SKIP;
    }
}

#endif //SMALLLANG_LEXERTABLE_H
