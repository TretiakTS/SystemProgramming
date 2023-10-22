#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 26

typedef struct {
    int num_states;
    int num_symbols;
    int start_state;
    int num_final_states;
    int final_states[MAX_STATES];
    int transitions[MAX_STATES][MAX_SYMBOLS];
} FiniteAutomaton;

// Reads the automaton definition from a file
bool read_automaton(const char* filename, FiniteAutomaton* fa) {
    FILE* file = fopen(filename, "r");
    if (!file) return false;

    fscanf(file, "%d", &fa->num_symbols);
    fscanf(file, "%d", &fa->num_states);
    fscanf(file, "%d", &fa->start_state);
    fscanf(file, "%d", &fa->num_final_states);

    for (int i = 0; i < fa->num_final_states; i++) {
        fscanf(file, "%d", &fa->final_states[i]);
    }

    // Initialize all transitions to -1 (indicating no transition)
    for (int i = 0; i < fa->num_states; i++) {
        for (int j = 0; j < fa->num_symbols; j++) {
            fa->transitions[i][j] = -1;
        }
    }

    int s, s_prime;
    char a;
    while (fscanf(file, "%d %c %d", &s, &a, &s_prime) != EOF) {
        fa->transitions[s][a - 'a'] = s_prime;
    }

    fclose(file);
    return true;
}

// Uses DFS to check if the word is accepted by the automaton
bool dfs(FiniteAutomaton* fa, int current_state, const char* word) {
    // Base case: if we've reached the end of the word
    if (*word == '\0') {
        for (int i = 0; i < fa->num_final_states; i++) {
            if (current_state == fa->final_states[i]) return true;
        }
        return false;
    }

    char symbol = *word - 'a';
    int next_state = fa->transitions[current_state][symbol];
    if (next_state == -1) return false;

    return dfs(fa, next_state, word + 1);
}

int main() {
    FiniteAutomaton fa;

    // Read the automaton from the file
    if (!read_automaton("C:\\Users\\tretiak\\CLionProjects\\System_Programming_lab_2\\input.txt", &fa)) {
        printf("Error reading automaton file.\n");
        return 1;
    }

    char w0[100];
    printf("Enter the fixed word (w0): ");
    scanf("%s", w0);

    // Check if the word is accepted by the automaton
    bool accepts = dfs(&fa, fa.start_state, w0);

    if (accepts) {
        printf("The automaton accepts words of the form w = w1w0.\n");
    } else {
        printf("The automaton does NOT accept words of the form w = w1w0.\n");
    }

    return 0;
}


