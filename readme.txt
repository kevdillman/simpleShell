Shell starting steps:
1: unzip dillmanSimpleShell.zip
2: (optional) make clean to remove binary file
3: make, will automatically compile source code, limit maximum
    number of threads, and run the simpleShell executable

make commands:
    all -compile binary
    run -uses all if no binary, limit max number of threads, run executable
    clean -remove executable
    default -use run to ensure current binary and start shell

simpleShell Manual

NAME
    simpleShell - the simple shell

SYNOPSIS
    simpleShell <commnad> [<args>] - A simple shell program allowing the user to launch
        other applications from within it.

simpleShell COMMANDS (Note: all commands are single capital characters)

    C <file1> <file2>
        Copy file1 to file2, if file2 does not exist it will be created.
        ex: C test1.txt easyReading.txt

    D <file>
        Delete the given file.
        ex: D test1.txt

    E <message>
        Echo the message back to the shell as text.
        ex: E testing 1 2 3?

    H
        Help shows the available commands.

    L
        List the contents of the primary working directory.

    M <file>
        Create file if it does not exist, and open it in a text editor.
        Note: use [Ctrl]+[X] to exist
        ex: M test1.txt

    P <file>
        Print the contents of the file to the shell.
        ex: P test1.txt

    Q
        Quit the shell application.

    W [option -x]
        Clears the shell screen by removing all previous text.
            -x: Scrolls down past all prior text making input the top line in the shell.
        ex: W -x

    X <program name>
        Executes the given program.
        ex: X helloWorld
