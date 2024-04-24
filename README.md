cshell: Command-Line Interpreter with Built-in Functionality
cshell is a command-line interpreter developed as part of an assignment, aimed at providing users with a versatile shell environment with built-in functionalities and support for environment variables.

Features
Built-in Commands: cshell supports several built-in commands, including:
exit: Terminate the shell cleanly.
log: Display the history of executed commands with timestamps and return codes.
print: Print arguments provided to the command.
theme: Change the color theme of the shell output.
Interactive and Script Modes: The shell operates in two modes:
Interactive Mode: Prompt users for input and execute commands in real-time.
Script Mode: Read commands from a script file and execute them sequentially.
Efficient Command Execution: Efficient execution of commands, including non-built-in ones, achieved through proper utilization of fork() and exec() system calls, ensuring streamlined output redirection and error handling.
Usage
Interactive Mode
To start cshell in interactive mode, simply run the executable without any command-line arguments:

bash
Copy code
./cshell
Script Mode
To execute commands from a script file, provide the filename as a command-line argument:

bash
Copy code
./cshell script_file.txt
Installation
Clone the repository:
bash
Copy code
git clone https://github.com/your_username/cshell.git
Compile the source code:
Copy code
gcc cshell.c -o cshell
