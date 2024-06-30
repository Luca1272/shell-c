# Simple Shell Program

This is a simple shell program written in C that can execute built-in commands like `exit`, `echo`, `pwd`, `cd`, and `type`. It also handles external commands by finding them in the system PATH and executing them.

## Built-in Commands

The following built-in commands are supported by the shell:

- `exit [status]`: Exits the shell with an optional status code.
- `echo [text]`: Prints the provided text to the terminal.
- `pwd`: Prints the current working directory.
- `cd [directory]`: Changes the current directory to the specified one. Use `cd ~` to navigate to the home directory.
- `type [command]`: Indicates whether a command is a built-in shell command or an external command, and shows the path if it is external.

## Example Usage

After running the shell program, you can enter commands at the prompt. Here are some examples:

1. Exiting the shell:
    ```
    $ exit 0
    ```
2. Echoing a message:
    ```
    $ echo Hello, World!
    Hello, World!
    ```
3. Printing the current working directory:
    ```
    $ pwd
    /home/user
    ```
4. Changing the directory:
    ```
    $ cd /path/to/directory
    ```
5. Checking the type of a command:
    ```
    $ type echo
    echo is a shell builtin
    ```

## Handling External Commands

If you enter a command that is not a built-in, the shell will search for it in the directories listed in the PATH environment variable and execute it if found. For example:

```
$ ls
file1.txt file2.txt
```

If the command is not found, you will see an error message:

```
$ unknown_command
unknown_command: command not found
```