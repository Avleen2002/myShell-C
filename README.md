# cshell: Command-Line Interpreter with Built-in Functionality

cshell is a command-line interpreter developed as part of an assignment, aimed at providing users with a versatile shell environment with built-in functionalities and support for environment variables.

## Features

- **Built-in Commands:** cshell supports several built-in commands, including:
  - `exit`: Terminate the shell cleanly.
  - `log`: Display the history of executed commands with timestamps and return codes.
  - `print`: Print arguments provided to the command.
  - `theme`: Change the color theme of the shell output.

- **Interactive and Script Modes:** The shell operates in two modes:
  - **Interactive Mode:** Prompt users for input and execute commands in real-time.
  - **Script Mode:** Read commands from a script file and execute them sequentially.

- **Efficient Command Execution:** Efficient execution of commands, including non-built-in ones, achieved through proper utilization of fork() and exec() system calls, ensuring streamlined output redirection and error handling.


## Acknowledgements
Special thanks to the creators of the Unix/Linux "ls" command and to professor Hazra Imran, for inspiration and guidance.
