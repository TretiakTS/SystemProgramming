#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef enum {
    START,
    IN_IDENTIFIER,
    IN_DECIMAL_NUMBER,
    IN_FLOAT_NUMBER,
    IN_STRING_CONST,
    IN_CHAR_CONST,
    IN_COMMENT,
    DONE
} StateType;

typedef enum {
    RESERVED_WORD,
    IDENTIFIER,
    SEPARATOR,
    OPERATOR,
    DECIMAL_NUMBER,
    FLOAT_NUMBER,
    STRING_CONST,
    CHAR_CONST,
    COMMENT,
    ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[256];
} Token;

const char* RESERVED_WORDS[] = {"using", "class", "static", "void", "string", "int", "float", "char", "is", NULL};

Token getNextToken(const char** src) {
    StateType state = START;
    Token token = {ERROR, {0}};
    const char* s = *src;
    int valIndex = 0;

    while (state != DONE) {
        char c = *s++;

        switch (state) {
            case START:
                if (isspace(c)) {
                    continue;
                } else if (isalpha(c) || c == '_') {
                    state = IN_IDENTIFIER;
                    token.value[valIndex++] = c;
                } else if (isdigit(c)) {
                    state = IN_DECIMAL_NUMBER;
                    token.value[valIndex++] = c;
                } else if (c == '"') {
                    state = IN_STRING_CONST;
                } else if (c == '\'') {
                    state = IN_CHAR_CONST;
                } else if (c == '/' && *s == '/') {
                    state = IN_COMMENT;
                    s++;
                } else if (strchr("=<>!+-*/%", c)) {
                    token.type = OPERATOR;
                    token.value[valIndex++] = c;
                    state = DONE;
                } else if (strchr(";,.[](){}", c)) {
                    token.type = SEPARATOR;
                    token.value[valIndex++] = c;
                    state = DONE;
                } else {
                    token.type = ERROR;
                    token.value[valIndex++] = c;
                    state = DONE;
                }
                break;

            case IN_IDENTIFIER:
                if (isalnum(c) || c == '_') {
                    token.value[valIndex++] = c;
                } else {
                    token.type = IDENTIFIER;
                    for (int i = 0; RESERVED_WORDS[i] != NULL; i++) {
                        if (strcmp(RESERVED_WORDS[i], token.value) == 0) {
                            token.type = RESERVED_WORD;
                            break;
                        }
                    }
                    s--;  // Push back the non-identifier character.
                    state = DONE;
                }
                break;

            case IN_DECIMAL_NUMBER:
                if (isdigit(c)) {
                    token.value[valIndex++] = c;
                } else if (c == '.') {
                    state = IN_FLOAT_NUMBER;
                    token.value[valIndex++] = c;
                } else {
                    token.type = DECIMAL_NUMBER;
                    s--;  // Push back the non-digit character.
                    state = DONE;
                }
                break;

            case IN_FLOAT_NUMBER:
                if (isdigit(c)) {
                    token.value[valIndex++] = c;
                } else if (tolower(c) == 'f') {
                    token.value[valIndex++] = c;
                    token.type = FLOAT_NUMBER;
                    state = DONE;
                } else {
                    token.type = ERROR;  // Invalid float format.
                    state = DONE;
                }
                break;

            case IN_STRING_CONST:
                if (c == '"') {
                    token.type = STRING_CONST;
                    state = DONE;
                } else if (c == '\0') {
                    token.type = ERROR;  // Unterminated string.
                    state = DONE;
                } else {
                    token.value[valIndex++] = c;
                }
                break;

            case IN_CHAR_CONST:
                if (c == '\'') {
                    token.type = CHAR_CONST;
                    state = DONE;
                } else if (c == '\0') {
                    token.type = ERROR;  // Unterminated char.
                    state = DONE;
                } else {
                    token.value[valIndex++] = c;
                }
                break;

            case IN_COMMENT:
                if (c == '\n' || c == '\0') {
                    token.type = COMMENT;
                    state = DONE;
                } else {
                    token.value[valIndex++] = c;
                }
                break;
        }
    }

    *src = s;
    return token;
}

int main() {
    const char* code =
            "using System;\n"
            "class HelloWorld {\n"
            "    static void Main(string[] args) {\n"
            "        int x = 10;\n"
            "        float y = 3.14f;\n"
            "        string s = \"Hello World\";\n"
            "        char c = 'A';\n"
            "        // This is a comment\n"
            "        Console.WriteLine(s);\n"
            "    }\n"
            "}\n";

    Token token;

    clock_t start_time = clock(); // Record start time

    do {
        token = getNextToken(&code);
        if (token.type != ERROR && token.value[0] != '\0') {
            printf("<%s, ", token.value);
            switch (token.type) {
                case RESERVED_WORD:   printf("RESERVED_WORD>\n"); break;
                case IDENTIFIER:     printf("IDENTIFIER>\n"); break;
                case SEPARATOR:      printf("SEPARATOR>\n"); break;
                case OPERATOR:       printf("OPERATOR>\n"); break;
                case DECIMAL_NUMBER: printf("DECIMAL_NUMBER>\n"); break;
                case FLOAT_NUMBER:   printf("FLOAT_NUMBER>\n"); break;
                case STRING_CONST:   printf("STRING_CONST>\n"); break;
                case CHAR_CONST:     printf("CHAR_CONST>\n"); break;
                case COMMENT:        printf("COMMENT>\n"); break;
                default:             printf("ERROR>\n"); break;
            }
        }
    } while (token.type != ERROR && token.value[0] != '\0');

    clock_t end_time = clock(); // Record end time
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Lexer executed in %f seconds.\n", execution_time);

    return 0;
    }