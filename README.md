# Minishell

**As beautiful as a shell**

---

## Summary

**Minishell** is a simple shell implementation written in C, designed to emulate basic Unix shell behavior. This project focuses on handling user commands, parsing arguments, managing processes, and working with file descriptors. It also provides essential built-in commands for interactive command-line functionality.

---

## Features

### Core Functionality

- **Command Execution**:
  - Executes binaries from the `$PATH` or via relative/absolute paths.

- **Built-in Commands**:
  - `echo` (with `-n` option).
  - `cd` (with relative/absolute paths).
  - `pwd`.
  - `export` (for environment variable management).
  - `unset`.
  - `env`.
  - `exit`.

- **Command Parsing**:
  - Supports single and double quotes for grouping arguments.
  - Expands environment variables prefixed with `$`.

- **Redirection**:
  - Input redirection (`<`).
  - Output redirection (`>`).
  - Append mode (`>>`).
  - Heredoc (`<<`).

- **Pipelines**:
  - Implements pipes (`|`) to connect commands.

- **Environment Variables**:
  - Expands `$?` to the exit status of the last command.

- **Signal Handling**:
  - `ctrl-C`: Displays a new prompt.
  - `ctrl-D`: Exits the shell.
  - `ctrl-\`: Does nothing.

---

## Structure

### Directories and Files

#### `src/`

- **`minishell.c`**:
  Main entry point, initializes the shell and manages the REPL loop.

- **`parser.c`**:
  Parses user input into commands and arguments.

- **`builtins.c`**:
  Implements built-in shell commands (`cd`, `echo`, etc.).

- **`executor.c`**:
  Executes parsed commands, handles redirections, and manages pipes.

- **`utils.c`**:
  Helper functions for string manipulation, memory management, and error handling.

#### `include/`

- **`minishell.h`**:
  Declares global structures, function prototypes, and constants.

### Key Files

- **Makefile**:
  Automates compilation with the following targets:
  - `all`, `clean`, `fclean`, `re`.

---

## Usage

### Compilation

To compile the shell, run:
```bash
make
```

### Launching the Shell
Start the shell with:
```
./minishell
```

### Example Commands
Basic Commands:
```
echo "Hello, World!"
cd ..
pwd
```
### Environment Variables:
```
export MY_VAR="Hello"
echo $MY_VAR
```

### Redirections:
```
ls > output.txt
cat < input.txt
echo "Append this" >> output.txt
```

### Pipelines:
```
ls | grep minishell
```

### Heredoc:
```
cat << EOF
This is a heredoc example.
EOF
```

## Project Requirements

### Allowed Functions:
readline, malloc, free, fork, execve, pipe, dup, dup2, and more.

### Memory Management:
Ensure no memory leaks (except those caused by readline).

### Norm Compliance:
All code must adhere to the 42 Norm.
Learning Outcomes

### Process Management:
Learn to manage child processes using fork and execve.

### File Descriptor Manipulation:
Handle redirections and pipelines with dup and dup2.

### Interactive Input:
Use readline for dynamic command-line interaction.

### Shell Basics:
Understand the core principles of Unix shells.

Minishell – A glimpse into the world of command-line interfaces! 🐚
