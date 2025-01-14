#include "funcs.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char get_next_char(FILE *arquivo, Buffer *buffer) {
  char ch = fgetc(arquivo); // Lê o próximo caractere do arquivo

  if (ch == '\n') {
    buffer->line_number++; // Atualiza o número da linha ao encontrar '\n'
    buffer->column = 0; // o valor da coluna volta para zero
  }

  return ch; // Retorna o caractere lido
}

typedef struct {
    const char *lexeme;       // O lexema (palavra ou símbolo)
    TokenType token;          // Tipo do token
    const char *token_name;   // Nome textual do token
} LexemeTokenMap;

const LexemeTokenMap lexeme_token_map[] = {
    // Palavras-chave
    {"auto", AUTO, "AUTO"}, {"break", BREAK, "BREAK"}, {"case", CASE, "CASE"}, 
    {"char", CHAR, "CHAR"}, {"const", CONST, "CONST"}, {"continue", CONTINUE, "CONTINUE"},
    {"default", DEFAULT, "DEFAULT"}, {"do", DO, "DO"}, {"double", DOUBLE, "DOUBLE"},
    {"else", ELSE, "ELSE"}, {"enum", ENUM, "ENUM"}, {"extern", EXTERN, "EXTERN"},
    {"float", FLOAT, "FLOAT"}, {"for", FOR, "FOR"}, {"goto", GOTO, "GOTO"},
    {"if", IF, "IF"}, {"int", INT, "INT"}, {"long", LONG, "LONG"},
    {"register", REGISTER, "REGISTER"}, {"return", RETURN, "RETURN"}, 
    {"short", SHORT, "SHORT"}, {"signed", SIGNED, "SIGNED"}, 
    {"sizeof", SIZEOF, "SIZEOF"}, {"static", STATIC, "STATIC"}, 
    {"struct", STRUCT, "STRUCT"}, {"switch", SWITCH, "SWITCH"}, 
    {"typedef", TYPEDEF, "TYPEDEF"}, {"union", UNION, "UNION"},
    {"unsigned", UNSIGNED, "UNSIGNED"}, {"void", VOID, "VOID"}, 
    {"volatile", VOLATILE, "VOLATILE"}, {"while", WHILE, "WHILE"},
    {"inline", INLINE, "INLINE"}, {"_Alignas", ALIGNAS, "ALIGNAS"},
    {"_Alignof", ALIGNOF, "ALIGNOF"}, {"_Atomic", ATOMIC, "ATOMIC"},
    {"_Bool", BOOL, "BOOL"}, {"_Complex", COMPLEX, "COMPLEX"},
    {"_Generic", GENERIC, "GENERIC"}, {"_Imaginary", IMAGINARY, "IMAGINARY"},
    {"_Noreturn", NORETURN, "NORETURN"}, {"_Static_assert", STATIC_ASSERT, "STATIC_ASSERT"},
    {"_Thread_local", THREAD_LOCAL, "THREAD_LOCAL"},

    // Símbolos simples
    {";", SYMBOL_SEMICOLON, "SYMBOL_SEMICOLON"}, 
    {":", SYMBOL_COLON, "SYMBOL_COLON"},
    {",", SYMBOL_COMMA, "SYMBOL_COMMA"}, 
    {".", SYMBOL_DOT, "SYMBOL_DOT"}, 
    {"=", SYMBOL_EQUALS, "SYMBOL_EQUALS"}, 
    {"+", SYMBOL_PLUS, "SYMBOL_PLUS"}, 
    {"-", SYMBOL_MINUS, "SYMBOL_MINUS"}, 
    {"*", SYMBOL_STAR, "SYMBOL_STAR"}, 
    {"/", SYMBOL_SLASH, "SYMBOL_SLASH"}, 
    {"<", SYMBOL_LESS, "SYMBOL_LESS"}, 
    {">", SYMBOL_GREATER, "SYMBOL_GREATER"}, 
    {"(", SYMBOL_LPAREN, "SYMBOL_LPAREN"}, 
    {")", SYMBOL_RPAREN, "SYMBOL_RPAREN"}, 
    {"{", SYMBOL_LBRACE, "SYMBOL_LBRACE"}, 
    {"}", SYMBOL_RBRACE, "SYMBOL_RBRACE"}, 
    {"[", SYMBOL_LBRACKET, "SYMBOL_LBRACKET"},
    {"]", SYMBOL_RBRACKET, "SYMBOL_RBRACKET"},
    {"#", SYMBOL_HASH, "SYMBOL_HASH"}, 
    {"!", SYMBOL_EXCLAMATION, "SYMBOL_EXCLAMATION"},
    {"%", SYMBOL_PERCENT, "SYMBOL_PERCENT"},
    {"&", SYMBOL_AMPERSAND, "SYMBOL_AMPERSAND"},
    {"|", SYMBOL_PIPE, "SYMBOL_PIPE"},
    {"^", SYMBOL_CARET, "SYMBOL_CARET"},
    {"~", SYMBOL_TILDE, "SYMBOL_TILDE"},
    {"?", SYMBOL_QUESTION, "SYMBOL_QUESTION"},

    // Símbolos compostos
    {"==", EQ_OP, "DOUBLE_EQUALS"},
    {"!=", NE_OP, "NOT_EQUALS"},
    {">=", GE_OP, "GE_OP"},
    {"<=", LE_OP, "LE_OP"},
    {"&&", AND_OP, "AND_OP"},
    {"||", OR_OP, "OR_OP"},
    {"<<", LEFT_OP, "LEFT_OP"},
    {">>", RIGHT_OP, "RIGHT_OP"},
    {"++", INC_OP, "INC_OP"},
    {"--", DEC_OP, "DEC_OP"},
    {"->", PTR_OP, "PTR_OP"},
    {"+=", ADD_ASSIGN, "ADD_ASSIGN"},
    {"-=", SUB_ASSIGN, "SUB_ASSIGN"},
    {"*=", MUL_ASSIGN, "MUL_ASSIGN"},
    {"/=", DIV_ASSIGN, "DIV_ASSIGN"},
    {"%=", MOD_ASSIGN, "MOD_ASSIGN"},
    {"<<=", LEFT_ASSIGN, "LEFT_ASSIGN"},
    {">>=", RIGHT_ASSIGN, "RIGHT_ASSIGN"},
    {"&=", AND_ASSIGN, "AND_ASSIGN"},
    {"^=", XOR_ASSIGN, "XOR_ASSIGN"},
    {"|=", OR_ASSIGN, "OR_ASSIGN"},
};


const int lexeme_token_map_size = sizeof(lexeme_token_map) / sizeof(lexeme_token_map[0]);

// Busca na tabela unificada
const char* find_token_name(const char *lexeme) {
    for (int i = 0; i < lexeme_token_map_size; i++) {
        if (strcmp(lexeme, lexeme_token_map[i].lexeme) == 0) {
            return lexeme_token_map[i].token_name; // Retorna o nome do token
        }
    }
    return "IDENTIFIER"; // Se não encontrar, assume que é um identificador
}

void store_token(Token *token, Buffer *buffer, const char *token_name) {
    strncpy(token->type, token_name, sizeof(token->type) - 1);
    token->type[sizeof(token->type) - 1] = '\0';

    strncpy(token->value,buffer->data, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';

    token->line = buffer->line_number;
    token->column = buffer->column;
    buffer->token_count++;
}

// Função para comparar lexemas
void compare_lexema(Buffer *buffer) {
    // Busca o token correspondente
    const char *token_name = find_token_name(buffer->data);
    if (strcmp(token_name, "IDENTIFIER") != 0) { // Se encontrou na tabela de palavras reservadas
        printf("<%s> \"%s\" [linha: %d]\n", token_name, buffer->data, buffer->line_number);
        store_token(&buffer->tokens[buffer->token_count], buffer, token_name);
        return;
    }

    // Verifica se é um número (com ou sem ponto flutuante)
    int is_float = 0, is_number = 1;
    for (int i = 0; buffer->data[i] != '\0'; i++) {
        if (!isdigit(buffer->data[i])) {
            if (buffer->data[i] == '.' && is_number && i > 0 && isdigit(buffer->data[i - 1])) {
                is_float = 1; // Contém um ponto válido
            } else {
                is_number = 0; // Não é um número válido
                break;
            }
        }
    }
    if (is_number) {
        token_name = is_float ? "F_CONSTANT" : "I_CONSTANT";
        printf("<%s> \"%s\" [linha: %d]\n", token_name, buffer->data, buffer->line_number);
        store_token(&buffer->tokens[buffer->token_count], buffer, token_name);
        return;
    }

    // Verifica se é um identificador válido
    if (isalpha(buffer->data[0]) || buffer->data[0] == '_') {
        for (int i = 1; buffer->data[i] != '\0'; i++) {
            if (!isalnum(buffer->data[i]) && buffer->data[i] != '_') {
                // Não é um identificador válido
                report_lexical_error(buffer);
                return;
            }
        }
        // É um identificador válido
        printf("<IDENTIFIER> \"%s\" [linha: %d]\n", buffer->data, buffer->line_number);
        store_token(&buffer->tokens[buffer->token_count], buffer, "IDENTIFIER");
        return;
    }

    // Verifica se é uma string literal
    if (buffer->data[0] == '"' && buffer->data[strlen(buffer->data) - 1] == '"') {
        printf("<STRING_LITERAL> \"%s\" [linha: %d]\n", buffer->data, buffer->line_number);
        store_token(&buffer->tokens[buffer->token_count], buffer, "STRING_LITERAL");
        return;
    }
    
    // verifica se é um char literal
    if (buffer->data[0] == '\'' && buffer->data[strlen(buffer->data) - 1] == '\'') {
        printf("<I_CONSTANT> \"%s\" [linha: %d]\n", buffer->data, buffer->line_number);
        store_token(&buffer->tokens[buffer->token_count], buffer, "I_CONSTANT");
        return;
    }

    // Caso contrário, trata como erro genérico
    report_lexical_error(buffer);
}


char peek_char(FILE *arquivo) {
    char ch = fgetc(arquivo);
    ungetc(ch, arquivo); // Devolve o caractere ao fluxo
    return ch;
}

void skip_comment(FILE *arquivo, Buffer *buffer) {
    char ch = get_next_char(arquivo, buffer);
    if (ch == '/') { // Comentário de linha
        while ((ch = get_next_char(arquivo, buffer)) != '\n' && ch != EOF);
    } else if (ch == '*') { // Comentário de bloco
        while (1) {
            ch = get_next_char(arquivo, buffer);
            if (ch == '*' && peek_char(arquivo) == '/') {
                get_next_char(arquivo, buffer); // Consome '/'
                break;
            }
            if (ch == EOF) {
                fprintf(stderr, "ERRO LEXICO: Comentário de bloco não terminado [linha: %d], COLUNA %d.\n", buffer->line_number, buffer->column);
                exit(1);
            }
        }
    }
}

void report_lexical_error(Buffer *buffer) {
    printf("ERRO LEXICO: \"%s\" INVALIDO [linha: %d], COLUNA %d\n", buffer->data,buffer->line_number,buffer->column);
    exit(1);
}
