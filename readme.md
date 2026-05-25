# POSIX-Compliant Unix Shell

A custom Unix-like command-line interpreter implemented in C. This project utilizes an Abstract Syntax Tree (AST) for recursive command parsing and execution, demonstrating low-level systems programming concepts including process isolation, inter-process communication (IPC), and explicit file descriptor routing.

---

## Core Architecture

* **AST-Based Parsing:** Transforms flat text input into a hierarchical execution tree based on operator precedence.
* **Process Management:** Isolates execution environments using `fork()`, `execvp()`, and `waitpid()` to maintain the integrity of the parent terminal.
* **Multi-Stage Pipelining:** Supports chained execution (`|`) via dynamic allocation of anonymous pipes (`pipe()`) and precise read/write descriptor management.
* **I/O Redirection:** Handles standard stream routing (`<`, `>`, `>>`) through direct file descriptor manipulation (`dup2()`).
* **Logical Operators:** Implements short-circuit evaluation for `&&` and `||` execution blocks.
* **Context-Aware Built-in Execution:** Executes built-in commands natively within the parent process to alter state (e.g., `cd`), but automatically forks into a child process if the built-in is part of an I/O pipeline or redirection block to ensure stream isolation.

---

## Supported Built-in Commands

| Command | Syntax | Description | Specialized Implementations |
| --- | --- | --- | --- |
| **`cd`** | `cd [dir]` | Changes the current working directory using `chdir()`. | Validates argument presence and directory path viability. |
| **`pwd`** | `pwd` | Prints the absolute path of the current directory. | Dynamically queries the OS via `getcwd()`. |
| **`echo`** | `echo [args]` | Outputs text arguments directly to standard output. | Supports flags: `-n` (suppress trailing newline), `-e` (parse `\n`, `\t` escape characters), and basic environmental resolution for `$USER`. |
| **`help`** | `help` | Outputs documentation for available internal shell utilities. | Standard hardcoded command inventory printer. |
| **`exit`** | `exit` | Terminates the current interactive shell instance. | Invokes an immediate system exit with exit code status `0`. |

---

## File Structure

* **`main.c`**: Core driver. Manages the Read-Eval-Print Loop (REPL), terminal prompt rendering, and primary tokenization.
* **`parser.c`**: Implements the recursive descent parser, handling token splitting and building the execution tree hierarchy.
* **`executor.c`**: Evaluates the AST nodes recursively, creating anonymous system pipes and routing descriptors.
* **`commands.c`**: Resolves built-in matching tables, forks built-ins during pipeline states, and executes external binary fallbacks using the system `PATH`.
* **`shell.h`**: Global definition file configuring the `ASTNode` data structures, operation types, and functional prototypes.
* **`CMakeLists.txt`**: Build configuration framework compiling the system source files.
* **`build/`**: Target binaries generation workspace.
* **`myshell`**: Compiled production-ready executable artifact.

---

## Technical Flow Chart

### Execution Layer Routing (`execute_command_io`)

```
                     [Command Token]
                            |
               Is it a registered Built-in?
               /                          \
            (Yes)                         (No)
             /                              \
Are streams redirected?             Invoke launch_external_io()
      /          \                          |
   (Yes)         (No)                   fork() child
   /                \                       |
fork() child     Run in Parent        dup2() standard streams
   |                 Process                |
dup2() streams       |                  execvp() binary execution
   |                 |                      |
Run Built-in     Run Built-in         waitpid() collected by parent

```

---

## Build Instructions

This project utilizes CMake for build system automation.

1. Generate the compiler layout inside the tracking directory:

```bash
cmake -S . -B build

```

2. Compile the binaries:

```bash
cmake --build build

```

3. Run the interactive interpreter:

```bash
./build/myshell

```

---

## Detailed Usage Examples

### 1. Pipelining and Filtering

Chain the execution of multiple independent software processes across common standard pipelines:

```bash
ls -l | grep source | wc -l

```

### 2. File Stream Redirection

Redirect files to input descriptors or capture outputs into continuous standard file logs:

```bash
cat < input.txt > output.txt
echo -e "Line One\tLine Two\nEnd of Log" >> output.txt

```

### 3. Logical Operations and Conditionals

Leverage short-circuit validation mechanisms for robust runtime orchestration scripts:

```bash
mkdir new_project && cd new_project || echo "Initialization failed"

```