Minishell
==========

Overview
--------
Minishell is a didactic project that aims to create a simple shell, similar to *bash*.


Consists in coding a command-line interface in C, that interprets commands and executes them.

To clone
--------

```shell
git clone --recurse-submodule git@github.com:d-branco/minishell-42.git
```
`--recurse-submodule` is needed to include the *libft* library submodule.

## To build
```shell
make
```

## To run
```shell
./minishell
```

Project Instructions
====================
## Restrictions
- [ ] Use **one global variable** at most (to indicate a received signal);
- [ ] Follow the [norm](doc/normv4.pdf);
- [ ] Limited use of function to the [allowed bellow](#allowed-functions).
## Comand Prompt
 - [ ] **display a prompt** when waiting for a command;
 - [ ] have a working **history**.
## Command Execution
- [ ] Execute commands with their parameters;
	- [ ] Handle relative and absolute paths;
	- [ ] Handle PATH environment variable;
- [ ] Support pipes (`|`) to connect commands;
- [ ] Support redirections:
  - [ ] `<` (input redirection);
  - [ ] `>` (output redirection);
  - [ ] `<<` (here-document);
  - [ ] `>>` (append output redirection).
## Command Line Parsing
- [ ] Parse command lines with quotes (single `'` and double `"`);
- [ ] Ignore unclosed quotes and special characters (like backslash`\` or semicolon `;`);
- [ ] Handle environment variables `($NAME)` expansion;
- [ ] Handle exit status `$?`;
- [ ] handle `ctrl-C`, `ctrl-D`, and `ctrl-\` signals (similar to bash);
- [ ] `BONUS` Handle wildcards `*` in the current working directory;
- [ ] `BONUS` Handle logical operators `AND` (`&&`) and `OR` (`||`) with parentheses.
## Builtins commands
- [ ] `echo` with option `-n`;
- [ ] `cd` with relative or absolute path;
- [ ] `pwd` without options;
- [ ] `export` without options;
- [ ] `unset` without options;
- [ ] `env` without options or arguments;
- [ ] `exit` without options.

Directory organization
-----------------
```txt
minishell/
├── include/
│   ├── minishell.h
│   └── <features>.h
├── srcs/
│   ├── main.c
│   ├── <feature>/
│   └── utils/
├── libft/
│   ├── <libraries>/
│   └── Makefile
├── Makefile
├── doc/
│   ├── subject.pdf
│   └── <notes>.md
├── LICENSE
└── README.md
```

Allowed Functions
-----------------
### String and Command Line Functions
- [`readline`](https://man7.org/linux/man-pages/man3/readline.3.html): Reads a line from user input with editing capabilities
- [`rl_clear_history`](https://tiswww.cwru.edu/php/chet/readline/readline.html#index-rl_005fclear_005fhistory): Clears the readline history list
- [`rl_on_new_line`](https://tiswww.cwru.edu/php/chet/readline/readline.html#index-rl_005fon_005fnew_005fline): Tells readline that the cursor has moved to a new line
- [`rl_replace_line`](https://tiswww.cwru.edu/php/chet/readline/readline.html#index-rl_005freplace_005fline): Replaces the current readline buffer content
- [`rl_redisplay`](https://tiswww.cwru.edu/php/chet/readline/readline.html#Redisplay): Updates the display according to the current readline buffer
- [`add_history`](https://man7.org/linux/man-pages/man3/history.3.html): Adds a line to the readline history
### Standard I/O Functions
- [`printf`](https://man7.org/linux/man-pages/man3/printf.3.html): Prints formatted output to stdout
- [`write`](https://man7.org/linux/man-pages/man2/write.2.html): Writes data to a file descriptor
- [`read`](https://man7.org/linux/man-pages/man2/read.2.html): Reads data from a file descriptor
- [`perror`](https://man7.org/linux/man-pages/man3/perror.3.html): Prints a descriptive error message to stderr
### Memory Management
- [`malloc`](https://man7.org/linux/man-pages/man3/malloc.3.html): Allocates memory from the heap
- [`free`](https://man7.org/linux/man-pages/man3/free.3.html): Deallocates previously allocated memory
### File Operations
- [`access`](https://man7.org/linux/man-pages/man2/access.2.html): Checks if process can access a file
- [`open`](https://man7.org/linux/man-pages/man2/open.2.html): Opens a file and returns a file descriptor
- [`close`](https://man7.org/linux/man-pages/man2/close.2.html): Closes a file descriptor
- [`unlink`](https://man7.org/linux/man-pages/man2/unlink.2.html): Removes a file from the filesystem
- [`getcwd`](https://man7.org/linux/man-pages/man3/getcwd.3.html): Gets the current working directory
- [`chdir`](https://man7.org/linux/man-pages/man2/chdir.2.html): Changes the current working directory
### File Status
- [`stat`](https://man7.org/linux/man-pages/man2/stat.2.html): Gets file information
- [`lstat`](https://man7.org/linux/man-pages/man2/lstat.2.html): Similar to stat, but doesn't follow symbolic links
- [`fstat`](https://man7.org/linux/man-pages/man2/fstat.2.html): Gets file information from a file descriptor
### Process Control
- [`fork`](https://man7.org/linux/man-pages/man2/fork.2.html): Creates a new process by duplicating the calling process
- [`wait`](https://man7.org/linux/man-pages/man2/wait.2.html): Waits for a child process to terminate
- [`waitpid`](https://man7.org/linux/man-pages/man2/waitpid.2.html): Waits for a specific child process to terminate
- [`wait3`](https://man7.org/linux/man-pages/man2/wait3.2.html): Waits for a child process and retrieves resource usage
- [`wait4`](https://man7.org/linux/man-pages/man2/wait4.2.html): Waits for a specific child process and retrieves resource usage
- [`exit`](https://man7.org/linux/man-pages/man3/exit.3.html): Terminates the calling process
- [`execve`](https://man7.org/linux/man-pages/man2/execve.2.html): Replaces the current process image with a new one
### Signal Handling
- [`signal`](https://man7.org/linux/man-pages/man2/signal.2.html): Sets a signal handler for a specific signal
- [`sigaction`](https://man7.org/linux/man-pages/man2/sigaction.2.html): Examines or modifies signal actions
- [`sigemptyset`](https://man7.org/linux/man-pages/man3/sigemptyset.3.html): Initializes a signal set to be empty
- [`sigaddset`](https://man7.org/linux/man-pages/man3/sigaddset.3.html): Adds a signal to a signal set
- [`kill`](https://man7.org/linux/man-pages/man2/kill.2.html): Sends a signal to a process or process group
### File Descriptor Manipulation
- [`dup`](https://man7.org/linux/man-pages/man2/dup.2.html): Duplicates a file descriptor
- [`dup2`](https://man7.org/linux/man-pages/man2/dup2.2.html): Duplicates a file descriptor to a specific number
- [`pipe`](https://man7.org/linux/man-pages/man2/pipe.2.html): Creates a pipe for interprocess communication
### Directory Operations
- [`opendir`](https://man7.org/linux/man-pages/man3/opendir.3.html): Opens a directory stream
- [`readdir`](https://man7.org/linux/man-pages/man3/readdir.3.html): Reads a directory entry
- [`closedir`](https://man7.org/linux/man-pages/man3/closedir.3.html): Closes a directory stream
### Error Handling
- [`strerror`](https://man7.org/linux/man-pages/man3/strerror.3.html): Returns string describing an error code
### Terminal Functions
- [`isatty`](https://man7.org/linux/man-pages/man3/isatty.3.html): Tests if a file descriptor refers to a terminal
- [`ttyname`](https://man7.org/linux/man-pages/man3/ttyname.3.html): Returns name of terminal device
- [`ttyslot`](https://man7.org/linux/man-pages/man3/ttyslot.3.html): Returns the slot number in the terminal table
- [`ioctl`](https://man7.org/linux/man-pages/man2/ioctl.2.html): Controls device parameters
- [`tcsetattr`](https://man7.org/linux/man-pages/man3/tcsetattr.3.html): Sets terminal attributes
- [`tcgetattr`](https://man7.org/linux/man-pages/man3/tcgetattr.3.html): Gets terminal attributes
### Environment Functions
- [`getenv`](https://man7.org/linux/man-pages/man3/getenv.3.html): Gets an environment variable
### Terminal Capability Functions
- [`tgetent`](https://linux.die.net/man/3/tgetent): Loads terminal capability database
- [`tgetflag`](https://linux.die.net/man/3/tgetflag): Gets boolean terminal capability
- [`tgetnum`](https://linux.die.net/man/3/tgetnum): Gets numeric terminal capability
- [`tgetstr`](https://linux.die.net/man/3/tgetstr): Gets string terminal capability
- [`tgoto`](https://linux.die.net/man/3/tgoto): Formats cursor motion strings
- [`tputs`](https://linux.die.net/man/3/tputs): Outputs terminal capability strings

![https://octodex.github.com/images/welcometocat.png](https://octodex.github.com/images/mona-the-rivetertocat.png)