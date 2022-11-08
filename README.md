# Part I

An Assignment for Intro to Computer System -- the mini-shell!

# Part 1 - Signals

Implemented Signal Handler

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // This is new!


// Create a signal handler
void sigint_handler(int sig){
	// Ask yourself why 35?
	write(1,"Terminating through signal handler\n",35); 
	exit(0);
}

int main(){

	// Install our signal handler
	signal(SIGINT, sigint_handler);

	printf("You can only terminate by pressing Ctrl+C\n");

	while(1){
		printf("Running forever!\n");
		sleep(1);
	}

	return 0;
}
```

# Part 2 - Mini-Shell

Build a Mini-Shell. Mini-Shell is inspired by other popular shells. The good news is you can validate the behavior of your shell against the Unix terminal to ensure it is working.

At a high level, implementing mini-shell takes the following components:
1. You will have a function that reads in commands a user types.
2. Your program will execute in an infinite loop.
3. Your program will execute child processes from the shell (the shell is the parent process) based on the command.

## Shell specification requirements:

1. You should have a prompt "mini-shell>" in front of each command that is typed out.
2. The maximum input size(i.e. BUFFER SIZE) on a given line will be 80 characters for our shell. You can specify this as a constant global variable.
3. Implement a signal handler so when the user presses 'Ctrl+C' the text 'mini-shell terminated' is printed out.
4. When you launch a new child process from your shell, the child process should run in the foreground by default until it is completed.
5. If the user adds the pipe '|' then the output of one process should be the input to the next process.
6. Your shell should have some built-in functions that you will have to implement (i.e. functions written in C code). These include 'exit' to terminate your shell, 'cd' so you may change directories, and 'help' explaining all the built-in commands available in your shell.
	1. You can test `cd` by moving up or down the directory tree.
	2. `help` should print out all of the built-in commands that you provide (including your user made one).
	3. `exit` should terminate the shell 
7. You will want to think about how to store these built-in commands.
8. If a command is not found, your shell should print out an error message, and resume execution.
	- e.g. `mini-shell>Command not found--Did you mean something else?`
9. You need to implement and document 1 addditional built-in command that you choose here. 
	1. This built-in command can be of your choosing, but should not be trivial (i.e. not just print a message or clear the screen).
		- An example might be 'history' which stores all of the previous commands a user has typed in and prints them out.
		- Another example may be 'guessinggame' which plays a little game in the terminal for guessing random numbers.
		- Another example may be when the user presses 'tab' a list of programs that can run are found.
		- You can be creative here. The constraint is that you are implementing this functionality in C, and not calling a script.

-----------------------------------------------------
NAME: 

guessgame

SYNOPSIS: 

mini-shell>GuessGame

Pick a number. In a range of 1 - 10 only!

Pick a number: 6

Incorrect. Guess Higher!

Pick a number: 9

Incorrect. Guess Higher!

Pick a number: 10

YASSSSSS!!!! YOU GOT THIS BOO!

DESCRIPTION:

The player guess a random selected number from 0 - 10. He/she has 5 chances to guess the number right or else the player lost the game. In each every move the player take, there's also an output of hints in each round. If they guess the random select number right, they win the game. Otherwise, they lost the game after 5th chances.

NOTES:

- The game generate random number via "int random_generate_number = rand() % 10 + 1;"  0 - 10 integer.
- The 5 guess chances are set up in a loop method.
- It prompt the user answer and compare the guess with the target number, then it print any corresponding hints.
- If users guess the number correctly, then the loop break. Otherwise, the game continues until the player use up their 5 chances. 

-----------------------------------------------------

# Notes:

- You should avoid [fork bombs]
- No memory leaks
- Handling the '|', pipe  between an arbitrary number of processes.

