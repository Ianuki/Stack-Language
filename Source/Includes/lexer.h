#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_EOL,

    TOKEN_INVALID_SYNTAX,

    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,

    TOKEN_PUSH,
    TOKEN_POP,
    TOKEN_JUMP,
    TOKEN_LOG,

    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOR,
    TOKEN_NAND,
    TOKEN_NOT,
    TOKEN_XOR,

    TOKEN_PERIOD,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_EQ,
    TOKEN_NEQ,
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    uint64_t length;
    uint64_t value;
} Token;

typedef struct {
    const char* name;
    TokenType type;
} Keyword;

typedef struct {
    const char* start;
    const char* current;
    uint64_t line;
} Lexer;

void vl_lexer_init(Lexer*, const char*);
Token vl_lexer_next(Lexer*);

#endif /* LEXER_H */
