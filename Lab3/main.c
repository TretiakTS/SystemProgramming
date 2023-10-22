#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Define our token types.
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

// List of C# reserved words.
const char* RESERVED_WORDS[] = {"using", "class", "static", "void", "string", "int", "float", "char", "is", NULL};

Token getNextToken(const char** src) {
    Token token = {ERROR, {0}};
    const char* s = *src;

    while (isspace(*s)) s++;  // Skip whitespace.

    // Check for end of input.
    if (*s == '\0') {
        return token;
    }

    // Check for operators.
    if (strchr("=<>!+-*/%", *s)) {
        if (*s == '/' && *(s+1) == '/') {  // Check for comments.
            token.type = COMMENT;
            s += 2;
            while (*s != '\0' && *s != '\n') {
                strncat(token.value, s, 1);
                s++;
            }
            *src = s;
            return token;
        } else {
            token.type = OPERATOR;
            strncat(token.value, s, 1);
            s++;
        }
    }
        // Check for separators.
    else if (strchr(";,.[](){}", *s)) {
        token.type = SEPARATOR;
        strncat(token.value, s, 1);
        s++;
    }
        // Check for string constants.
    else if (*s == '"') {
        s++;
        token.type = STRING_CONST;
        while (*s != '"' && *s != '\0') {
            strncat(token.value, s, 1);
            s++;
        }
        if (*s == '"') {
            s++;
        } else {
            token.type = ERROR;  // Unterminated string.
        }
    }
        // Check for char constants.
    else if (*s == '\'') {
        s++;
        token.type = CHAR_CONST;
        strncat(token.value, s, 1); // Assume char constant is one character.
        s++;
        if (*s == '\'') {
            s++;
        } else {
            token.type = ERROR;  // Unterminated char or too long.
        }
    }
        // Check for numbers.
    else if (isdigit(*s)) {
        if (*s == '0' && *(s+1) == 'x') {
        } else {
            token.type = DECIMAL_NUMBER;
            while (isdigit(*s)) {
                strncat(token.value, s, 1);
                s++;
            }
            if (*s == '.') {
                token.type = FLOAT_NUMBER;
                strncat(token.value, s, 1);
                s++;
                while (isdigit(*s)) {
                    strncat(token.value, s, 1);
                    s++;
                }
                if (tolower(*s) == 'f') {
                    strncat(token.value, s, 1);
                    s++;
                }
            }
        }
    }
        // Check for identifiers and reserved words.
    else if (isalpha(*s) || *s == '_') {
        while (isalnum(*s) || *s == '_') {
            strncat(token.value, s, 1);
            s++;
        }
        token.type = IDENTIFIER;
        for (int i = 0; RESERVED_WORDS[i] != NULL; i++) {
            if (strcmp(RESERVED_WORDS[i], token.value) == 0) {
                token.type = RESERVED_WORD;
                break;
            }
        }
    }
        // Handle unrecognized input.
    else {
        strncat(token.value, s, 1);
        s++;
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

    return 0;
}




