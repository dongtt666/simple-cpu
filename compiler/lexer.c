#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "lexer.h"

struct keyword {
    const char *key;
    int val;
};

struct keyword s_keywords[] = {
    /* control key words */
    { "if",     TOK_IF      },
    { "else",   TOK_ELSE    },
    { "while",  TOK_WHILE   },
    { "return", TOK_RETURN  },
    { "for",    TOK_FOR     },
    { "do",     TOK_DO      },
    { "break",  TOK_BREAK   },
    { "continue",TOK_CONTINUE},

    /* data type key words */
    { "int",    TOK_INT     },
    { "float",  TOK_FLOAT   },
    { "char",   TOK_CHAR    },
    { "void",   TOK_VOID    },
    { "bool",   TOK_BOOL    },

    /* input/output key words */
    { "print",  TOK_PRINT   },
    { "input",  TOK_INPUT   },

    /* double character operators */
    { "==",     TOK_EQ          },
    { "!=",     TOK_NE          },
    { "<=",     TOK_LE          },
    { ">=",     TOK_GE          },
    { "&&",     TOK_LOGICAL_AND },
    { "||",     TOK_LOGICAL_OR  },
    { "<<",     TOK_SHL         },
    { ">>",     TOK_SHR         },
};

static const char *lexer_tok2key(token_type_t tok)
{
	unsigned i;

	for (i = 0; i < sizeof(s_keywords) / sizeof(s_keywords[0]); ++i) {
		if (s_keywords[i].val == tok)
			return s_keywords[i].key;
	}

	return NULL;
}

static token_type_t lexer_key2tok(const char *key)
{
    unsigned i;

    for (i = 0; i < sizeof(s_keywords) / sizeof(s_keywords[0]); ++i) {
        if (strcmp(s_keywords[i].key, key) == 0)
            return s_keywords[i].val;
    }

    return __TOK_ERROR;
}

struct lexer_st* lexer_create(const char *source)
{
    struct lexer_st *lexer = malloc(sizeof(struct lexer_st));
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
    lexer->tokens = malloc(MAX_TOKENS * sizeof(struct token_st));
    return lexer;
}

void lexer_destroy(struct lexer_st *lexer)
{
    free(lexer->tokens);
    free(lexer);
}

static void lexer_add_token(struct lexer_st *lexer, token_type_t type, const char *value)
{
    struct token_st *token = &lexer->tokens[lexer->token_count++];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    if (value) strcpy(token->value, value);
}

int lexer_tokenize(struct lexer_st *lexer)
{
    while (lexer->source[lexer->position]) {
        char c = lexer->source[lexer->position];

        if (isspace(c)) {
            if (c == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }

        if (isalpha(c)) {
            // 标识符或关键字
            char buffer[MAX_TOKEN_VALUE_LEN] = {0};
            int i = 0;
            while (isalnum(lexer->source[lexer->position])) {
                buffer[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            buffer[i] = '\0';

            token_type_t tok_type = lexer_key2tok(buffer);
            if (tok_type != __TOK_ERROR) {
                lexer_add_token(lexer, tok_type, buffer);
            } else {
                lexer_add_token(lexer, TOK_IDENTIFIER, buffer);
            }

            continue;
        }

        if (isdigit(c)) {
            // 数字
            char buffer[MAX_TOKEN_VALUE_LEN] = {0};
            int i = 0;
            while (isdigit(lexer->source[lexer->position])) {
                buffer[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            buffer[i] = '\0';
            lexer_add_token(lexer, TOK_NUMBER, buffer);
            continue;
        }

         /* 双目运算符和双字符token */
        if (lexer->source[lexer->position + 1]) {
            char two_chars[3] = {c, lexer->source[lexer->position + 1], '\0'};
            token_type_t tok_type = lexer_key2tok(two_chars);
            if (tok_type != __TOK_ERROR) {
                lexer_add_token(lexer, tok_type, two_chars);
                lexer->position += 2;
                lexer->column += 2;
                continue;
            }
        }

        // 单字符token
        switch (c) {
            /* 算术运算符 */
            case '+': lexer_add_token(lexer, TOK_PLUS, "+"); break;
            case '-': lexer_add_token(lexer, TOK_MINUS, "-"); break;
            case '*': lexer_add_token(lexer, TOK_MULTIPLY, "*"); break;
            case '/': lexer_add_token(lexer, TOK_DIVIDE, "/"); break;
            case '%': lexer_add_token(lexer, TOK_MODULO, "%"); break;
            /* 比较运算符 */
            case '<': lexer_add_token(lexer, TOK_LT, "<"); break;
            case '>': lexer_add_token(lexer, TOK_GT, ">"); break;
            /* 位运算符 */
            case '&': lexer_add_token(lexer, TOK_BIT_AND, "&"); break;
            case '|': lexer_add_token(lexer, TOK_BIT_OR, "|"); break;
            case '^': lexer_add_token(lexer, TOK_BIT_XOR, "^"); break;
            case '~': lexer_add_token(lexer, TOK_BIT_NOT, "~"); break;
            /* 逻辑运算符 */
            case '!': lexer_add_token(lexer, TOK_LOGICAL_NOT, "!"); break;
            /* 赋值运算符 */
            case '=': lexer_add_token(lexer, TOK_ASSIGN, "="); break;
            /* 分隔符 */
            case ';': lexer_add_token(lexer, TOK_SEMICOLON, ";"); break;
            case ',': lexer_add_token(lexer, TOK_COMMA, ","); break;
            case ':': lexer_add_token(lexer, TOK_COLON, ":"); break;
            case '?': lexer_add_token(lexer, TOK_QUESTION, "?"); break;
            /* 括号 */
            case '(': lexer_add_token(lexer, TOK_LPAREN, "("); break;
            case ')': lexer_add_token(lexer, TOK_RPAREN, ")"); break;
            case '{': lexer_add_token(lexer, TOK_LBRACE, "{"); break;
            case '}': lexer_add_token(lexer, TOK_RBRACE, "}"); break;
            case '[': lexer_add_token(lexer, TOK_LBRACKET, "["); break;
            case ']': lexer_add_token(lexer, TOK_RBRACKET, "]"); break;
            default:
                printf("Line %d, Column %d: Unknown character: %c\n", lexer->line, lexer->column, c);
                return -1;
        }
        lexer->position++;
        lexer->column++;
    }
    lexer_add_token(lexer, TOK_EOF, token_desc(TOK_EOF));
    return 0;
}

void lexer_print_tokens(const struct lexer_st *lexer)
{
    printf("=== Lexical Analysis Result ===\n");
    for (int i = 0; i < lexer->token_count; i++) {
        printf("Line %2d, Column %2d: %-12s '%s'\n",
               lexer->tokens[i].line, lexer->tokens[i].column,
               token_desc(lexer->tokens[i].type), lexer->tokens[i].value);
    }
    printf("Total %d tokens\n\n", lexer->token_count);
}
