#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

typedef enum {
    // Identificadores e constantes
    IDENTIFIER,              // Identificador
    I_CONSTANT,              // Constante inteira (num ou char)
    F_CONSTANT,              // Constante float
    STRING_LITERAL,          // Literal de string
    FUNC_NAME,               // Nome de função

    // Operadores e símbolos simples
    SYMBOL_LPAREN,           // (
    SYMBOL_RPAREN,           // )
    SYMBOL_LBRACE,           // {
    SYMBOL_RBRACE,           // }
    SYMBOL_LBRACKET,         // [
    SYMBOL_RBRACKET,         // ]
    SYMBOL_SEMICOLON,        // ;
    SYMBOL_COMMA,            // ,
    SYMBOL_DOT,              // .
    SYMBOL_COLON,            // :
    SYMBOL_QUESTION,         // ?
    SYMBOL_PLUS,             // +
    SYMBOL_MINUS,            // -
    SYMBOL_STAR,             // *
    SYMBOL_SLASH,            // /
    SYMBOL_PERCENT,          // %
    SYMBOL_AMPERSAND,        // &
    SYMBOL_PIPE,             // |
    SYMBOL_CARET,            // ^
    SYMBOL_TILDE,            // ~
    SYMBOL_EXCLAMATION,      // !
    SYMBOL_EQUALS,           // =
    SYMBOL_LESS,             // <
    SYMBOL_GREATER,          // >
    SYMBOL_HASH,             // #

    // Operadores compostos
    PTR_OP,                  // ->
    INC_OP,                  // ++
    DEC_OP,                  // --
    LEFT_OP,                 // <<
    RIGHT_OP,                // >>
    LE_OP,                   // <=
    GE_OP,                   // >=
    EQ_OP,                   // ==
    NE_OP,                   // !=
    AND_OP,                  // &&
    OR_OP,                   // ||
    MUL_ASSIGN,              // *=
    DIV_ASSIGN,              // /=
    MOD_ASSIGN,              // %=
    ADD_ASSIGN,              // +=
    SUB_ASSIGN,              // -=
    LEFT_ASSIGN,             // <<=
    RIGHT_ASSIGN,            // >>=
    AND_ASSIGN,              // &=
    XOR_ASSIGN,              // ^=
    OR_ASSIGN,               // |=

    // Palavras-chave
    TYPEDEF, EXTERN, STATIC, AUTO, REGISTER, INLINE,
    CONST, RESTRICT, VOLATILE,
    BOOL, CHAR, SHORT, INT, LONG, SIGNED, UNSIGNED,
    FLOAT, DOUBLE, VOID, COMPLEX, IMAGINARY,
    STRUCT, UNION, ENUM, ELLIPSIS,

    CASE, DEFAULT, IF, ELSE, SWITCH, WHILE, DO, FOR,
    GOTO, CONTINUE, BREAK, RETURN, SIZEOF,

    ALIGNAS, ALIGNOF, ATOMIC, GENERIC, NORETURN, STATIC_ASSERT, THREAD_LOCAL,

    // Identificadores semânticos específicos
    TYPEDEF_NAME,            // Nome definido por typedef
    ENUMERATION_CONSTANT     // Constante de enumeração
} TokenType;


typedef struct {
    char type[24];     // Tipo do token (ex: "ID", "NUM", "SYMBOL")
    char value[64];       // Valor do token (ex: "int", "42")
    int line;             // Linha do token
    int column;           // Coluna do token
} Token;

typedef struct {
    char *data; // data do buffer
    int size; // tamanho do buffer
    int position; // posição que está do buffer
    int line_number; // número da linha do código lido
    int column; // coluna onde está o lexema atual
    Token tokens[4096]; // vetor de tokens
    int token_count; // próxima posição disponível do tokens[]
} Buffer;

char get_next_char(FILE *arquivo, Buffer *buffer);
void reload_buffer(FILE *arquivo, Buffer *buffer);
int check_next_char(char c);
void compare_lexema(Buffer *buffer);
char peek_char(FILE *arquivo);
void skip_comment(FILE *arquivo, Buffer *buffer);
void report_lexical_error(Buffer *buffer);

#endif
