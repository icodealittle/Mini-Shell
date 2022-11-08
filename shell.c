//
// Created by Soumeng Chea on 7/16/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // This is new!
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SZ 80

//Command that divided by spaces between the words
char space_cmmd[80][80];
//Current path directory
char current_path[80];

// Create a signal handler
void sigint_handler(int sig) {
    // Ask yourself why 35?
    write(1, "Mini-Shell terminating through signal handler \n", 35);
    exit(0);
}

//Getting current path working directory
int workingDirectory() {

    char *productOutcome = getcwd(current_path, BUF_SZ);
    if (productOutcome == NULL) {
        return 0;
    } else {
        return 1;
    }
}

int cmmd_cd(int cmmd_num);

int cmmd_split(char command[BUF_SZ]);

int cmmd_help();

int cmmd_exit();

int cmmd_gg(); //Calling the guessing game command from A1 of 5006

int cmmd_call(int cmmd_num);

int is_pipe(int cmmd_num);

int execv_pipe(char *command_before[], char *command_after[]);

int main() {

    //Tracker of the command numbers being use in the shell
    int result_tracker;

    //Signal Handler installation
    signal(SIGINT, sigint_handler);
    printf("You're currently in a shell.\n");
    printf("You can only terminate by pressing Ctrl+C\n");
    char command[BUF_SZ];
    while (1) {
        printf("mini-shell>");
        scanf("%[^\n]", command);
        //Dealt with "\n" input
        getchar();

        int cmmd_num = cmmd_split(command);

        if (cmmd_num != 0) { //If the input isn't null
            //Getting the command before implement the pipe
            if (is_pipe(cmmd_num)) {
                char *command_before[BUF_SZ];
                char split[20];
                split[0] = '\0';
                strcat(split, space_cmmd[0]);
                command_before[0] = split;
                int i;
                for (i = 1; i < is_pipe(cmmd_num) - 1; i++) {
                    command_before[i] = space_cmmd[i];
                }
                command_before[i + 1] = NULL;

                //Getting the command after implement the pipe
                char *command_after[BUF_SZ];
                char split_t[20];
                split_t[0] = '\0';
                strcat(split_t, space_cmmd[is_pipe(cmmd_num)]);
                command_after[0] = split_t;
                int j;
                int k;
                for (j = is_pipe(cmmd_num) + 1, k = 1; j < cmmd_num; j++, k++) {
                    command_after[k] = space_cmmd[j];
                }
                if (k == 1) {
                    command_after[k] = NULL;
                } else {
                    command_after[k + 1] = NULL;
                }

                result_tracker = execv_pipe(command_before, command_after);
                if (!result_tracker) {
                    printf("Error!\n");
                    continue;
                }

                //Starting to use/implement any commands without pipe
            } else if (strcmp(space_cmmd[0], "exit") == 0) { //Command: Exit
                result_tracker = cmmd_exit();
                if (!result_tracker) {
                    printf("Error!\n");
                    continue;
                }
            } else if (strcmp(space_cmmd[0], "help") == 0) { //Command: Help
                result_tracker = cmmd_help();
                if (!result_tracker) {
                    printf("Error!\n");
                    continue;
                }
            } else if (strcmp(space_cmmd[0], "cd") == 0) { //Command: cd
                result_tracker = cmmd_cd(cmmd_num);
                if (!result_tracker) {
                    exit(-1);
                } else {
                    result_tracker = workingDirectory();
                    if (!result_tracker) {
                        printf("Error!\n");
                        continue;
                    }
                }

            } else if (strcmp(space_cmmd[0], "guessgame") == 0) { //Command: Guessing Game from 5006 A1
                result_tracker = cmmd_gg();
                if (!result_tracker) {
                    printf("Error!\n");
                    continue;
                }
            } else {
                result_tracker = cmmd_call(cmmd_num); //Non-built in commands
                if (!result_tracker) {
                    printf("Non-Built in command. Error!\n");
                    continue;
                }
            }

        }

    }

    return 0;
}

//This command spilt any spaces into an array of string.
//Return: the part number that the command has
int cmmd_split(char command[BUF_SZ]) {

    int counter_num = 0;
    int i;
    int j;
    int string_length = strlen(command);

    for (i = 0, j = 0; i < string_length; i++) {
        if (command[i] != ' ') {
            space_cmmd[counter_num][j++] = command[i];
        } else {
            if (j != 0) {
                space_cmmd[counter_num][j] = '\0';
                counter_num++;
                j = 0;
            }
        }
    }

    if (j != 0) {
        space_cmmd[counter_num][j] = '\0';
        counter_num++;
    }

    return counter_num;
}

//This command change the directory of the user location.
//2 Parts: "cd" command and the directory destination.
int cmmd_cd(int cmmd_num) {

    int cmmd_result = 1;

    if (cmmd_num != 2) {
        cmmd_result = 0;
    } else {
        int cmmd_result2 = chdir(space_cmmd[1]);
        if (cmmd_result2 == -1) {
            cmmd_result = 0;
        }
    }

    return cmmd_result;
}

//This command print outputs of any relations to the built-in commands and other information.
int cmmd_help() {

    printf("4 built-in commands in this mini shell.\n");
    printf("Hints: the built-in commands are all in lowercases.\n");
    printf("cd\tmoving up or down the directory tree.\n");
    printf("Help\tprint out all of the built-in commands.\n");
    printf("Exit\tto terminate the shell.\n");
    printf("GuessGame\tplay a Game (guess a number from 0 - 10)\n");

    return 1;
}

//This command is to exit the mini-shells
int cmmd_exit() {

    pid_t exit_cmmd = getpid();
    if (kill(exit_cmmd, SIGTERM) == -1) {
        return 0;
    } else {
        return 1;
    }
}

//Copied from my 5006 A1
//This command is a random number guesssing game.
//The player guess a number in a range of 0 - 10.
int cmmd_gg() {

    srand(time(NULL));
    int random_generate_number = rand() % 10 + 1;
    printf("Pick a number. In a range of 1 - 10 only!\n");
    int player_guess;
    int num_guess = 0;
    int guess_round;

    for (guess_round = 0; guess_round < 5; guess_round++) {

        printf("Pick a number: ");
        scanf("%d", &player_guess);
        num_guess++;
        if (player_guess > random_generate_number) {
            printf("Incorrect. Guess Lower!\n");
            if (guess_round == 4) {
                printf("BOOOO! Try again!");
            }
        } else if (player_guess < random_generate_number) {

            printf("Incorrect. Guess Higher!\n");
            if (guess_round == 4) {
                printf("BOOOO! Try again!");
            }
        } else if (player_guess == random_generate_number) {
            printf("YASSSSSS!!!! YOU GOT THIS BOO!\n");
            break;
        }
    }
    getchar();
    return 1;
}

//This command is for any non built-in commands.
//It's execute in a fork() process.
//Return: error message.
int cmmd_call(int cmmd_num) {

    char *myargv[cmmd_num + 1];
    char i[20];
    i[0] = '\0';

    strcat(i, space_cmmd[0]);

    myargv[0] = i;
    int j;
    for (j = 1; j < cmmd_num; j++) {
        myargv[j] = space_cmmd[j];
    }
    myargv[cmmd_num] = NULL;

    int status_child;
    if (fork() == 0) {
        execvp(myargv[0], myargv);
        exit(1);
    } else {
        wait(&status_child);
        if (WEXITSTATUS(status_child) > 0) {
            printf("Command not found--Did you mean something else?\n");
        }
        return 1;
    }
}

//This command is for pipe implementation.
//Return 0 if there is no pipe. Otherwise, return the index of pipe command.
int is_pipe(int cmmd_num) {

    int i = 0;
    for (i = 0; i < cmmd_num; i++) {
        if (strcmp(space_cmmd[i], "|") == 0) {
            return i + 1;
        }
    }
    return 0;
}

//This command is for an execution of pipe implementation.
//Execute the command before the pipe in the child's child process, 
//It passes the output the parent processor.
//After, it executes the command after the pipe in the child process.
//If either of the 2 command fails, then it returns the error message.
int execv_pipe(char *command_before[], char *command_after[]) {

    int c_status;
    pid_t cmmd_id = fork();
    if (cmmd_id == 0) {
        int fd[2];
        int ret = pipe(fd);

        int c_status2;
        if (fork() == 0) {

            close(fd[0]);
            dup2(fd[1], 1);
            execvp(command_before[0], command_before);
            exit(1);
        }
        wait(&c_status2);
        if (WEXITSTATUS(c_status2) > 0) {
            printf("Command not found--Did you mean something else?\n");
        }
        close(fd[1]);

        dup2(fd[0], 0);
        execvp(command_after[0], command_after);
        exit(1);
    } else {
        wait(&c_status);
        if (WEXITSTATUS(c_status) > 0) {
            printf("Command not found--Did you mean something else?\n");
        }
    }
    return 1;
}
