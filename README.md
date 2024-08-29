# Mini-PowerShell
Mini Shell in C - Documentation
Overview
This mini shell program is a simple command-line interface (CLI) developed in C that replicates basic shell functionalities. It allows users to input commands and execute them in a manner similar to a terminal shell. This mini shell supports basic command parsing, process creation, and execution using Windows-specific APIs.

Features
Read user input from the command line.
Parse input into commands and arguments.
Execute commands using a new process.
Continuously run in a loop until terminated.
Files
mini_shell.c: Contains the main source code for the mini shell.
Dependencies
Windows OS: Uses Windows-specific headers (<windows.h> and <process.h>) for process creation and management.
Dev-C++ or other C compilers compatible with Windows.
Compilation
To compile the code, use the following command in the Dev-C++ environment or any compatible C compiler:

bash
Copy code
gcc mini_shell.c -o mini_shell
Usage
Run the compiled executable:

bash
Copy code
./mini_shell
The shell will display a prompt (mini-shell>) where you can type commands like dir, echo Hello, etc.

Code Structure and Function Descriptions
1. #include Directives
c
Copy code
#include <stdio.h>      // For standard input/output functions
#include <stdlib.h>     // For memory allocation and process termination functions
#include <string.h>     // For string manipulation functions
#include <windows.h>    // For Windows-specific functionality
#include <process.h>    // For process control functions (_spawnvp)
These headers provide access to necessary functions for input/output operations, memory management, string handling, and Windows-specific process control.
2. Macros
c
Copy code
#define MAX_COMMAND_LENGTH 1024 // Maximum allowed length for input commands
#define MAX_ARGS 64             // Maximum number of command arguments
#define DELIMITERS " \t\r\n\a"  // Characters used to split input into tokens
Purpose: Define constants used throughout the code for input handling and tokenization.
3. read_line Function
c
Copy code
char *read_line() {
    char *line = (char *)malloc(MAX_COMMAND_LENGTH);
    if (!line) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }
    if (fgets(line, MAX_COMMAND_LENGTH, stdin) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }
    return line;
}
Description: Reads a line of input from the user.
Returns: A pointer to the dynamically allocated string containing the user input.
Error Handling: If memory allocation or input reading fails, it prints an error message and terminates the program.
4. split_line Function
c
Copy code
char **split_line(char *line) {
    int bufsize = MAX_ARGS;
    char **tokens = (char **)malloc(bufsize * sizeof(char *));
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
            tokens = (char **)realloc(tokens, bufsize * sizeof(char *));
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
Description: Splits the input line into individual command tokens (arguments).
Returns: An array of strings (tokens) representing the command and its arguments.
Error Handling: Ensures memory allocation is successful; otherwise, it exits the program.
5. execute Function
c
Copy code
void execute(char **args) {
    if (args[0] == NULL) {
        return; // No command entered
    }

    intptr_t pid = _spawnvp(_P_WAIT, args[0], args);
    if (pid == -1) {
        perror("mini-shell");
    }
}
Description: Executes the command parsed from the user input using the _spawnvp function.
Parameters: args - An array of strings containing the command and its arguments.
Error Handling: Prints an error message if the command fails to execute.
6. main Function
c
Copy code
int main() {
    char *line;
    char **args;
    int status = 1; // Controls the loop; can be modified to implement an exit command

    do {
        printf("mini-shell> ");
        line = read_line();
        args = split_line(line);
        execute(args);

        free(line);
        free(args);
    } while (status);

    return 0;
}
Description: The main loop of the shell, responsible for repeatedly reading input, parsing it, and executing commands.
Flow:
Displays the shell prompt.
Reads input from the user.
Parses the input into commands and arguments.
Executes the command.
Frees allocated memory to prevent memory leaks.
Possible Improvements
Built-in Commands: Add support for built-in commands like exit, cd, etc.
Error Handling Enhancements: More detailed error messages and specific handling of different command errors.
Command History: Implement command history to allow users to recall previous commands.
Signal Handling: Add signal handling to gracefully handle interruptions (like Ctrl+C).
