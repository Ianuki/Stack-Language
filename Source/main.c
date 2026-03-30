/*

    A string is an array of characters that ends at a null character
    Not every array of characters that has a null character is a string
    An array of characters that does not have a null character will never be a string

    I havent figured out what a character is yet

*/

/*

    Ts code sucks ahh
    i wanted to make a stack based "language" asap just for fun and i used the
    same lexer im using for vilang

    i [[[might]]] have broken a few ABI rules or wtf that is so dont blame me if it bricks your device
    (YOUVE BEEN WARNED)
    nobody gonna run ts so whatever anyway who gaf
    i did pay attention to calling conventions such as stack alignment so yeah tbh who needs more than that

*/

/*

    Im being fr this code is shit dont take this as serious work

    i make professional grade organized linus torvalds IBM nasa (stack only, ball knowers will get it) gentoo
    kernel michelin star ultra optimization cache hit bloat free code all the time
    THIS is an exception so PLEASE dear recruiter who took their sweet time to look at my projects dont take this code seriously

    i MIGHT break this into different files but still just because it has a gitignore doesnt mean its a serious project

*/

/*

    >>> NOT PRODUCTION CODE <<<

*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "Includes/utilities.h"
#include "Includes/lexer.h"
#include "Includes/errors.h"
  
char* read_file_to_string(const char* file_name) {
    FILE * file = fopen(file_name, "rb");

    if (!file) { printf("Failed to open file."); return NULL; }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    
    char* source_code = (char*)malloc(size + 1);

    if (source_code == NULL) {
        printf("Malloc error.");
        fclose(file);

        return NULL;
    }

    source_code[size] = (char)0;

    fread(source_code, 1, size, file);
    fclose(file);

    return source_code;
}

int main(int argc, char** argv) {
    if (argc < 2) { printf("Missing arguments.\n"); return 1; }

    char* source_file_name = argv[1];
    char* source_code = read_file_to_string(source_file_name);

    Lexer lexer; vl_lexer_init(&lexer, source_code);

    int64_t stack_depth = 0;

    Token token = vl_lexer_next(&lexer);
    while (token.type != TOKEN_EOF) {
        switch (token.type) {
            case TOKEN_PERIOD: {
                token = vl_lexer_next(&lexer);
                if (token.type != TOKEN_IDENTIFIER) exit(ERR_EXPECTED_GOT);

                printf("\n%.*s:", token.length, token.start);
            } break;  

            case TOKEN_LOG: {
                if (stack_depth % 2 == 0) {
                    printf("\n\tsub rsp, 8");
                    printf("\n\tmov rsi, %d", stack_depth * 8);
                    printf("\n\tcall log");
                    printf("\n\tadd rsp, 8");
                } else {
                    printf("\n\tmov rsi, %d", stack_depth * 8);
                    printf("\n\tcall log");
                }
            } break;  

            case TOKEN_PUSH: {
                token = vl_lexer_next(&lexer);
                if (token.type != TOKEN_NUMBER) exit(ERR_EXPECTED_GOT);

                printf("\n\tmov rax, %llu", (unsigned long long)token.value);
                printf("\n\tpush rax");
                stack_depth++;
            } break;    

            case TOKEN_POP: {
                printf("\n\tpop rax");
                stack_depth--;
            } break;

            case TOKEN_JUMP: { /* i'll fix this bs later. i just want to get ts working */
                token = vl_lexer_next(&lexer);

                switch (token.type) {
                    case TOKEN_GT: {
                        Token value = vl_lexer_next(&lexer);
                        if (value.type != TOKEN_NUMBER) exit(ERR_INVALID_SYNTAX);
                        Token label = vl_lexer_next(&lexer);
                        if (label.type != TOKEN_IDENTIFIER) exit(ERR_INVALID_SYNTAX);
                        printf("\n\tmov al, byte ptr [rsp]");
                        printf("\n\tcmp al, %u", value.value);
                        printf("\n\tjg %.*s", label.length, label.start);
                    } break;

                    case TOKEN_LT: {
                        Token value = vl_lexer_next(&lexer);
                        if (value.type != TOKEN_NUMBER) exit(ERR_INVALID_SYNTAX);
                        Token label = vl_lexer_next(&lexer);
                        if (label.type != TOKEN_IDENTIFIER) exit(ERR_INVALID_SYNTAX);
                        printf("\n\tmov al, byte ptr [rsp]");
                        printf("\n\tcmp al, %u", value.value);
                        printf("\n\tjl %.*s", label.length, label.start);
                    } break;

                    case TOKEN_EQ: {
                        Token value = vl_lexer_next(&lexer);
                        if (value.type != TOKEN_NUMBER) exit(ERR_INVALID_SYNTAX);
                        Token label = vl_lexer_next(&lexer);
                        if (label.type != TOKEN_IDENTIFIER) exit(ERR_INVALID_SYNTAX);
                        printf("\n\tmov al, byte ptr [rsp]");
                        printf("\n\tcmp al, %u", value.value);
                        printf("\n\tje %.*s", label.length, label.start);
                    } break;

                    case TOKEN_NEQ: {
                        Token value = vl_lexer_next(&lexer);
                        if (value.type != TOKEN_NUMBER) exit(ERR_INVALID_SYNTAX);
                        Token label = vl_lexer_next(&lexer);
                        if (label.type != TOKEN_IDENTIFIER) exit(ERR_INVALID_SYNTAX);
                        printf("\n\tcmp byte ptr [rsp], %u", value.value);
                        printf("\n\tjne %.*s", label.length, label.start);
                    } break;

                    case TOKEN_IDENTIFIER: { /* Unconditional jump */
                        printf("\n\tjmp %.*s", token.length, token.start);
                    }
                }
            } break; 

            case TOKEN_ADD: {
                printf("\n\tpop rbx");
                printf("\n\tpop rax");
                printf("\n\tadd al, bl");
                printf("\n\tmovzx rax, al");
                printf("\n\tpush rax");
                stack_depth--;
            } break;

            case TOKEN_SUB: {
                printf("\n\tpop rbx");
                printf("\n\tpop rax");
                printf("\n\tsub al, bl");
                printf("\n\tmovzx rax, al");
                printf("\n\tpush rax");
                stack_depth--;
            } break;
        }

        token = vl_lexer_next(&lexer);
    }


    free(source_code);
}