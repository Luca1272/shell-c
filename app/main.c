#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Check if the path is executable
int is_executable(const char* path) { return access(path, X_OK) == 0; }

// Find command in PATH
char* find_in_path(const char* command) {
    char* path_env = getenv("PATH"); // Get PATH environment variable
    if (path_env == NULL) {
        return NULL;
    }

    char* path_copy = strdup(path_env); // Duplicate PATH
    char* dir = strtok(path_copy, ":"); // Tokenize PATH
    static char full_path[1024];

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command); // Build full path
        if (is_executable(full_path)) { // Check if executable
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":"); // Next token
    }
    free(path_copy);
    return NULL;
}

// Fork and execute command
void fork_and_exec_cmd(char* full_path, int argc, char** argv) {
    pid_t pid = fork(); // Create child process
    if (pid == 0) {
        execv(full_path, argv); // Execute command
        perror("execv"); // Handle error
        exit(1);
    }
    else if (pid < 0) {
        perror("fork"); // Handle error
    }
    else {
        int status;
        waitpid(pid, &status, 0); // Wait for child process to finish
    }
}

// Handle user input
int handle_input(char* input) {
    char command[20];
    sscanf(input, "%s", command); // Parse command
    if (!strcmp(command, "exit")) {
        int arg = 0;
        sscanf(input, "%*s %d", &arg); // Parse exit argument
        exit(arg); // Exit shell
    }

    if (!strcmp(command, "echo")) {
        printf("%s\n", input + 5); // Print echo message
        return 1;
    }

    if (!strcmp(command, "pwd")) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd)); // Get current working directory
        printf("%s\n", cwd); // Print current directory
        return 1;
    }

    if (!strcmp(command, "cd")) {
        char* path = input + 3; // Parse path
        if (strcmp(path, "~") == 0) {
            path = getenv("HOME"); // Handle home directory
        }

        if (chdir(path) == 0) { // Change directory
            return 1;
        }
        else {
            printf("cd: %s: No such file or directory\n", path); // Handle error
            return 1;
        }
    }

    if (!strcmp(command, "type")) {
        char arg[20];
        sscanf(input, "%*s %s", arg); // Parse argument
        if (!strcmp(arg, "exit") || !strcmp(arg, "echo") || !strcmp(arg, "type") ||
            !strcmp(arg, "pwd")) {
            printf("%s is a shell builtin\n", arg); // Built-in command
            return 1;
        }
        else {
            char* path = find_in_path(arg); // Find in PATH
            if (path) {
                printf("%s is %s\n", arg, path); // Print path
                return 1;
            }
            else {
                printf("%s: not found\n", arg); // Command not found
                return 1;
            }
        }
    }
    else {
        char* argv[10];
        int argc = 0;
        char* token = strtok(input, " "); // Tokenize input
        while (token != NULL && argc < 10) {
            argv[argc++] = token; // Parse arguments
            token = strtok(NULL, " ");
        }

        argv[argc] = NULL;
        char* cmd_path = find_in_path(argv[0]); // Find command path

        if (cmd_path) {
            fork_and_exec_cmd(cmd_path, argc, argv); // Execute command
            return 1;
        }
        else {
            return 0; // Command not found
        }
    }
}

// Main function
int main() {
    while (true) {
        printf("$ "); // Prompt
        fflush(stdout);
        char input[100];
        fgets(input, 100, stdin); // Get user input
        input[strlen(input) - 1] = '\0'; // Remove newline
        if (!handle_input(input)) {
            printf("%s: command not found\n", input); // Command not found
        }
    }
    return 0;
}