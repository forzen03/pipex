# pipex

A 42 school project that recreates the behavior of shell pipes in C, handling command execution and I/O redirection.

## Description

**pipex** replicates the functionality of the shell pipe operator `|`, allowing you to chain commands together with input and output file redirection. The program mimics the shell command:

```bash
< infile cmd1 | cmd2 > outfile
```

The program takes an input file, executes the first command on its content, pipes the output to the second command, and writes the final result to an output file.

### Key Features

- **Input validation** — handles file permissions, command not found, and edge cases.
- **Multiple pipes (bonus)** — supports chaining any number of commands together.
- **Here_doc support (bonus)** — read input from stdin until a delimiter is reached.
- **Proper memory management** — no leaks, all file descriptors properly closed.

## Shell Equivalents

| Pipex Command | Shell Equivalent |
|---------------|------------------|
| `./pipex infile "cmd1" "cmd2" outfile` | `< infile cmd1 \| cmd2 > outfile` |
| `./pipex_bonus infile "cmd1" "cmd2" "cmd3" outfile` | `< infile cmd1 \| cmd2 \| cmd3 > outfile` |
| `./pipex_bonus here_doc LIM "cmd1" "cmd2" outfile` | `cmd1 << LIM \| cmd2 >> outfile` |

## Build

```bash
# Compile pipex
make

# Compile the bonus version
make bonus

# Clean object files
make clean

# Full clean (objects + executables)
make fclean

# Rebuild
make re
```

## Usage

```bash
# Basic usage with two commands
./pipex infile "cmd1" "cmd2" outfile

# Example: grep and count lines
./pipex input.txt "grep hello" "wc -l" output.txt

# Bonus: multiple commands
./pipex_bonus infile "cat" "grep error" "wc -l" outfile

# Bonus: here_doc mode
./pipex_bonus here_doc EOF "cat" "wc -l" output.txt
```

## Project Structure

```
pipex/
├── pipex.c               # Entry point, main process handling
├── pipex.h               # Main header file
├── pipex_utils.c         # Utility functions (path parsing, etc.)
├── pipex_utils2.c        # Additional utility functions
├── pipex_bonus.c         # Bonus entry point with multiple pipe support
├── pipex_bonus.h         # Bonus header file
├── pipex_utils_bonus.c   # Bonus utility functions
├── pipex_utils2_bonus.c  # Additional bonus utilities
├── first_process_bonus.c # First child process handler
├── mid_process_bonus.c   # Middle processes handler
├── last_process_bonus.c  # Last child process handler
├── here_doc_bonus.c      # Here_doc implementation
├── Makefile              # Build system
└── libft/                # Custom C library (libft)
    ├── libft.h
    ├── ft_*.c
    └── Makefile
```

## How It Works

1. **Parse arguments** — extract input file, commands, and output file from command line.
2. **Create pipes** — set up inter-process communication channels using `pipe()`.
3. **Fork processes** — create child processes for each command using `fork()`.
4. **Redirect I/O** — use `dup2()` to redirect stdin/stdout through pipes and files.
5. **Execute commands** — use `execve()` to run the specified commands.
6. **Wait for completion** — parent waits for all child processes using `wait()`.

## Author

**njaradat** — [42 School](https://42.fr/)
