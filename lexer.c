#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "funcs.h"
#include <string.h>

void lexer(Buffer *buffer, FILE *arquivo) {
    char ch;
    while ((ch = get_next_char(arquivo, buffer)) != EOF) {
        buffer->column++;
        // Ignora espaços, tabulações e novas linhas
        if (isspace(ch)) {
            if (buffer->position > 0) { // Finaliza o lexema atual
                buffer->data[buffer->position] = '\0';
                compare_lexema(buffer); // Processa o lexema
                buffer->position = 0;   // Reinicia o buffer
            }
        }
        // Detecta diretivas iniciadas por '#'
        else if (ch == '#') {
            char next_word[32]; // Para armazenar palavras como "define" ou "include"
            int i = 0;

            // Captura a próxima palavra após o '#'
            while ((ch = get_next_char(arquivo, buffer)) != EOF && isalpha(ch) && i < 31) {
                next_word[i++] = ch;
            }
            next_word[i] = '\0'; // Finaliza a string

            // Verifica se a palavra capturada é "include"
            if (strcmp(next_word, "include") == 0) {
                // Ignora o restante da linha se for #include
                while ((ch = get_next_char(arquivo, buffer)) != '\n' && ch != EOF);
            } else {
                // Processa o '#' e a palavra seguinte
                buffer->data[0] = '#';
                buffer->data[1] = '\0'; // Finaliza o lexema
                compare_lexema(buffer); // Processa o '#'
                buffer->position = 0;   // Reinicia o buffer

                // Coloca a palavra capturada no buffer e processa
                strcpy(buffer->data, next_word);
                compare_lexema(buffer);
                buffer->position = 0;   // Reinicia o buffer
            }
        }
        // Detecta strings literais
        else if (ch == '"') {
            buffer->data[buffer->position++] = ch;
            while ((ch = get_next_char(arquivo, buffer)) != EOF && ch != '"') {
                buffer->data[buffer->position++] = ch;
                if (buffer->position >= buffer->size - 1) {
                    printf("Erro: String muito longa.\n");
                    exit(1);
                }
            }
            if (ch == '"') {
                buffer->data[buffer->position++] = ch;
                buffer->data[buffer->position] = '\0';
                compare_lexema(buffer);
                buffer->position = 0;
                continue;
            } else {
                printf("Erro: String não fechada.\n");
                exit(1);
            }
        }
        // Detecta caracteres literais
        else if (ch == '\'') {
            buffer->data[buffer->position++] = ch;
            ch = get_next_char(arquivo, buffer);
            if (ch == EOF) {
                printf("ERRO LEXICO: \"%c\" INVALIDO [linha: %d], COLUNA %d\n", ch,buffer->line_number,buffer->column);
                exit(1);
            }
            buffer->data[buffer->position++] = ch;

            ch = get_next_char(arquivo, buffer);
            if (ch == '\'') {
                buffer->data[buffer->position++] = ch;
                buffer->data[buffer->position] = '\0';
                compare_lexema(buffer);
                buffer->position = 0;
                continue;
            } else {
                report_lexical_error(buffer);
                exit(1);
            }
        }
        // Detecta números (inteiros e floats)
        else if (isdigit(ch) || (ch == '.' && isdigit(peek_char(arquivo)))) {
            buffer->data[buffer->position++] = ch;
            int has_dot = (ch == '.'); // Verifica se já encontrou um ponto
            while (isdigit(peek_char(arquivo)) || (!has_dot && peek_char(arquivo) == '.')) {
                ch = get_next_char(arquivo, buffer);
                if (ch == '.') {
                    has_dot = 1; // Marca que encontrou um ponto
                }
                buffer->data[buffer->position++] = ch;
                if (buffer->position >= buffer->size - 1) {
                    printf("Erro: Número muito longo.\n");
                    exit(1);
                }
            }
            buffer->data[buffer->position] = '\0'; // Finaliza o lexema
            compare_lexema(buffer); // Processa o número
            buffer->position = 0;   // Reinicia o buffer
            continue;
        }
        // Detecta identificadores e palavras-chave
        else if (isalpha(ch) || ch == '_') {
            buffer->data[buffer->position++] = ch;
            while (isalnum(peek_char(arquivo)) || peek_char(arquivo) == '_') {
                buffer->data[buffer->position++] = get_next_char(arquivo, buffer);
                if (buffer->position >= buffer->size - 1) {
                    printf("Erro: Lexema muito longo.\n");
                    exit(1);
                }
            }
            buffer->data[buffer->position] = '\0'; // Finaliza o lexema
            compare_lexema(buffer); // Processa o identificador ou palavra-chave
            buffer->position = 0;   // Reinicia o buffer
            continue;
        }
        // Ignora comentários
        else if (ch == '/' && (peek_char(arquivo) == '/' || peek_char(arquivo) == '*')) {
            skip_comment(arquivo, buffer);
            continue;
        }
        // Detecta símbolos especiais (incluindo duplos como '==' ou '!=')
        else {
            buffer->data[0] = ch;
            buffer->data[1] = '\0';
            char next_ch = peek_char(arquivo);

            // Verifica se é um símbolo duplo
            if ((ch == '=' && next_ch == '=') || 
                (ch == '!' && next_ch == '=') || 
                (ch == '<' && next_ch == '=') || 
                (ch == '>' && next_ch == '=') || 
                (ch == '+' && next_ch == '+') || 
                (ch == '-' && next_ch == '-') || 
                (ch == '&' && next_ch == '&') || 
                (ch == '|' && next_ch == '|') || 
                (ch == '<' && next_ch == '<') || 
                (ch == '>' && next_ch == '>') || 
                (ch == '+' && next_ch == '=') || 
                (ch == '-' && next_ch == '=') || 
                (ch == '*' && next_ch == '=') || 
                (ch == '/' && next_ch == '=') || 
                (ch == '%' && next_ch == '=')) {
                buffer->data[1] = get_next_char(arquivo, buffer);
                buffer->data[2] = '\0'; // Finaliza o símbolo duplo
            }
            compare_lexema(buffer); // Processa o símbolo
            buffer->position = 0;   // Reinicia o buffer
        }
    }

    // printf("valor dos tokens:\n");
    // for(int i=0;i< 50;i++){
    //     printf("%s\n",buffer->tokens[i].type);
    // }
}