# minishell

A simple shell implementation in C, built to emulate basic shell behavior and provide command-line interface functionality.

This project replicates the functionality of a Unix shell, handling commands, arguments, environment variables, and essential shell operations. It showcases command parsing, process management, and file descriptor manipulation.

## Structure

### src/

The `src` directory contains the core source code for the shell's implementation:

- minishell.c: The main file that initializes the shell, reads user input, and executes commands.
- parser.c: Handles command parsing, tokenizing input to interpret commands and arguments.
- builtins.c: Implements built-in shell commands like `cd`, `echo`, `exit`, and `env`, providing basic shell functionality.
- executor.c: Manages command execution, including handling pipes and file redirections.
- utils.c: Contains utility functions to handle memory allocation, error messages, and other supportive tasks.

### include/

The `include` directory holds the header files for the project:

- minishell.h: Declares essential structures, global variables, and function prototypes for shell components.

### Makefile

The Makefile automates the build process, compiling all source files and creating the executable for *minishell*.

## Features

- **Command Parsing**: Supports basic command parsing with arguments and environment variable handling.
- **Built-in Commands**: Implements shell built-ins, including `cd`, `echo`, `exit`, and `env`.
- **Process Management**: Executes commands as new processes, with support for piping and redirection.
- **Error Handling**: Displays informative error messages and handles invalid input gracefully.

## Usage

To compile the project, run:

    make

Launch the shell with:

    ./minishell

Once launched, you can enter commands like in a regular shell environment. Use built-in commands or external binaries available in your PATH.

### Example Commands

    echo "Hello, World"
    cd ..
    ls | grep minishell

These commands demonstrate output redirection, navigation, and text filtering using pipes.

