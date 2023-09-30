#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MaxWordLength 30

// Function to read each word from a file
char *readOneWord(FILE *file) {
    char c;
    int pointer = 0;
    char *word = NULL;

    // Read characters from the file until EOF or max word length is reached
    while ((c = fgetc(file)) != EOF) {
        // Check if the character is alphabetic
        if (isalpha(c) && pointer < MaxWordLength) {
            // Allocate memory for the word if not already allocated
            if (word == NULL) {
                word = (char *)malloc((MaxWordLength + 1) * sizeof(char));
            }
            // Store alphabetic characters in the word
            word[pointer++] = c;
        } else if (pointer > 0) {
            // If non-alphabetic character encountered and some characters already read, break
            break;
        } else {
            // If non-alphabetic character encountered and no characters read, continue
            continue;
        }

        // If the word length exceeds MaxWordLength, read and discard the rest of the word
        if (pointer >= MaxWordLength) {
            while ((c = fgetc(file)) != EOF && isalpha(c)) {
                // Discard characters from the beginning of the word
            }
            // Free the memory for the word
            free(word);
            word = NULL;
            pointer = 0;
        }
    }

    // Null-terminate the word
    if (word != NULL) {
        word[pointer] = '\0';
    }

    // If word is NULL and EOF is reached, return NULL
    if (word == NULL && c == EOF) {
        return NULL;
    }

    return word;
}

// Function to check if a letter is a consonant (not a vowel)
int isConsonant(char letter) {
    char vowels[] = "AEIOUaeiou"; // Define vowels
    return strchr(vowels, letter) == NULL; // If the letter is not in vowels, it's a consonant
}

// Function to check if a word has double consonants
int hasDoubleConsonants(const char *word) {
    int length = strlen(word);
    for (int i = 0; i < length - 1; i++) {
        if (isConsonant(word[i]) && isConsonant(word[i + 1]) && word[i] == word[i + 1]) {
            return 1; // Contains double consonants
        }
    }
    return 0; // Does not contain double consonants
}

int main() {
    char inputfilename[] = "C:\\Users\\tretiak\\CLionProjects\\SystemProgramming_lab1\\text.txt";
    char outputfilename[] = "C:\\Users\\tretiak\\CLionProjects\\SystemProgramming_lab1\\output.txt";
    FILE *inputFile = fopen(inputfilename, "r");
    FILE *outputFile = fopen(outputfilename, "w");
    if (inputFile == NULL) {
        perror("Error opening the file");
        return 1;
    }

    if (outputFile == NULL) {
        perror("Error opening the output file");
        return 1;
    }

    char uniqueWords[1000][MaxWordLength + 1]; // Array to store unique words
    int uniqueWordCount = 0;

    char *word;
    while ((word = readOneWord(inputFile)) != NULL) {
        if (strlen(word) > 0 && hasDoubleConsonants(word)) {
            int isDuplicate = 0;

            // Check for duplicates in the array of unique words
            for (int i = 0; i < uniqueWordCount; i++) {
                if (strcmp(word, uniqueWords[i]) == 0) {
                    isDuplicate = 1;
                    break;
                }
            }

            // If not a duplicate, copy the word to the array of unique words
            if (!isDuplicate) {
                strcpy(uniqueWords[uniqueWordCount], word);
                uniqueWordCount++;
                fprintf(outputFile, "%s\n", word);
            }
        }
        free(word); // Free the memory allocated by readOneWord
    }

    printf("Words with double consonants:\n");
    for (int i = 0; i < uniqueWordCount; i++) {
        printf("%s\n", uniqueWords[i]);
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

