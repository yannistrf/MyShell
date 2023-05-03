# MyShell
Ioannis Triantafyllopoulos sdi2000193

A simple Unix based shell, implemented in C.
To compile run "make" and then "./mysh" to execute. You can also run
"make run" to compile and execute however the signal behavior may be
altered because of the make parent process. Run "help" too see all of
the built in commands.

## Parsing (parser.c)
Responsible for the parsing is the struct CommandParser. It basically has
4 jobs:
1. To create tokens seperated by semicolons. (semicolon_separation)
2. To create tokens seperated by pipes. (pipe_separation)
3. To extract the arguments and handle any redirections
4. To decide wether or not the pipe current expression will run on the background

## Redirections (parser.c)
The redirections are handled from the parser and are pretty straight forward.
If there is redirection symbol inside the arguments (<, >, >>) expect a file
after that and manage the file descriptors accordingly. The umask syscall is
also being used so that the created file can have the proper permissions. 

## Pipes (pipes.c parser.c)
After the parser has generated the pipe seperated tokens, the actual pipes
are created. The set of the pipes that exist each time is represented from
the Pipes struct. For each process included in the pipe seperated list,
the following must happen: its 0 file descriptor must be replaced from the
previous pipe's read end and its 1 file descriptor must be replaced from the
next pipe's write end (handle_pipes).

## Background commands (proc.c parser.c)
For every pipe expression, the ids of the processes included are saved. If the
parser has decided that the expression will run on the foreground then after
the children are created the parent process is going to wait for all of them
to finish. The waitpid call is used instead of the simple wait call because
there is the possibility of a background process finishing at the same time.
If that happens then the bg process will get cleaned and the fg process will
turn into a zombie. If the expression is going to run on the background, this step
will be skipped and the following will happen: after every "command handling"
taken from the user, there will be an attempt to clean any finished processes.
This is happening through the waitpid call with the WNOHANG flag which doesn't
block the execution of the program. In that way, there are no zombie processes
left.
Note that for an expression to run on the background it needs to end with a '&'
and to be space seperated with the previous word.

## Wild characters (wildchar.c)
When there are arguments with wild characters in them, we need to check for any
successful matches and if there are any we must add them to the current argument
list. If there are no matches, we need to handle the wild character argument as
if it was a normal argument and also added to the argument list. The matches are
performed through the glob api.

## Aliases (alias.c)
After the pipe separation and the argument extraction, for each process to get
executed, a check happens in case the first argument is an alias. All of the
aliases and their actual commands are kept inside the Aliases struct. There
exists a table with pairs (alias - command) which is dynamically managed.
Note that with this implementation there can't be aliases created for complex
commands.

## Signals (sig.c)
After the shell is initialised, we modify the signal receiving behaviour of our
shell process. Basically for the rest of its life it will ignore the SIGINT and
SIGTSTP signals. On the other hand, when a child is created, inheriting the same 
behavior, we change back to the default handlers for those signals. The child
processes will receive the signal without the parent taking any action since they
all belong to the same process group and the signals received from the parent
are distributed to the rest of the processes too. Press Ctrl+C (SIGINT) and Ctrl+Z
(SIGTSTP) to test it.

## History (history.c)
The command history can be shown to the user by executing the "history" command.
After that they can use the past commands by using the correct number.
e.g.
History table

#0 ls -la
#1 echo hello there
#2 wc -l

We can execute a command that looks like this
$ #1 > file.txt ; #0 | #2 ; sleep 3 &

All of the history is kept in the History struct, which is actually a table that
holds a maximum number of the previous input lines from the user.