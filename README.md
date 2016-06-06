# MyShell
##A shell implementation in C

Program waits for a command. It reads a command and executes it.
If a command is appended by "&", it will execute the command in the background.
By giving stop <pid>, background program is stoped.
By giving start <pid>, background program is continued.
By giving "jobs", it will list commands and pids running in the background.
When Ctrl-z id pressed, command running in the foreground is stopped.
