#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // Windows-specific headers
#include <process.h>  // For _spawnvp instead of fork and execvp

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
#define DELIMITERS " \t\r\n\a"

// Function to read a line of input from the user
char *read_line() {
    char *line = (char *)malloc(MAX_COMMAND_LENGTH); // Casting malloc to char*
    if (!line) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }
    // Read input from the user
    if (fgets(line, MAX_COMMAND_LENGTH, stdin) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }
    return line;
}

// Function to split a line into tokens (command and arguments)
char **split_line(char *line) {
    int bufsize = MAX_ARGS;
    char **tokens = (char **)malloc(bufsize * sizeof(char *)); // Casting malloc to char**
    char *token;
    int position = 0;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIMITERS);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = (char **)realloc(tokens, bufsize * sizeof(char *)); // Casting realloc to char**
            if (!tokens) {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

// Function to execute the command
void execute(char **args) {
    if (args[0] == NULL) {
        // An empty command was entered
        return;
    }

    // Create a new process
    intptr_t pid = _spawnvp(_P_WAIT, args[0], args); // Synchronous execution
    if (pid == -1) {
        perror("Vivek's Power-shell"); // If _spawnvp fails
    }
}

int main() {
    char *line;
    char **args;
    int status = 1;

    // Main loop of the shell
    do {
        printf("Vivek's Power-shell> ");  // Display the prompt
        line = read_line();      // Read input
        args = split_line(line); // Parse the input
        execute(args);           // Execute the command

        free(line);  // Free the allocated memory
        free(args);
    } while (status);

    return 0;
}

