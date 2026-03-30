/* 

    FIX THIS STUPID ASS LEXER ASAP
    its breaking when i try to tokenize a string

*/

#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Includes/errors.h"
#include "Includes/lexer.h"

Keyword keywords[] = {
    {"psh",  TOKEN_PUSH},
    {"pop",  TOKEN_POP},
    {"jmp", TOKEN_JUMP},
    {"log",  TOKEN_LOG},
    {"add",  TOKEN_ADD},
    {"sub",  TOKEN_SUB},
    {"mul",  TOKEN_MUL},
    {"div",  TOKEN_DIV},
    {"and",  TOKEN_AND},
    {"or",   TOKEN_OR}, 
    {"nor",  TOKEN_NOR},
    {"nand", TOKEN_NAND},
    {"not",  TOKEN_NOT},
    {"xor",  TOKEN_XOR},
};

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(Keyword))

static int is_at_end(Lexer* lexer) { return *lexer->current == '\0'; }
static char advance(Lexer* lexer) { lexer->current++; return lexer->current[-1]; }
static char peek(Lexer* lexer) { return *lexer->current; }
static char peek_next(Lexer* lexer) { if (is_at_end(lexer)) return '\0'; return lexer->current[1]; }

static int match(Lexer* lexer, char expected) {
    if (is_at_end(lexer)) return 0;
    if (*lexer->current != expected) return 0;
    lexer->current++;
    return 1;
}

static Token make_token(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.value = 0;
    return token;
}

static void skip_whitespace(Lexer* lexer) {
    while (1) {
        char c = peek(lexer);

        if (c == ' ' || c == '\t' || c == '\r') {
            advance(lexer);
        }
        else if (c == '/' && peek_next(lexer) == '*') {
            advance(lexer); advance(lexer);

            while (!(peek(lexer) == '*' && peek_next(lexer) == '/')) {
                if (is_at_end(lexer)) return;
                advance(lexer);
            }

            advance(lexer); advance(lexer);
        }
        else {
            break;
        }
    }
}

static Token make_number(Lexer* lexer) {
    TokenType type = TOKEN_NUMBER;

    if (isalpha(peek(lexer))) {
        type = TOKEN_IDENTIFIER;
    }

    while (isalpha(peek(lexer)) || isdigit(peek(lexer))) {
        if (!isdigit(peek(lexer))) { /* Handle wrong number format */
            type = TOKEN_IDENTIFIER;
        }

        advance(lexer);
    }

    Token token = make_token(lexer, type);
    if (type == TOKEN_NUMBER) {
        token.value = strtol(token.start, NULL, 10);
    }

    return token;
}

static Token make_string(Lexer* lexer) {
    while (!is_at_end(lexer) && peek(lexer) != '"') {
        if (peek(lexer) == '\n') {
            printf("Lexer error: newline in string.");
            return make_token(lexer, TOKEN_INVALID_SYNTAX);
        }
        advance(lexer);
    }

    if (is_at_end(lexer)) {
        printf("Lexer error: unclosed string.");
        exit(ERR_UNCLOSED_STRING);
    }

    Token token = make_token(lexer, TOKEN_STRING);

    advance(lexer);
    
    token.start++; 
    token.length--; 

    return token;
}

static Token check_keyword(Lexer* lexer) {
    while (isalnum(peek(lexer)) || peek(lexer) == '_' && !(peek(lexer) == '"')) advance(lexer);

    uint64_t length = (uint64_t)(lexer->current - lexer->start);

    for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (strlen(keywords[i].name) == length && strncmp(lexer->start, keywords[i].name, length) == 0) {
            return make_token(lexer, keywords[i].type);
        }
    }

    return make_token(lexer, TOKEN_IDENTIFIER);
}

void vl_lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 0;
}

Token vl_lexer_next(Lexer* lexer) {
    skip_whitespace(lexer);

    lexer->start = lexer->current;

    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }
        
    char c = advance(lexer);

    if (isalpha(c) || c == '_' || c == '@')
        return check_keyword(lexer);

    if (isdigit(c))
        return make_number(lexer);

    switch (c) {
        case '\n': return make_token(lexer, TOKEN_EOL);
        case ';': return make_token(lexer, TOKEN_EOL);
        case '.': return make_token(lexer, TOKEN_PERIOD);
        case '>': return make_token(lexer, TOKEN_GT);
        case '<': return make_token(lexer, TOKEN_LT);
        case '=': return make_token(lexer, TOKEN_EQ);
        case '!': {
            if (match(lexer, '=')) return make_token(lexer, TOKEN_NEQ);
            return make_token(lexer, TOKEN_NOT);
        }
    }

    return make_token(lexer, TOKEN_EOF);
}
