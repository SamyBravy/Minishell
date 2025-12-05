# Minishell: As beautiful as a shell

[![Standard](https://img.shields.io/badge/Standard-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![OS](https://img.shields.io/badge/OS-Linux-yellow.svg)]()
[![Architecture](https://img.shields.io/badge/Architecture-Process%20Management-green.svg)]()

## Overview

**Minishell** is a simplified bash implementation written in C. It is designed to demonstrate mastery of process creation, synchronization, file descriptor management, and signal handling.

The project involves building a shell that can interpret and execute commands, handle pipes and redirections, and manage environment variables, mimicking the behavior of the popular bash shell.

## Installation

### Prerequisites
*   GCC or Clang compiler
*   `make`
*   `readline` library

```bash
# Install readline on Linux (Ubuntu/Debian)
sudo apt-get install libreadline-dev
```

### Compilation
The project utilizes a `Makefile` to manage the build process.

| Command | Action |
|:---:|---|
| `make` | Compiles the **minishell** executable. |
| `make clean` | Removes object files. |
| `make fclean` | Removes object files and the executable. |
| `make re` | Recompiles the project from scratch. |

## Usage

### 1. Start the Shell
Run the executable to start the shell session.

```bash
./minishell
```

### 2. Execute Commands
You can run commands just like in a standard bash shell.

```bash
minicecco:/home/s/Minishell> ls -la
minicecco:/home/s/Minishell> grep "int" main.c | wc -l
minicecco:/home/s/Minishell> echo "Hello 42" > hello.txt
```

## Features & Implementation

### Technical Architecture
*   **Parsing:** Tokenizes input, handles quotes (`'` and `"`), and expands environment variables.
*   **Execution:** Uses `fork()`, `execve()`, and `waitpid()` to execute commands.
*   **Pipes:** Implements inter-process communication using `pipe()` and `dup2()`.
*   **Signals:** Handles `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` to mimic bash behavior.

### Built-in Commands
The shell includes its own implementation of the following built-ins:
*   `echo` (with `-n`)
*   `cd` (relative and absolute paths)
*   `pwd`
*   `export`
*   `unset`
*   `env`
*   `exit`

### Redirections
Supports standard input/output redirections:
*   `<` : Input redirection
*   `>` : Output redirection
*   `<<` : Here-document
*   `>>` : Append output
