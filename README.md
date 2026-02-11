# Pipex

A 42 school project that recreates the behavior of shell pipes in C. This program handles the redirection of input/output between commands, mimicking the shell's pipe `|` operator.

## 📋 Description

Pipex replicates the functionality of the shell command:
```bash
< infile cmd1 | cmd2 > outfile
```

The program takes an input file, executes the first command on its content, pipes the output to the second command, and writes the final result to an output file.

## 🚀 Features

### Mandatory Part
- Handles two commands with input and output file redirection
- Proper error handling for file operations and command execution
- Memory management with no leaks

### Bonus Part
- **Multiple pipes**: Handle any number of commands
  ```bash
  ./pipex_bonus infile cmd1 cmd2 cmd3 ... cmdn outfile
  ```
- **Here_doc support**: Read input from standard input until a delimiter is reached
  ```bash
  ./pipex_bonus here_doc LIMITER cmd1 cmd2 outfile
  ```

## 🛠️ Installation

1. Clone the repository:
   ```bash
   git clone git@github.com:forzen03/pipex.git
   cd pipex
   ```

2. Compile the project:
   ```bash
   # For mandatory part
   make

   # For bonus part
   make bonus
   ```

## 📖 Usage

### Mandatory Version
```bash
./pipex infile "cmd1" "cmd2" outfile
```

**Example:**
```bash
./pipex input.txt "grep hello" "wc -l" output.txt
```
This is equivalent to:
```bash
< input.txt grep hello | wc -l > output.txt
```

### Bonus Version

**Multiple commands:**
```bash
./pipex_bonus infile "cmd1" "cmd2" "cmd3" ... "cmdn" outfile
```

**Example:**
```bash
./pipex_bonus input.txt "cat" "grep error" "wc -l" output.txt
```

**Here_doc mode:**
```bash
./pipex_bonus here_doc LIMITER "cmd1" "cmd2" outfile
```

**Example:**
```bash
./pipex_bonus here_doc EOF "cat" "wc -l" output.txt
```
This is equivalent to:
```bash
cat << EOF | wc -l >> output.txt
```

## 📁 Project Structure

```
pipex/
├── pipex.c              # Main mandatory implementation
├── pipex.h              # Header file for mandatory part
├── pipex_utils.c        # Utility functions
├── pipex_utils2.c       # Additional utility functions
├── pipex_bonus.c        # Main bonus implementation
├── pipex_bonus.h        # Header file for bonus part
├── pipex_utils_bonus.c  # Bonus utility functions
├── pipex_utils2_bonus.c # Additional bonus utilities
├── first_process_bonus.c# First child process handler
├── mid_process_bonus.c  # Middle processes handler
├── last_process_bonus.c # Last child process handler
├── here_doc_bonus.c     # Here_doc implementation
├── Makefile             # Build configuration
└── libft/               # Custom C library
```

## 🔧 Makefile Commands

| Command | Description |
|---------|-------------|
| `make` | Compile mandatory part |
| `make bonus` | Compile bonus part |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and executables |
| `make re` | Recompile the project |

## 💡 How It Works

1. **Parse arguments**: Extract input file, commands, and output file from command line
2. **Create pipes**: Set up inter-process communication channels
3. **Fork processes**: Create child processes for each command
4. **Redirect I/O**: Use `dup2()` to redirect stdin/stdout through pipes and files
5. **Execute commands**: Use `execve()` to run the specified commands
6. **Wait for completion**: Parent waits for all child processes to finish

## 📚 Key Concepts

- **Fork**: Creating child processes
- **Pipe**: Inter-process communication
- **Dup2**: File descriptor duplication and redirection
- **Execve**: Program execution
- **Wait**: Process synchronization

## ⚙️ Requirements

- GCC compiler
- Make
- Unix-like operating system (Linux/macOS)

## 👤 Author

**njaradat** - 42 Student

## 📄 License

This project is part of the 42 school curriculum.
